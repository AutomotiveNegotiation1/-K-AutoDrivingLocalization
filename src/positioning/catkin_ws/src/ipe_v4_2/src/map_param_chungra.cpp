#include <map_param_chungra.h>
#include <stdlib.h>
#include "PositioningSystem_V5_1_types.h"
// ohoo map parameters
/*
const double s = 4.6857; 
const double roll = 0.0277; 
const double pitch = 4.7510; 
const double yaw = -0.0216; 

const double sA_00 = -0.1791; 
const double sA_01 = -4.7018; 
const double sA_02 = 0.1343; 
const double sA_10 = 4.6391; 
const double sA_11 = -0.0002; 
const double sA_12 = -0.1005; 
const double sA_20 = 0.0216; 
const double sA_21 = 0.0277; 
const double sA_22 = 0.9994; 

const double SLAMposInit_0 = 2.7476; 
const double SLAMposInit_1 = 77.4887; 
const double SLAMposInit_2 = 0.0; 


const double MapParam[16] = {s, roll, pitch, yaw, sA_00, sA_01, sA_02, sA_10, sA_11, sA_12, sA_20, sA_21, sA_22, SLAMposInit_0, SLAMposInit_1, SLAMposInit_2};
*/

/* ohjun map parameters 
const double s = 3.7138; 
const double roll = 0.012; 
const double pitch = 4.722; 
const double yaw = -0.0216; 

const double sA_00 = -0.035; 
const double sA_01 = -3.701; 
const double sA_02 = 0.0456; 
const double sA_10 = 3.6505; 
const double sA_11 = 0.1008; 
const double sA_12 = -0.0801; 
const double sA_20 = 0.0216; 
const double sA_21 = 0.0121; 
const double sA_22 = 0.9997; 

const double SLAMposInit_0 = 3.9997; 
const double SLAMposInit_1 = 77.71; 
const double SLAMposInit_2 = 0.0; 


const double MapParam[16] = {s, roll, pitch, yaw, sA_00, sA_01, sA_02, sA_10, sA_11, sA_12, sA_20, sA_21, sA_22, SLAMposInit_0, SLAMposInit_1, SLAMposInit_2};
*/
//extern const struct0_T *MapParam;

const double s = 4.6857;
//const double sA[9] = {-0.1791, -4.7018, 0.1343, 4.6391, -0.0002, -0.1005,  0.0216 , 0.0277, 0.9994 };  
const double sA[9] = {-0.1791, 4.6391,  0.0216 , -4.7018,  -0.0002, 0.0277,  0.1343, -0.1005, 0.9994 };  
const double roll = 0.0277; 
const double pitch = 4.7510; 
const double yaw = -0.0216; 
const double SLAMposInit[3] = {2.7476, 77.4887, 0.0};

struct0_T Param = {s, {-0.1791, -4.7018, 0.1343, 4.6391, -0.0002, -0.1005,  0.0216 , 0.0277, 0.9994 }, roll, pitch, yaw, {2.7476, 77.4887, 0.0}};

const struct0_T* MapParam = &Param; 


