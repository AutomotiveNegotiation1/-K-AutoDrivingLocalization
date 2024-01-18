#ifndef SENSORDATA_H
#define SENSORDATA_H

struct SensorData {
    int No;
    double Ln;
    double Lp;
    double LnC;
    double PP;
    double Nanchor;
    double RxIDUWB1;
    double RxIDUWB2;
    double RxIDUWB3;
    double RxIDUWB4;
    double RxIDUWB5;
    double RxIDUWB6;
    double RxDistUWB1;
    double RxDistUWB2;
    double RxDistUWB3;
    double RxDistUWB4;
    double RxDistUWB5;
    double RxDistUWB6;
    double s_time;
    double real_tag_pos_b1;
    double imag_tag_pos_b1;
    double real_tag_pos_b2;
    double imag_tag_pos_b2;
    double real_tag_pos_b3;
    double imag_tag_pos_b3;
    double real_tag_pos_b4;
    double imag_tag_pos_b4;
    double xain1;
    double xain2;
    double xain3;
    double xain4;
    double xain5;
    double xain6;
    double yain1;
    double yain2;
    double yain3;
    double yain4;
    double yain5;
    double yain6;
    double real_TagPos1;
    double imag_TagPos1;
    double real_TagPos2;
    double imag_TagPos2;
    double real_TagPos3;
    double imag_TagPos3;
    double real_TagPos4;
    double imag_TagPos4;
    double kf_psi;

    double IMUacc_c1;
    double IMUacc_c2;
    double IMUacc_c3;
    double IMUgyro_c1;
    double IMUgyro_c2;
    double IMUgyro_c3;
    double b_acc_o1;
    double b_acc_o2;
    double b_acc_o3;
    double b_gyro1;
    double b_gyro2;
    double b_gyro3;
    double IMUSel;
    double gyro_psi;
    double cent_pos_est1;
    double cent_pos_est2;
    double cent_pos_est3;
    double cent_vel_est1;
    double cent_vel_est2;
    double cent_vel_est3;
};


// struct IMUSensorData{
//     int No ;
//     double IMUacc_c1;
//     double IMUacc_c2;
//     double IMUacc_c3;
//     double IMUgyro_c1;
//     double IMUgyro_c2;
//     double IMUgyro_c3;
//     double s_time;
//     double b_acc_o1;
//     double b_acc_o2;
//     double b_acc_o3;
//     double b_gyro1;
//     double b_gyro2;
//     double b_gyro3;
//     double IMUSel;
//     double kf_psi;
//     double gyro_psi;
//     double cent_pos_est1;
//     double cent_pos_est2;
//     double cent_pos_est3;
//     double cent_vel_est1;
//     double cent_vel_est2;
//     double cent_vel_est;
// }

#endif

