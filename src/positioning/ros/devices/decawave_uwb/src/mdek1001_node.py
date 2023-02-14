#!/usr/bin/env python3

import rospy
from decawave_uwb.msg import uwb_anchor
import serial, time
import numpy as np

import subprocess
import sys
import threading
import queue

anchor_list = ['991B', '4630', '4302', '1221', '439D', '89A4',  '4599', '9B8F']
#self.anchor_list = ['991B', '4630', '4302', '1221', '439D', '89A4',  '4599', '9B8F', '1A91']

class UWBProtocol(object):
    cnt = 1
    hz = 0
    numMeans = None
    start_time = None
    prev_time = None
    strr = None

    rospy.init_node('get_uwb_data', anonymous=True)

    pub_uwb = rospy.Publisher('/uwb/ranging', uwb_anchor, queue_size=10)
    uwbMsg = uwb_anchor() 

    uwbMsg.AnchorID = [0, 0, 0, 0, 0, 0, 0, 0, 0]
    uwbMsg.AnchorID = [991, 4630, 4302, 1221, 439, 894, 4599, 98, 191]


    def listToString(self, str_list):
        result = ""
        for s in str_list:
            result += s
        return result.strip()

    def listIntToString(self, str_list):
        result = ""
        for s in str_list:
            result += str(s) + " "
        return result.strip()

    def DWM1001_init(self, ser):
        ser.write(b'\x0D\x0D')
        while(1):
            raw = ser.readline()
            rawData = raw.split(b'>')
            if rawData[0] == b'dwm':
                break
        raw = ser.readline()
        time.sleep(1)


    def pub_uwb_msg_set(self, name, uwbMsg, anchorList, dat):
        uwbMsg.header.stamp = rospy.Time.now()
        uwbMsg.header.frame_id = 'MDEK1001'
        uwbMsg.TagID = name
        
        numMeas = int(dat[1])
        uwbMsg.ActiveNum = numMeas

        uwbMsg.Range = [np.nan, np.nan, np.nan, np.nan, np.nan, np.nan, np.nan, np.nan]

        for i in range(0, numMeas):
            anchor = dat[6*i+3]
            anchor = anchor.decode()
            dist = float(dat[6*i+7])
            idx = anchorList.index(anchor)
            uwbMsg.Range[idx] = dist


        if dat[-5].decode() == 'POS':
            uwbMsg.Pos.position.x = float(dat[-4])
            uwbMsg.Pos.position.y = float(dat[-3])
            uwbMsg.Pos.position.z = float(dat[-2])

        print(uwbMsg)

        return uwbMsg


    def connection_made(self):
        """Called when reader thread is started"""   
        print("wow")


    def data_received(self, name, serial_instance):
        """Called with snipperts received from ther serial port"""
    

    def connection_lost(self, exc):
        """\
        Called when ther serial port is closed or the reader loop terminated 
        oherwise    
        """


class Processing(UWBProtocol):
    """\
    UWM Protocol as used by the ReaderThread based on Multi-UWM. This base class provides funtion implementatons by ljw0904
    """
    def connection_made(self):
        running = True
        
    def connection_lost(self, exc):
        self.transport = None

    def data_received(self, name, data):
        rospy.init_node('get_uwb_data', anonymous=True)

        pub_uwb = self.pub_uwb
        uwbMsg = self.uwbMsg
        self.DWM1001_init(data)

        while not rospy.is_shutdown():
                raw = data.readline()
                # print(raw)
                rawData = raw.split(b',')
                # print(rawData)

                if len(rawData) >= 8:
                    numMeas = int(rawData[1])
                    start_time = time.perf_counter()
                    
                    if self.cnt > 1:
                        self.hz = 1/(start_time - prev_time)
                        self.hz = round(self.hz, 2)


                    output = [np.nan, np.nan, np.nan, np.nan, np.nan, np.nan, np.nan, np.nan, np.nan]
                    flags = [0, 0, 0, 0, 0, 0, 0, 0, 0]

                    for i in range(0, numMeas):
                        anchor = rawData[6*i+3]
                        anchor = anchor.decode()
                        dist = float(rawData[6*i+7])
                        idx = anchor_list.index(anchor)
                        output[idx] = dist
                        flags[idx] = 1
                        
                    
                    strr = str(name)+' '+str(self.cnt)+' '+str(start_time)+' '+str(self.hz)+' '+str(numMeas)+' '+self.listIntToString(flags)+' '+self.listIntToString(output)+'\n'

                    #if cnt%10 == 0:
                    # print(strr)

                    self.pub_uwb_msg_set(name, uwbMsg, anchor_list, rawData)
                    pub_uwb.publish(uwbMsg)
                    

                    self.cnt += 1
                    prev_time = start_time
                    
                    

            # time.sleep(1)
        data.close()            

    def write(self, serial_instance):
        if len(serial_instance.readline()) < 30:
            serial_instance.write(b'lec\n')
        
    def isDone(self):
        return self.running


