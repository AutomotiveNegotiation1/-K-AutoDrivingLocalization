function Adiff = GyroToAcc(Gyro,V)

Mat = [0 V(3) -V(2);
      -V(3) 0 V(1);
      V(2) -V(1) 0];
Adiff = Mat*Gyro;