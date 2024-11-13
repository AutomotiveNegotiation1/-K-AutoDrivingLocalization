#! /usr/bin/env python
from inout_deepl.config import ConfigParam
from inout_deepl.data_loader import make_data_loader_inout_version
from inout_deepl.create_model import create_model
from inout_deepl.eval import inference_per_data
import rospy 
from sensor_msgs.msg import Image 
from cv_bridge import CvBridge
import cv2
from PIL import Image as PIL_Image
import numpy as np 
from std_msgs.msg import String 

def callback(msg): 
    bridge = CvBridge() 
    cv_img = bridge.imgmsg_to_cv2(msg, desired_encoding="passthrough")
    cv_img = cv2.cvtColor(cv_img, cv2.COLOR_BGR2RGB) 
    pil_img = PIL_Image.fromarray(cv_img) 
    
    test_loader, _ = make_data_loader_inout_version(pil_img, config, data_type='test')
    #print("process img")

    prediction = inference_per_data(model, test_loader)
    predict_label = int(np.argmax(prediction[0]))
    label_name =''
    if predict_label == 0:
        label_name = 'indoor'
    elif predict_label == 1: 
        label_name = 'outdoor'
    elif predict_label == 2:
        label_name = 'ramp'
    
    print(label_name)
    pub.publish(label_name)
    #test_acc = evaluation(model, test_loader, 'eval_test', 0, save_img=False)
    rate = rospy.Rate(5)
    rate.sleep()



import os 
print(os.getcwd)
rospy.init_node('determine_inout_node', anonymous=True) 
rospy.Subscriber("/zed_ai/zed_node/right/image_rect_color", Image, callback, queue_size=1)
#rospy.Subscriber("/zed2/zed_node/left/image_rect_color", Image, callback, queue_size=10)
pub = rospy.Publisher('/inout_deepl/pred_label', String, queue_size=10)
config_path = '../src/inout_deepl/train_inout_eff1.json'
config = ConfigParam(config_path)

model = create_model(config, is_train=False)

rospy.spin() 

    