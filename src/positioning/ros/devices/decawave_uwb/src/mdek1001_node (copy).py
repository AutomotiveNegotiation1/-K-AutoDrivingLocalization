import serial
import time
import numpy as np

cnt = 1
hz = 0

anchor_list = ['991B', '4630', '4302', '1221']

def listToString(str_list):
    result = ""
    for s in str_list:
        result += s + " "
    return result.strip()

def listIntToString(str_list):
    result = ""
    for s in str_list:
        result += str(s) + " "
    return result.strip()

def DWM1001_init(ser):
    ser.write(b'\x0D\x0D')
    while(1):
        raw = ser.readline()
        rawData = raw.split(b'>')
        if rawData[0] == b'dwm':
            break
    raw = ser.readline()
    time.sleep(1)



if __name__ == '__main__':


    ser = serial.Serial("/dev/ttyACM1", 115200, timeout = 1)
    if ser.is_open == False:
        print("serial port >> error")
    time.sleep(1)

    DWM1001_init(ser)

    if len(ser.readline()) < 30:
        ser.write(b'lec\n')
        # time.sleep(0.5)
    while(1):
        raw = ser.readline()
        rawData = raw.split(b',')
        # print(rawData)

        if len(rawData) >= 8:
            numMeas = int(rawData[1])
            start_time = time.perf_counter()
            if cnt > 1:
                hz = 1/(start_time-prev_time)
                hz = round(hz, 2)

            output = [np.nan, np.nan, np.nan, np.nan]
            flags = [0, 0, 0, 0]
            uwb_pos = [0, 0, 0]
            for i in range(0, numMeas):
                anchor = rawData[6*i+3].decode('utf-8')
                dist = float(rawData[6*i+7])
                idx = anchor_list.index(anchor)
                output[idx] = dist
                flags[idx] = 1

            if rawData[-5].decode('utf-8') == 'POS':
                uwb_pos[0] = float(rawData[-4])
                uwb_pos[1] = float(rawData[-3])
                uwb_pos[2] = float(rawData[-2])

            strr = str(cnt)+' '+str(start_time)+' '+str(hz)+' '+str(numMeas)+' '+listIntToString(flags)+' '+listIntToString(output)+' '+listIntToString(uwb_pos)+'\n'

            if cnt%10 == 0:
                print(strr)


            cnt += 1
            prev_time = start_time



    # time.sleep(1)
    ser.close()



