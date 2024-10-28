# pylint: disable=missing-module-docstring
# pylint: disable=missing-class-docstring
# pylint: disable=missing-function-docstring

import sys
import os
import json
import pickle

sys.path.append(os.path.dirname(os.path.abspath(os.path.dirname(__file__))))


class ConfigParam():
    def __init__(self, json_path):
        super().__init__()

        config_str = open(json_path, 'rt').read()
        config_dict = json.loads(config_str)
        #print (config_str)

        file_dict = config_dict['file_path']
        self.train_path = file_dict['train_path']
        self.val_path = file_dict['val_path']
        self.test_path = file_dict['test_path']
        self.label_path = file_dict['label_path']

        data_dict = config_dict['data_path']
        #self.images_pkl = load_obj(data_dict['image_pkl_path'])
        self.data_size = 0

        model_param_dict = config_dict['model_param']
        self.model_type = model_param_dict['model_type']
        self.image_size = int(model_param_dict['image_size'])
        self.log_dir = model_param_dict['log_dir']
        self.pretrained_path = model_param_dict['pretrained_path']
        self.resume = int(model_param_dict['resume'])
        self.resume_path = model_param_dict['resume_path']

        self.labels_dict, self.labels = read_labels_file(self.label_path)

        train_param_dict = config_dict['train_param']
        self.batch_size = int(train_param_dict['batch_size'])
        self.class_size = len(self.labels)

        self.lr_param_dict = config_dict['lr_param']
        self.max_epoch = int(self.lr_param_dict['max_epoch'])

        self.aug_param_dict = config_dict['augmentation_param']


def load_obj(name):
    with open(name, 'rb') as fin:
        return pickle.load(fin)

# pylint: disable=missing-module-docstring
# pylint: disable=missing-class-docstring
# pylint: disable=missing-function-docstring

def read_labels_file(label_path):
    print(os.getcwd())
    label_file = open(label_path, 'rt', encoding='utf-8')
    labels_dict = {}
    labels = []

    print ('Category info')
    for line in label_file:
        line = line.replace('\n', '')
        split = line.split('\t')
        cate_idx = int(split[0])
        cate_name = split[1]
        labels_dict[cate_name] = cate_idx
        labels.append(cate_name)
        print (cate_idx, cate_name)
    print('-------------')
    label_file.close()

    return labels_dict, labels


if __name__ == '__main__':
    config_path = sys.argv[1]
    config = ConfigParam(config_path)