class ReaderThread(threading.Thread):
    """\
    Implement a serial port read loop and dispatch to a UWM_Protocol instance (like the asyncio.Protocol)
    but do it with threads.
    Calls to close() will close the serial port but i is also possible to just stop() this thread and continue the
    serial port instance otherwise.
    """

    def __init__(self, _, serial_instance, protocol_factory):
        """\
        Initialize thread.
        Note that the seial_instance timeout is set to one second!
        Other settings are not changed.
        """
        super(ReaderThread, self).__init__()
        self.daemon =True
        self.name = _
        self.serial = serial_instance
        self.protocol_factory = protocol_factory
        self.alive = True
        self._lock = threading.Lock()
        self._connenction_made = threading.Event()
        self.protocol = None
        
    def stop(self):
        """Stop the reader thread"""
        self.alive = False
        if hasattr(self.serial, "cancel_read"):
            self.serial.cancel_read()
        self.join(2)

    def run(self):
        """Reader loop"""
        if not hasattr(self.serial, "cancel_read"):
            self.serial.timeout = 1
        self.protocol = self.protocol_factory()
    
        try:
            self.protocol.connection_made()
        except Exception as e:
            self.alive = False
            self.protocol.connection_lost(e)
            # self._conntection_made.set()
            threading.Event()
            return
        error = None
        # self._connenction_made.set()
        threading.Event()
    
        while self.alive and self.serial.is_open:
            try:
                # read all that is there or wait for one byte(blocking)
                data = self.serial
            except serial.SerialException as e:
                # probably some I/O problem such as disconnected USB serial
                # adapters --> exit
                error = e
                break
            else:
                if data:
                    # make a separated try-except for called used code
                    try:
                        self.protocol.data_received(self.name, data)
                    except Exception as e:
                        error = e
                        break
        self.alive = False
        self.protocol.connection_lost(error)
        self.protocol = None

    def write(self, data):
        """Thread safe writing (uses lock)"""
        with self._lock:
            self.serial.write(data)

    def close(self):
        """Close the serial port and exit reader thread (uses lock)"""
        # use the lock to let other threads finish writing
        with self._lock:
            # first stop reading, so that closing cna be done on idle port
            self.stop()
            self.serial.close()

    def connect(self):
        """
        Wait until connection is set up and return the transport and protocol instances.
        """
        if self.alive:
            self._connenction_made.wait()
            if not self.alive:
                raise RuntimeError('connection_lost already called')
            return (self, self.protocol)
        else:
            raise RuntimeError('already stopepd')
        

    def __enter__(self):
        """\
        Enter context hndler. May raise runtimeError in case the connection
        could not be created.
        """
        self.start()
        self._connenction_made.wait()
        if not self.alive:
            raise RuntimeError('conntection_lost already called')
        return self.protocol

    def __exit__(self, exc_type, exc_val, exc_tb):
        """Leave context:close port"""
        self.close()




if __name__ == '__main__':
    ser = []
    serial_list = ['ttyUWB', 'ttyUWB2'] 

    for i, _ in enumerate(serial_list):
        ser.append(serial.Serial("/dev/{}".format(_), 115200, timeout = 1))
        if ser[i].is_open == False:
            print("serial port >> error")
        else:
            print("Successfully opened /dev/{}".format(_))

    p1 = ReaderThread(serial_list[0], ser[0], Processing)
    p2 = ReaderThread(serial_list[1], ser[1], Processing)
    p1.start()
    p2.start()
    p1.join()
    p2.join()

        # with ReaderThread(_, ser, UWM_Protocol) as p:
        #     time.sleep(1)
    




