#!/usr/bin/env python3

import rospy
from decawave_uwb.msg import uwb_anchor
import numpy as np

class Point3D:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

dt = np.dtype([('AnchorID', 'S10'), ('x', 'f'), ('y', 'f'), ('z', 'f'), ('Range', 'f')])

anchor =  np.array([('991B', 0, 0, 2, 0), # AnchorID, x, y, z, range
                    ('4630', 0, 0, 2, 0), 
                    ('4302', 0, 0, 2, 0),
                    ('1221', 0, 0, 2, 0),
                    ('439D', 0, 0, 2, 0),
                    ('89A4', 0, 0, 2, 0),
                    ('4599', 0, 0, 2, 0),
                    ('9B8F', 0, 0, 2, 0),
                    ], dtype=dt)

meas_list = [0, 0, 0, 0, 0, 0, 0, 0]

def callback(data):
    print("callback start!\n")
    print("data.Range", len(data.Range))
    print(data.AnchorID)

    ## initalize uwb tag coordinates
    uwb_tag = Point3D(x=0, y=0, z=1) # z coordinate is initialized to 1 due to the cart

    k = 0

    ## 1. Range values
    for i in range(0,len(data.Range)):
        #print("i: ",i,", data.Range= ", data.Range[i])
        if np.isnan(data.Range[i]):
            continue
        else:
            meas_list[k] = data.Range[i]
            anchor[k]['Range'] = data.Range[i]
        #print("i: ",i,", meas= ", meas_list[k])
        print("anchor ID: ", anchor[i]['AnchorID'] ,anchor[k] ,"range: ",k,", meas= ", anchor[k]['Range'])
        k = k+1

        ## 2. Distance between tag and anchors


    ## 3. Bias


    ## 4. A matrix


    ## 5. Pseudo-inverse


    ## 6. Delta x


    ## 7. Update x

    
    ## Exception (Refer matlab code)


    ## Final output coordinates


    ## Publish coordinates

    #rospy.loginfo(rospy.get_caller_id() + "I heard %f", data.Range)

def uwb_pos_calculator():
    rospy.init_node('uwb_pos_calc', anonymous=True)
    rospy.Subscriber("/uwb/ranging", uwb_anchor, callback)
    rospy.spin()

if __name__ == '__main__':
    #rospy.loginfo("main start!\n")
    uwb_pos_calculator()
