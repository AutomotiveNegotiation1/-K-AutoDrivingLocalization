# /usr/bin/env python3

import numpy as np
import threading
import sympy

#import matplotlib.pyplot as plt

import rospy
from decawave_uwb.msg import uwb_anchor
from sensor_msgs.msg import Imu
from ublox_f9r.msg import gnssPVT
from geometry_msgs.msg import Pose

from mdek1001_utils import get_json_parser, get_device_parser

    
class KetiThread(threading.Thread):
    # (x,y,z) for base A:9B8F
    xA, yA, zA = 0.00, 0.00, 0.00
    
    # (x,y,z) for base B:89A4
    xB, yB, zB = 1.99, 0.00, 0.00
    
    # (x,y,z) for base C:991B
    xC, yC, zC = 0.00, 4.19, 0.00
    
    # (x,y,z) for base D:439D
    xD, yD, zD = 1.99, 4.19, 0.00
    
    # (TA, TB, TC, TD) for four distances
    disArr = [0.0, 0.0, 0.0, 0.0]
    
    # (x,y,z) for the target
    coorArr = [0.0, 0.0, 0.0]
    
    # For WIFI
    c = None
    client = None
    address = None
    rangeData = None
    
    def __init__(self, serialName):
        # Thread initailizae
        threading.Thread.__init__(self)
        
        # Ros Message variance
        self.wow = None
        
        self.portName = serialName.split("/").pop()
        # Ros subscriber initailize
        rospy.init_node('calc_matlab_node', anonymous=True)
      
    def get_uwb_data(self, msg):
        print("wwwwww")
        #self.rangeData = self.convertDistance(msg.Range, msg.AnchorID)
        self.convertDistance(msg.Range)     
        print(self.disArr)
        

    
    def get_ublox_data(self, msg):
        
        pass
        # print("1:", msg)
    
    def setDistance(self, inArr):
        """_summary_

        Args:
            inArr (_type_): _description_

        Returns:
            _type_: _description_
        """
        self.disArr = inArr
        return True
    
    def getDistance(self):
        """_summary_

        Returns:
            _type_: _description_
        """
        return self.disArr
    
    def convertDistance(self, inStr):
        """_summary_

        Args:
            inStr (_type_): _description_

        Returns:
            _type_: _description_
        """
        #range_filter = []
        #anchor_filter = []
        inStrArray = np.array(inStr)
        #ancrhoIdArray = np.array(anchorId)
        for i, arr in enumerate(inStrArray):
            self.disArr[i] = arr
        #    if 'nan' is not arr:
        #        self.disArr[i] = arr
        #        anchor_filter.append(ancrhoIdArray[i])             
        #return range_filter, anchor_filter
        #return inStrArray
    
    def calculateTriPosition(self):
        """_summary_

        Returns:
            _type_: _description_
        """
        xa, ya, da, xb, yb, db, xc, yc, dc = {
            self.xA,
            self.yA,
            self.disArr[0],
            self.xB,
            self.yB,
            self.disArr[1],
            self.xC,
            self.yC,
            self.disArr[2]
        }
        
        x, y =sympy.symbols("x y")
        
        # List of equations
        f1 = (
            2 * x * (xa - xc)
            + np.square(xc)
            - np.square(xa)
            + 2 * x * (ya - yc)
            + np.square(yc)
            - np.square(ya)
            - (np.square(dc) - np.square(da))
        )
        
        f2 = (
            2 * x * (xb - xc)
            + np.square(xc)
            - np.square(xb)
            + 2 * y * (yb - yc)
            + np.square(yc)
            - np.square(yb)
            - (np.square(dc) - np.square(db))
        )

        # Solve the equations
        result = sympy.solve([f1, f2], [x, y])
        locx, locy = result[x], result[y]
        self.coorArr = [locx, locy, None]
        return [locx, locy]
    
    def calculateQuartPosition(self):
        """_summary_

        Returns:
            _type_: _description_
        """
        [a, b] = self.calculateTriPosition()
        z1 = sympy.symbols("z1")
        f1 = (
            np.square(a - self.xD)
            + np.square(b - self.yD)
            + np.square(z1 - self.zD)
            - np.square(self.disArr[3])
        )
        rst1 = sympy.solve(f1, z1)

        z2 = sympy.symbols("z2")
        f2 = (
            np.square(a - self.xB)
            + np.square(b - self.yB)
            + np.square(z2 - self.zB)
            - np.square(self.disArr[1])
        )
        rst2 = sympy.solve(f2, z2)

        sol1 = list(rst1)
        sol2 = list(rst2)

        if complex(sol1[0]).real > complex(sol1[1]).real:
            sol1[0], sol1[1] = sol1[1], sol1[0]

        if complex(sol2[0]).real > complex(sol2[1]).real:
            sol2[0], sol2[1] = sol2[1], sol2[0]

        min1 = abs(sol1[0] - sol2[0])
        min2 = abs(sol1[1] - sol2[1])

        min0 = min(min1, min2)

        out = 0

        if min0 == min1:
            out = (sol1[0] + sol2[0]) / 2
        elif min0 == min2:
            out = (sol1[1] + sol2[1]) / 2

        out = complex(out).real

        self.coorArr = [a, b, out]
        return [a, b, out]
        
        
    def setBaseACoor(self, x, y, z):
        """_summary_

        Args:
            x (_type_): _description_
            y (_type_): _description_
            z (_type_): _description_

        Returns:
            _type_: _description_
        """
        self.xA = x
        self.yA = y
        self.zA = z
        return True
    
    def setBaseBCoor(self, x, y, z):
        """_summary_

        Args:
            x (_type_): _description_
            y (_type_): _description_
            z (_type_): _description_

        Returns:
            _type_: _description_
        """
        self.xB = x
        self.yB = y
        self.zB = z
        return True
    
    def setBaseCCoor(self, x, y, z):
        """_summary_

        Args:
            x (_type_): _description_
            y (_type_): _description_
            z (_type_): _description_
        """
        self.xC = x
        self.yC = y
        self.zC = z
        return True
    
    def setBaseDCoor(self, x, y, z):
        """_summary_

        Args:
            x (_type_): _description_
            y (_type_): _description_
            z (_type_): _description_
        """
        self.xD = x
        self.yD = y
        self.zD = z
        return True
    
    def getFourBaseCoor(self):
        return [self.xA, 
                self.yA, 
                self.zA, 
                self.xB, 
                self.yB, 
                self.zB, 
                self.xC, 
                self.yC, 
                self.zC, 
                self.xD, 
                self.yD, 
                self.zD
                ]
        
    def run(self):
        rospy.Subscriber("/uwb/{}/ranging".format(self.portName), uwb_anchor, self.get_uwb_data)
        rospy.Subscriber('/ublox/imu', Imu, self.get_ublox_data)
        rospy.Subscriber('/ublox/gnss_pvt', gnssPVT, self.get_ublox_data)
        
        self.setBaseACoor(0.00, 0.00, 0.00)
        self.setBaseBCoor(1.99, 0.00, 0.00)
        self.setBaseCCoor(0.00, 4.19, 0.00)
        self.setBaseDCoor(1.99, 4.19, 0.00)

        data = self.calculateQuartPosition()
        print(data)
        rospy.spin()
    
        
if __name__=='__main__':
    config = get_json_parser('./mdek1001_config.json')
    devInfo = get_device_parser(config)
    th = []
    for dev in devInfo:
        conf = devInfo[dev]
        for i, con in enumerate(conf[:]):                     
            if dev == "UWB":
                th.append(KetiThread(con['port']))
                
    for t in th:
        t.start()
        
    for t in th:
        t.join()

        
