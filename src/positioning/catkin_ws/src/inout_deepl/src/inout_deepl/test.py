# pylint: disable=missing-module-docstring
# pylint: disable=missing-class-docstring
# pylint: disable=missing-function-docstring

import os
import sys

from config import ConfigParam
from create_model import create_model
from data_loader import make_data_loader
from eval import evaluation


def main():
    ##################################
    #####      Train Config      #####
    ##################################
    config_path = '../../configs/train_inout_eff1.json'
    config = ConfigParam(config_path)
    
    #mode = int(sys.argv[2])
    #name_type = int(sys.argv[3])  # 0 : log + best, 1 : input name
    #is_align = int(sys.argv[4])   # 0 : is_align, 1: no_align : 1

    config.resume = 2
    config.resume_path = os.path.join(config.log_dir, 'model_ckpt_8.pth')
   
    ##################################
    #####      Load Dataset      #####
    ##################################
    print('==> Preparing data..')
    test_loader, _ = make_data_loader(config, data_type='test')

    ##################################
    #####      Create Model      #####
    ##################################
    model = create_model(config, is_train=False)

    ##################################
    #####   Train & Evaluation   #####
    ##################################
    test_acc = evaluation(model, test_loader, 'eval_test', 0, save_img=False)
    print (test_acc)


if __name__ == '__main__':
    main()
