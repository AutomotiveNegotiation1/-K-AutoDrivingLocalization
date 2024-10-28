# pylint: disable=missing-module-docstring
# pylint: disable=missing-class-docstring
# pylint: disable=missing-function-docstring

import sys
import os
from io import BytesIO

import numpy as np
from PIL import Image

import torch
import torchvision.transforms as transforms
from torch.utils.data import Dataset 

sys.path.append(os.path.dirname(os.path.abspath(os.path.dirname(__file__))))
from inout_deepl.model.efficientnet_pytorch import EfficientNet

#################################
###### Pytorch Data Loader ######
#################################

# inout data version 
def make_data_loader_inout_version(pil_img, config, data_type='test'):
    batch_size = config.batch_size
    is_train = False

    if config.image_size == -1:
        if config.model_type.startswith('efficientnet'):
            config.image_size = EfficientNet.get_image_size(config.model_type)

    dataset = get_dataset(pil_img,
                          config.image_size,
                          aug_param=config.aug_param_dict,
                          is_train=False)

    data_loader = torch.utils.data.DataLoader(dataset=dataset,
                                              batch_size=batch_size,
                                              shuffle=is_train,
                                              num_workers=4)
    #print ('%s Data Count : %d' % (data_type, dataset.num_samples))
    return data_loader, len([0])



# changed for 'in_out', hy.joo (24.10.19)
def get_dataset(pil_img, image_size, aug_param=None, is_train=False):
    transform = get_image_size_and_transform(image_size,
                                            aug_param=aug_param,
                                            is_train=False)
    retDataset = InoutDataset(pil_img, transform)
    return retDataset

def get_image_size_and_transform(image_size, aug_param=None, is_train=True):
    if is_train:
        print ("Train Augmentation : ")
        transform_list = []
        if aug_param['Affine']:
            degree = aug_param['Affine_Param']['degree']
            translate = aug_param['Affine_Param']['translate']
            scales = aug_param['Affine_Param']['scales']
            fillcolor = aug_param['Affine_Param']['fillcolor']
            transform_list.append(
                transforms.RandomAffine(
                    degree,
                    translate=tuple(translate) if len(translate) != 0 else None,
                    scale=tuple(scales) if len(scales) != 0 else None,
                    fill=tuple(fillcolor)
                )
            )
            print ("Random Affine : degree = %f, translate = %s, scale = %s, fillcolor = %s" % (
                    degree,
                    list(map(str, translate)),
                    list(map(str, scales)),
                    list(map(str, fillcolor))
                )
            )

        transform_list.append(transforms.Resize((image_size, image_size), interpolation=2))
        print ("Resize, ", image_size, 2)

        if aug_param['ColorJitter']:
            brightness = aug_param['ColorJitter_Param']['brightness']
            contrast = aug_param['ColorJitter_Param']['contrast']
            saturation = aug_param['ColorJitter_Param']['saturation']
            hue = aug_param['ColorJitter_Param']['hue']
            transform_list.append(transforms.ColorJitter(brightness=brightness, contrast=contrast,
                                                         saturation=saturation, hue=hue))
            print ("Color Jitter : brightness = %f, contrast = %f, saturation = %f, hue = %f" % (
                    brightness,
                    contrast,
                    saturation,
                    hue
                )
            )

        if aug_param['Horizontal_Flip']:
            transform_list.append(transforms.RandomHorizontalFlip())
            print ("Random Horizontal Flip")

        transform_list.append(transforms.ToTensor())
        transform_list.append(transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225]))
        transform = transforms.Compose(transform_list)
    else:
        transform_list = []
        transform_list.append(transforms.Resize((image_size, image_size), interpolation=2))
        transform_list.append(transforms.ToTensor())
        transform_list.append(transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225]))
        transform = transforms.Compose(transform_list)
    return transform

# Custom Inout Dataset 
class InoutDataset(Dataset) : 
    def __init__(self, pil_img, transform):
      
        self.pil_img = pil_img 
        self.transform = transform 

    def __len__(self):
        return len([0]) 
    
    def __getitem__(self, idx):     

        if self.pil_img.mode != 'RGB':
            self.pil_img = self.pil_img.convert('RGB')

        img = self.transform(self.pil_img)

        return img 
