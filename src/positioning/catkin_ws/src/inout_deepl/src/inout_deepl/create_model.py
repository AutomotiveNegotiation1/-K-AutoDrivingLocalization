# pylint: disable=missing-module-docstring
# pylint: disable=missing-class-docstring
# pylint: disable=missing-function-docstring

import sys
import os

import shutil
from collections import OrderedDict

import torch
import torch.nn as nn
import torch.optim as optim
import torch.backends.cudnn as cudnn
from tensorboardX import SummaryWriter

sys.path.append(os.path.dirname(os.path.abspath(os.path.dirname(__file__))))
from inout_deepl.model.efficientnet_pytorch import EfficientNet


def create_model(config, is_train=False):
    model = TrainModelPytorch(config, is_train=is_train)
    return model


class TrainBase():
    def __init__(self, config):
        super().__init__()
        self.model_type = config.model_type
        self.image_size = config.image_size
        self.class_size = config.class_size
        #self.class_cnt = config.class_cnt

        self.data_size = config.data_size
        self.labels = config.labels

        self.batch_size = config.batch_size
        self.pretrained_path = config.pretrained_path
        self.resume = config.resume
        self.resume_path = config.resume_path

        self.log_dir = config.log_dir

        self.best_acc = 0.0
        self.start_epoch = 0
        self.cur_epoch = 0
        self.cur_weights_path = ''

        self.lr_param_dict = config.lr_param_dict


class TrainModelPytorch(TrainBase):
    def __init__(self, config, is_train=False):
        super().__init__(config)

        if is_train:
            self.create_train_model()
        else:
            self.create_eval_model()

        self.set_loss()
        self.set_optimizer()
        self.set_lr_scheduler()
        self.set_summarywriter()

    def __call__(self, images, labels, is_train=False):
        images, labels = images.to(self.device), labels
        loss= -1         
        if is_train:
            self.optimizer.zero_grad()

            outputs = self.net(images)
            loss = self.criterion(outputs, labels)

            loss.backward()
            self.optimizer.step()
        else:
            with torch.no_grad():
                outputs = self.net(images)
            
        return outputs.to('cpu').detach().numpy(), loss

    def create_train_model(self):
        print('==> Building model.. : ', self.model_type)
        self.device = 'cuda' if torch.cuda.is_available() else 'cpu'
        print(self.device)
        print('-------')

        if self.model_type.startswith('efficientnet'):
            if self.resume == 1:
                self.net = EfficientNet.from_pretrained(self.model_type,
                                                        self.pretrained_path)
            else:
                self.net = EfficientNet.from_name(self.model_type)
            num_ftrs = self.net._fc.in_features
            self.net._fc = nn.Linear(num_ftrs, self.class_size)

        self.net = self.net.to(self.device)
        print(self.net)
        if self.device == 'cuda':
            self.net = torch.nn.DataParallel(self.net)
            cudnn.benchmark = True

        if self.resume >= 2:
            self.load_weights()

        self.net.train()

    def create_eval_model(self):
        self.create_train_model()
        self.net.eval()

    def change_train_model(self):
        self.net.train()

    def change_eval_model(self):
        self.net.eval()

    def load_weights(self):
        print ('==> Resuming from checkpoint.. : ', self.resume_path)
        checkpoint = torch.load(self.resume_path)
        self.start_epoch = checkpoint['epoch']

        state_dict = checkpoint['weight']
        new_state_dict = OrderedDict()
        for key, val in state_dict.items():
            if 'module' not in key:
                key = 'module.'+ key
            else:
                key = key.replace('features.module.', 'module.features.')
            new_state_dict[key] = val
        self.net.load_state_dict(new_state_dict)

    def save_weights(self, epoch):
        #self.delete_pth_file(self.cur_weights_path)
        #self.net.train()
        if self.device == 'cuda':
            state = {
                'weight': self.net.module.state_dict(),
                'optimizer': self.optimizer.state_dict(),
                'epoch': epoch,
            }
        else:
            state = {
                'weight': self.net.state_dict(),
                'optimizer': self.optimizer.state_dict(),
                'epoch': epoch,
            }

        if not os.path.exists(self.log_dir):
            os.makedirs(self.log_dir)

        self.cur_weights_path = os.path.join(self.log_dir, 'model_ckpt_%d.pth' % (epoch))
        torch.save(state, self.cur_weights_path)

    def save_best_weights(self):
        self.copy_best_pth_file(self.cur_weights_path)

    def set_loss(self):
        self.criterion = nn.CrossEntropyLoss()

    def set_optimizer(self):
        if self.lr_param_dict['optimizer'] == 'sgd':
            self.optimizer = optim.SGD(self.net.parameters(),
                                       lr=self.lr_param_dict['init_lr'],
                                       momentum=0.9)

    def set_lr_scheduler(self):
        if self.lr_param_dict['lr_scheduler'] == 'step':
            self.lr_scheduler = optim.lr_scheduler.StepLR(
                optimizer=self.optimizer,
                step_size=self.lr_param_dict['epochs_before_decay'],
                gamma=self.lr_param_dict['lr_decay_factor']
            )

    def get_lr(self):
        self.lr_scheduler.step()
        lr_val = self.optimizer.param_groups[0]['lr']
        return lr_val

    def set_summarywriter(self):
        self.summary_writer = SummaryWriter(logdir=self.log_dir)

    def write_summary(self, train_acc, val_acc, test_acc, loss, epoch):
        self.summary_writer.add_scalar('train acc per epoch', train_acc, epoch)
        self.summary_writer.add_scalar('train loss per epoch', loss, epoch)
        self.summary_writer.add_scalar('val acc per epoch', val_acc, epoch)
        self.summary_writer.add_scalar('test acc per epoch', test_acc, epoch)
        

    @staticmethod
    def delete_pth_file(model_path):
        if os.path.isfile(model_path):
            os.remove(model_path)

    @staticmethod
    def copy_best_pth_file(model_path):
        model_base = '/'.join(model_path.split('/')[:-1]) + '/'
        best_pth_file = model_base + 'best_ckpt.pth'
        shutil.copyfile(model_path, best_pth_file)
