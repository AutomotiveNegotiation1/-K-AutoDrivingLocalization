#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <complex>
#include <list>
#include <ros/ros.h>

#include "rtwtypes.h"
#include "UWBpos6.h"
#include "UWBpos6_terminate.h"
#include "rt_nonfinite.h"
#include "uwbsubscriber.h"


int main() {
    std::ifstream file("/home/umaps/-K-AutoDrivingLocalization/src/positioning/keti_ws/src/visual/src/processed_data.csv");
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open the file!" << std::endl;
        return 1;
    }

    std::list<double> Ln,Lp,LnC,TagNum,Nanchor,s_time,prevTagHeading,RxIDin,RxDistin,xain,yain,fprevTagPos,ftag_pos_b;
    int Lnsize, Lpsize, Lncsize, TagNumsize, Nanchorsize, headsize, Rxidsize, Rxdistsize, xainsize, yainsize, tagpossize, tag_b_size, s_timesize;

    int k = 0;
    bool flag = false;
    creal_T tag_pos_b[4];
    creal_T prevTagPos[4];

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string value;

        if (k != 0 && k % 8 == 0) {
            Lnsize = Ln.size();
            Lpsize = Lp.size();
            Lncsize = LnC.size();
            TagNumsize = TagNum.size();
            Nanchorsize = Nanchor.size();
            headsize = prevTagHeading.size();
            Rxidsize = RxIDin.size();
            Rxdistsize = RxDistin.size();
            xainsize = xain.size();
            yainsize = yain.size();
            tagpossize = fprevTagPos.size();
            tag_b_size = ftag_pos_b.size();
            s_timesize = s_time.size();

            double aLn;
            double aLp;
            double aLnC;
            double aTagNum;
            double aNanchor;
            double aprevTagHeading;
            double aRxIDin[6];
            double aRxDistin[6];
            double axain[6];
            double ayain[6];
            // double aprevTagPos = new double[tagpossize];
            // double atag_pos_b = new double[tag_b_size];
            double as_time;

            int index = 0;
            for(double val : Ln) {
                aLn = val;
                ++index;
            }

            index = 0;
            for(double val : Lp) {
                aLp = val;
                ++index;
            }

            index = 0;
            for(double val : LnC) {
                aLnC = val;
                ++index;
            }
            
            index = 0;
            for(double val : TagNum) {
                aTagNum = val;
                ++index;
            }

            index = 0;
            for(double val : Nanchor) {
                aNanchor = val;
                ++index;
            }

            index = 0;
            for(double val : prevTagHeading) {
                aprevTagHeading = val;
                ++index;
            }

            index = 0;
            for(double val : RxIDin) {
                aRxIDin[index] = val;
                ++index;
            }

            index = 0;
            for(double val : RxDistin) {
                aRxDistin[index] = val;
                ++index;
            }

            index = 0;
            for(double val : xain) {
                axain[index] = val;
                ++index;
            }

            index = 0;
            for(double val : yain) {
                ayain[index] = val;
                ++index;
            }

            if (flag == false){
                argInit_1x4_creal_T(tag_pos_b);
                argInit_1x4_creal_preT(prevTagPos);
                flag = true;
            }
            double UWBout[21];

            UWBpos6(aLn, aLp, aLnC, aTagNum, aNanchor, aRxIDin, aRxDistin, as_time, tag_pos_b, axain, ayain, prevTagPos, aprevTagHeading, UWBout);

            std::vector<creal_T> tag_pos_est(4);   // Each element should be an array or another container. This depends on the exact type of data.
            std::vector<creal_T> tag_pos_est_aver(4);

            tag_pos_est[0].re = UWBout[0];
            tag_pos_est[1].re = UWBout[1];
            tag_pos_est[2].re = UWBout[2];
            tag_pos_est[3].re = UWBout[3];

            tag_pos_est[0].im = UWBout[4];
            tag_pos_est[1].im = UWBout[5];
            tag_pos_est[2].im = UWBout[6];
            tag_pos_est[3].im = UWBout[7];

            ROS_INFO("tag_pos_est--> (%f,%f), (%f,%f)", tag_pos_est[0].re, tag_pos_est[0].im, tag_pos_est[1].re, tag_pos_est[1].im);
            ROS_INFO("tag_pos_est--> (%f,%f), (%f,%f)", tag_pos_est[2].re, tag_pos_est[2].im, tag_pos_est[3].re, tag_pos_est[3].im);


            double heading_est = UWBout[8];
            ROS_INFO("heading_est--> %f", heading_est);

            tag_pos_est_aver[0].re = UWBout[9];
            tag_pos_est_aver[1].re = UWBout[10];
            tag_pos_est_aver[2].re = UWBout[11];
            tag_pos_est_aver[3].re = UWBout[12];

            tag_pos_est_aver[0].im = UWBout[13];
            tag_pos_est_aver[1].im = UWBout[14];
            tag_pos_est_aver[2].im = UWBout[15];
            tag_pos_est_aver[3].im = UWBout[16];

            ROS_INFO("tag_pos_est_aver--> (%f,%f), (%f,%f)", tag_pos_est_aver[0].re, tag_pos_est_aver[0].im, tag_pos_est_aver[1].re, tag_pos_est_aver[1].im);
            ROS_INFO("tag_pos_est_aver--> (%f,%f), (%f,%f)", tag_pos_est_aver[2].re, tag_pos_est_aver[2].im, tag_pos_est_aver[3].re, tag_pos_est_aver[3].im);

            double headingest_a_aver_v = UWBout[17];
            ROS_INFO("headingest_a_aver_v--> %f", headingest_a_aver_v);

            Ln.clear();
            Lp.clear();
            LnC.clear();
            TagNum.clear();
            Nanchor.clear();
            s_time.clear();
            prevTagHeading.clear();
            
            RxIDin.clear();
            RxDistin.clear();
            xain.clear();
            yain.clear();
            fprevTagPos.clear();
            ftag_pos_b.clear();
        }

        
        std::getline(iss, value, ',');
        if (value != "NaN"){
            Ln.push_back(std::stod(value));
        }

        std::getline(iss, value, ',');
        if (value != "NaN"){
            Lp.push_back(std::stod(value));
        }

        std::getline(iss, value, ',');
        if (value != "NaN"){
            LnC.push_back(std::stod(value));
        }

        std::getline(iss, value, ',');
        if (value != "NaN"){
            TagNum.push_back(std::stod(value));
        }

        std::getline(iss, value, ',');
        if (value != "NaN"){
            Nanchor.push_back(std::stod(value));
        }

        std::getline(iss, value, ',');
        if (value != "NaN"){
            RxIDin.push_back(std::stod(value));
        }

        std::getline(iss, value, ',');
        if (value != "NaN"){
            RxDistin.push_back(std::stod(value));
        }

        std::getline(iss, value, ',');
        if (value != "NaN"){
            s_time.push_back(std::stod(value));
        }

        std::getline(iss, value, ',');
        if (value != "NaN"){
            ftag_pos_b.push_back(std::stod(value));
        }


        std::getline(iss, value, ',');
        if (value != "NaN"){
            xain.push_back(std::stod(value));
        }

        std::getline(iss, value, ',');
        if (value != "NaN"){
            yain.push_back(std::stod(value));
        }

        std::getline(iss, value, ',');
        if (value != "NaN"){
            fprevTagPos.push_back(std::stod(value));
        }

        std::getline(iss, value, ',');
        if (value != "NaN"){
            prevTagHeading.push_back(std::stod(value));
        }
        

        k++;
    }


    return 0;
}
