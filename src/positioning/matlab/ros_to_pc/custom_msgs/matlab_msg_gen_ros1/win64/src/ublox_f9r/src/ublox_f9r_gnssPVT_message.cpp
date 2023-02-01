// Copyright 2019-2021 The MathWorks, Inc.
// Common copy functions for ublox_f9r/gnssPVT
#include "boost/date_time.hpp"
#include "boost/shared_array.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4265)
#pragma warning(disable : 4458)
#pragma warning(disable : 4100)
#pragma warning(disable : 4127)
#pragma warning(disable : 4267)
#pragma warning(disable : 4068)
#pragma warning(disable : 4245)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wshadow"
#endif //_MSC_VER
#include "ros/ros.h"
#include "ublox_f9r/gnssPVT.h"
#include "visibility_control.h"
#include "MATLABROSMsgInterface.hpp"
#include "ROSPubSubTemplates.hpp"
class UBLOX_F9R_EXPORT ublox_f9r_msg_gnssPVT_common : public MATLABROSMsgInterface<ublox_f9r::gnssPVT> {
  public:
    virtual ~ublox_f9r_msg_gnssPVT_common(){}
    virtual void copy_from_struct(ublox_f9r::gnssPVT* msg, const matlab::data::Struct& arr, MultiLibLoader loader); 
    //----------------------------------------------------------------------------
    virtual MDArray_T get_arr(MDFactory_T& factory, const ublox_f9r::gnssPVT* msg, MultiLibLoader loader, size_t size = 1);
};
  void ublox_f9r_msg_gnssPVT_common::copy_from_struct(ublox_f9r::gnssPVT* msg, const matlab::data::Struct& arr,
               MultiLibLoader loader) {
    try {
        //header
        const matlab::data::StructArray header_arr = arr["Header"];
        auto msgClassPtr_header = getCommonObject<std_msgs::Header>("std_msgs_msg_Header_common",loader);
        msgClassPtr_header->copy_from_struct(&msg->header,header_arr[0],loader);
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Header' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Header' is wrong type; expected a struct.");
    }
    try {
        //iTOW
        const matlab::data::TypedArray<uint32_t> iTOW_arr = arr["ITOW"];
        msg->iTOW = iTOW_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'ITOW' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'ITOW' is wrong type; expected a uint32.");
    }
    try {
        //year
        const matlab::data::TypedArray<uint16_t> year_arr = arr["Year"];
        msg->year = year_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Year' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Year' is wrong type; expected a uint16.");
    }
    try {
        //month
        const matlab::data::TypedArray<uint8_t> month_arr = arr["Month"];
        msg->month = month_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Month' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Month' is wrong type; expected a uint8.");
    }
    try {
        //day
        const matlab::data::TypedArray<uint8_t> day_arr = arr["Day"];
        msg->day = day_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Day' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Day' is wrong type; expected a uint8.");
    }
    try {
        //hour
        const matlab::data::TypedArray<uint8_t> hour_arr = arr["Hour"];
        msg->hour = hour_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Hour' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Hour' is wrong type; expected a uint8.");
    }
    try {
        //min
        const matlab::data::TypedArray<uint8_t> min_arr = arr["Min"];
        msg->min = min_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Min' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Min' is wrong type; expected a uint8.");
    }
    try {
        //sec
        const matlab::data::TypedArray<uint8_t> sec_arr = arr["Sec"];
        msg->sec = sec_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Sec' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Sec' is wrong type; expected a uint8.");
    }
    try {
        //valid
        const matlab::data::TypedArray<uint8_t> valid_arr = arr["Valid"];
        msg->valid = valid_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Valid' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Valid' is wrong type; expected a uint8.");
    }
    try {
        //tAcc
        const matlab::data::TypedArray<uint32_t> tAcc_arr = arr["TAcc"];
        msg->tAcc = tAcc_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'TAcc' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'TAcc' is wrong type; expected a uint32.");
    }
    try {
        //nano
        const matlab::data::TypedArray<int32_t> nano_arr = arr["Nano"];
        msg->nano = nano_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Nano' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Nano' is wrong type; expected a int32.");
    }
    try {
        //fixType
        const matlab::data::TypedArray<uint8_t> fixType_arr = arr["FixType"];
        msg->fixType = fixType_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'FixType' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'FixType' is wrong type; expected a uint8.");
    }
    try {
        //flags
        const matlab::data::TypedArray<uint8_t> flags_arr = arr["Flags"];
        msg->flags = flags_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Flags' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Flags' is wrong type; expected a uint8.");
    }
    try {
        //flags2
        const matlab::data::TypedArray<uint8_t> flags2_arr = arr["Flags2"];
        msg->flags2 = flags2_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Flags2' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Flags2' is wrong type; expected a uint8.");
    }
    try {
        //numSV
        const matlab::data::TypedArray<uint8_t> numSV_arr = arr["NumSV"];
        msg->numSV = numSV_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'NumSV' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'NumSV' is wrong type; expected a uint8.");
    }
    try {
        //lon
        const matlab::data::TypedArray<int32_t> lon_arr = arr["Lon"];
        msg->lon = lon_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Lon' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Lon' is wrong type; expected a int32.");
    }
    try {
        //lat
        const matlab::data::TypedArray<int32_t> lat_arr = arr["Lat"];
        msg->lat = lat_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Lat' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Lat' is wrong type; expected a int32.");
    }
    try {
        //height
        const matlab::data::TypedArray<int32_t> height_arr = arr["Height"];
        msg->height = height_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Height' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Height' is wrong type; expected a int32.");
    }
    try {
        //hMSL
        const matlab::data::TypedArray<int32_t> hMSL_arr = arr["HMSL"];
        msg->hMSL = hMSL_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'HMSL' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'HMSL' is wrong type; expected a int32.");
    }
    try {
        //hAcc
        const matlab::data::TypedArray<uint32_t> hAcc_arr = arr["HAcc"];
        msg->hAcc = hAcc_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'HAcc' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'HAcc' is wrong type; expected a uint32.");
    }
    try {
        //vAcc
        const matlab::data::TypedArray<uint32_t> vAcc_arr = arr["VAcc"];
        msg->vAcc = vAcc_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'VAcc' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'VAcc' is wrong type; expected a uint32.");
    }
    try {
        //velN
        const matlab::data::TypedArray<int32_t> velN_arr = arr["VelN"];
        msg->velN = velN_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'VelN' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'VelN' is wrong type; expected a int32.");
    }
    try {
        //velE
        const matlab::data::TypedArray<int32_t> velE_arr = arr["VelE"];
        msg->velE = velE_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'VelE' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'VelE' is wrong type; expected a int32.");
    }
    try {
        //velD
        const matlab::data::TypedArray<int32_t> velD_arr = arr["VelD"];
        msg->velD = velD_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'VelD' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'VelD' is wrong type; expected a int32.");
    }
    try {
        //gSpeed
        const matlab::data::TypedArray<int32_t> gSpeed_arr = arr["GSpeed"];
        msg->gSpeed = gSpeed_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'GSpeed' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'GSpeed' is wrong type; expected a int32.");
    }
    try {
        //heading
        const matlab::data::TypedArray<int32_t> heading_arr = arr["Heading"];
        msg->heading = heading_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Heading' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Heading' is wrong type; expected a int32.");
    }
    try {
        //sAcc
        const matlab::data::TypedArray<uint32_t> sAcc_arr = arr["SAcc"];
        msg->sAcc = sAcc_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'SAcc' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'SAcc' is wrong type; expected a uint32.");
    }
    try {
        //headAcc
        const matlab::data::TypedArray<uint32_t> headAcc_arr = arr["HeadAcc"];
        msg->headAcc = headAcc_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'HeadAcc' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'HeadAcc' is wrong type; expected a uint32.");
    }
    try {
        //pDOP
        const matlab::data::TypedArray<uint16_t> pDOP_arr = arr["PDOP"];
        msg->pDOP = pDOP_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'PDOP' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'PDOP' is wrong type; expected a uint16.");
    }
    try {
        //reserved1
        const matlab::data::TypedArray<uint8_t> reserved1_arr = arr["Reserved1"];
        size_t nelem = 6;
        	std::copy(reserved1_arr.begin(), reserved1_arr.begin()+nelem, msg->reserved1.begin());
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Reserved1' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Reserved1' is wrong type; expected a uint8.");
    }
    try {
        //headVeh
        const matlab::data::TypedArray<int32_t> headVeh_arr = arr["HeadVeh"];
        msg->headVeh = headVeh_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'HeadVeh' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'HeadVeh' is wrong type; expected a int32.");
    }
    try {
        //magDec
        const matlab::data::TypedArray<int16_t> magDec_arr = arr["MagDec"];
        msg->magDec = magDec_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'MagDec' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'MagDec' is wrong type; expected a int16.");
    }
    try {
        //magAcc
        const matlab::data::TypedArray<uint16_t> magAcc_arr = arr["MagAcc"];
        msg->magAcc = magAcc_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'MagAcc' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'MagAcc' is wrong type; expected a uint16.");
    }
  }
  //----------------------------------------------------------------------------
  MDArray_T ublox_f9r_msg_gnssPVT_common::get_arr(MDFactory_T& factory, const ublox_f9r::gnssPVT* msg,
       MultiLibLoader loader, size_t size) {
    auto outArray = factory.createStructArray({size,1},{"MessageType","Header","CLASSID","MESSAGEID","ITOW","Year","Month","Day","Hour","Min","Sec","Valid","VALIDDATE","VALIDTIME","VALIDFULLYRESOLVED","VALIDMAG","TAcc","Nano","FixType","FIXTYPENOFIX","FIXTYPEDEADRECKONINGONLY","FIXTYPE2D","FIXTYPE3D","FIXTYPEGNSSDEADRECKONINGCOMBINED","FIXTYPETIMEONLY","Flags","FLAGSGNSSFIXOK","FLAGSDIFFSOLN","FLAGSPSMMASK","PSMOFF","PSMENABLED","PSMACQUIRED","PSMTRACKING","PSMPOWEROPTIMIZEDTRACKING","PSMINACTIVE","FLAGSHEADVEHVALID","FLAGSCARRIERPHASEMASK","CARRIERPHASENOSOLUTION","CARRIERPHASEFLOAT","CARRIERPHASEFIXED","Flags2","FLAGS2CONFIRMEDAVAILABLE","FLAGS2CONFIRMEDDATE","FLAGS2CONFIRMEDTIME","NumSV","Lon","Lat","Height","HMSL","HAcc","VAcc","VelN","VelE","VelD","GSpeed","Heading","SAcc","HeadAcc","PDOP","Reserved1","HeadVeh","MagDec","MagAcc"});
    for(size_t ctr = 0; ctr < size; ctr++){
    outArray[ctr]["MessageType"] = factory.createCharArray("ublox_f9r/gnssPVT");
    // header
    auto currentElement_header = (msg + ctr)->header;
    auto msgClassPtr_header = getCommonObject<std_msgs::Header>("std_msgs_msg_Header_common",loader);
    outArray[ctr]["Header"] = msgClassPtr_header->get_arr(factory, &currentElement_header, loader);
    // CLASS_ID
    auto currentElement_CLASS_ID = (msg + ctr)->CLASS_ID;
    outArray[ctr]["CLASSID"] = factory.createScalar(static_cast<uint8_t>(currentElement_CLASS_ID));
    // MESSAGE_ID
    auto currentElement_MESSAGE_ID = (msg + ctr)->MESSAGE_ID;
    outArray[ctr]["MESSAGEID"] = factory.createScalar(static_cast<uint8_t>(currentElement_MESSAGE_ID));
    // iTOW
    auto currentElement_iTOW = (msg + ctr)->iTOW;
    outArray[ctr]["ITOW"] = factory.createScalar(currentElement_iTOW);
    // year
    auto currentElement_year = (msg + ctr)->year;
    outArray[ctr]["Year"] = factory.createScalar(currentElement_year);
    // month
    auto currentElement_month = (msg + ctr)->month;
    outArray[ctr]["Month"] = factory.createScalar(currentElement_month);
    // day
    auto currentElement_day = (msg + ctr)->day;
    outArray[ctr]["Day"] = factory.createScalar(currentElement_day);
    // hour
    auto currentElement_hour = (msg + ctr)->hour;
    outArray[ctr]["Hour"] = factory.createScalar(currentElement_hour);
    // min
    auto currentElement_min = (msg + ctr)->min;
    outArray[ctr]["Min"] = factory.createScalar(currentElement_min);
    // sec
    auto currentElement_sec = (msg + ctr)->sec;
    outArray[ctr]["Sec"] = factory.createScalar(currentElement_sec);
    // valid
    auto currentElement_valid = (msg + ctr)->valid;
    outArray[ctr]["Valid"] = factory.createScalar(currentElement_valid);
    // VALID_DATE
    auto currentElement_VALID_DATE = (msg + ctr)->VALID_DATE;
    outArray[ctr]["VALIDDATE"] = factory.createScalar(static_cast<uint8_t>(currentElement_VALID_DATE));
    // VALID_TIME
    auto currentElement_VALID_TIME = (msg + ctr)->VALID_TIME;
    outArray[ctr]["VALIDTIME"] = factory.createScalar(static_cast<uint8_t>(currentElement_VALID_TIME));
    // VALID_FULLY_RESOLVED
    auto currentElement_VALID_FULLY_RESOLVED = (msg + ctr)->VALID_FULLY_RESOLVED;
    outArray[ctr]["VALIDFULLYRESOLVED"] = factory.createScalar(static_cast<uint8_t>(currentElement_VALID_FULLY_RESOLVED));
    // VALID_MAG
    auto currentElement_VALID_MAG = (msg + ctr)->VALID_MAG;
    outArray[ctr]["VALIDMAG"] = factory.createScalar(static_cast<uint8_t>(currentElement_VALID_MAG));
    // tAcc
    auto currentElement_tAcc = (msg + ctr)->tAcc;
    outArray[ctr]["TAcc"] = factory.createScalar(currentElement_tAcc);
    // nano
    auto currentElement_nano = (msg + ctr)->nano;
    outArray[ctr]["Nano"] = factory.createScalar(currentElement_nano);
    // fixType
    auto currentElement_fixType = (msg + ctr)->fixType;
    outArray[ctr]["FixType"] = factory.createScalar(currentElement_fixType);
    // FIX_TYPE_NO_FIX
    auto currentElement_FIX_TYPE_NO_FIX = (msg + ctr)->FIX_TYPE_NO_FIX;
    outArray[ctr]["FIXTYPENOFIX"] = factory.createScalar(static_cast<uint8_t>(currentElement_FIX_TYPE_NO_FIX));
    // FIX_TYPE_DEAD_RECKONING_ONLY
    auto currentElement_FIX_TYPE_DEAD_RECKONING_ONLY = (msg + ctr)->FIX_TYPE_DEAD_RECKONING_ONLY;
    outArray[ctr]["FIXTYPEDEADRECKONINGONLY"] = factory.createScalar(static_cast<uint8_t>(currentElement_FIX_TYPE_DEAD_RECKONING_ONLY));
    // FIX_TYPE_2D
    auto currentElement_FIX_TYPE_2D = (msg + ctr)->FIX_TYPE_2D;
    outArray[ctr]["FIXTYPE2D"] = factory.createScalar(static_cast<uint8_t>(currentElement_FIX_TYPE_2D));
    // FIX_TYPE_3D
    auto currentElement_FIX_TYPE_3D = (msg + ctr)->FIX_TYPE_3D;
    outArray[ctr]["FIXTYPE3D"] = factory.createScalar(static_cast<uint8_t>(currentElement_FIX_TYPE_3D));
    // FIX_TYPE_GNSS_DEAD_RECKONING_COMBINED
    auto currentElement_FIX_TYPE_GNSS_DEAD_RECKONING_COMBINED = (msg + ctr)->FIX_TYPE_GNSS_DEAD_RECKONING_COMBINED;
    outArray[ctr]["FIXTYPEGNSSDEADRECKONINGCOMBINED"] = factory.createScalar(static_cast<uint8_t>(currentElement_FIX_TYPE_GNSS_DEAD_RECKONING_COMBINED));
    // FIX_TYPE_TIME_ONLY
    auto currentElement_FIX_TYPE_TIME_ONLY = (msg + ctr)->FIX_TYPE_TIME_ONLY;
    outArray[ctr]["FIXTYPETIMEONLY"] = factory.createScalar(static_cast<uint8_t>(currentElement_FIX_TYPE_TIME_ONLY));
    // flags
    auto currentElement_flags = (msg + ctr)->flags;
    outArray[ctr]["Flags"] = factory.createScalar(currentElement_flags);
    // FLAGS_GNSS_FIX_OK
    auto currentElement_FLAGS_GNSS_FIX_OK = (msg + ctr)->FLAGS_GNSS_FIX_OK;
    outArray[ctr]["FLAGSGNSSFIXOK"] = factory.createScalar(static_cast<uint8_t>(currentElement_FLAGS_GNSS_FIX_OK));
    // FLAGS_DIFF_SOLN
    auto currentElement_FLAGS_DIFF_SOLN = (msg + ctr)->FLAGS_DIFF_SOLN;
    outArray[ctr]["FLAGSDIFFSOLN"] = factory.createScalar(static_cast<uint8_t>(currentElement_FLAGS_DIFF_SOLN));
    // FLAGS_PSM_MASK
    auto currentElement_FLAGS_PSM_MASK = (msg + ctr)->FLAGS_PSM_MASK;
    outArray[ctr]["FLAGSPSMMASK"] = factory.createScalar(static_cast<uint8_t>(currentElement_FLAGS_PSM_MASK));
    // PSM_OFF
    auto currentElement_PSM_OFF = (msg + ctr)->PSM_OFF;
    outArray[ctr]["PSMOFF"] = factory.createScalar(static_cast<uint8_t>(currentElement_PSM_OFF));
    // PSM_ENABLED
    auto currentElement_PSM_ENABLED = (msg + ctr)->PSM_ENABLED;
    outArray[ctr]["PSMENABLED"] = factory.createScalar(static_cast<uint8_t>(currentElement_PSM_ENABLED));
    // PSM_ACQUIRED
    auto currentElement_PSM_ACQUIRED = (msg + ctr)->PSM_ACQUIRED;
    outArray[ctr]["PSMACQUIRED"] = factory.createScalar(static_cast<uint8_t>(currentElement_PSM_ACQUIRED));
    // PSM_TRACKING
    auto currentElement_PSM_TRACKING = (msg + ctr)->PSM_TRACKING;
    outArray[ctr]["PSMTRACKING"] = factory.createScalar(static_cast<uint8_t>(currentElement_PSM_TRACKING));
    // PSM_POWER_OPTIMIZED_TRACKING
    auto currentElement_PSM_POWER_OPTIMIZED_TRACKING = (msg + ctr)->PSM_POWER_OPTIMIZED_TRACKING;
    outArray[ctr]["PSMPOWEROPTIMIZEDTRACKING"] = factory.createScalar(static_cast<uint8_t>(currentElement_PSM_POWER_OPTIMIZED_TRACKING));
    // PSM_INACTIVE
    auto currentElement_PSM_INACTIVE = (msg + ctr)->PSM_INACTIVE;
    outArray[ctr]["PSMINACTIVE"] = factory.createScalar(static_cast<uint8_t>(currentElement_PSM_INACTIVE));
    // FLAGS_HEAD_VEH_VALID
    auto currentElement_FLAGS_HEAD_VEH_VALID = (msg + ctr)->FLAGS_HEAD_VEH_VALID;
    outArray[ctr]["FLAGSHEADVEHVALID"] = factory.createScalar(static_cast<uint8_t>(currentElement_FLAGS_HEAD_VEH_VALID));
    // FLAGS_CARRIER_PHASE_MASK
    auto currentElement_FLAGS_CARRIER_PHASE_MASK = (msg + ctr)->FLAGS_CARRIER_PHASE_MASK;
    outArray[ctr]["FLAGSCARRIERPHASEMASK"] = factory.createScalar(static_cast<uint8_t>(currentElement_FLAGS_CARRIER_PHASE_MASK));
    // CARRIER_PHASE_NO_SOLUTION
    auto currentElement_CARRIER_PHASE_NO_SOLUTION = (msg + ctr)->CARRIER_PHASE_NO_SOLUTION;
    outArray[ctr]["CARRIERPHASENOSOLUTION"] = factory.createScalar(static_cast<uint8_t>(currentElement_CARRIER_PHASE_NO_SOLUTION));
    // CARRIER_PHASE_FLOAT
    auto currentElement_CARRIER_PHASE_FLOAT = (msg + ctr)->CARRIER_PHASE_FLOAT;
    outArray[ctr]["CARRIERPHASEFLOAT"] = factory.createScalar(static_cast<uint8_t>(currentElement_CARRIER_PHASE_FLOAT));
    // CARRIER_PHASE_FIXED
    auto currentElement_CARRIER_PHASE_FIXED = (msg + ctr)->CARRIER_PHASE_FIXED;
    outArray[ctr]["CARRIERPHASEFIXED"] = factory.createScalar(static_cast<uint8_t>(currentElement_CARRIER_PHASE_FIXED));
    // flags2
    auto currentElement_flags2 = (msg + ctr)->flags2;
    outArray[ctr]["Flags2"] = factory.createScalar(currentElement_flags2);
    // FLAGS2_CONFIRMED_AVAILABLE
    auto currentElement_FLAGS2_CONFIRMED_AVAILABLE = (msg + ctr)->FLAGS2_CONFIRMED_AVAILABLE;
    outArray[ctr]["FLAGS2CONFIRMEDAVAILABLE"] = factory.createScalar(static_cast<uint8_t>(currentElement_FLAGS2_CONFIRMED_AVAILABLE));
    // FLAGS2_CONFIRMED_DATE
    auto currentElement_FLAGS2_CONFIRMED_DATE = (msg + ctr)->FLAGS2_CONFIRMED_DATE;
    outArray[ctr]["FLAGS2CONFIRMEDDATE"] = factory.createScalar(static_cast<uint8_t>(currentElement_FLAGS2_CONFIRMED_DATE));
    // FLAGS2_CONFIRMED_TIME
    auto currentElement_FLAGS2_CONFIRMED_TIME = (msg + ctr)->FLAGS2_CONFIRMED_TIME;
    outArray[ctr]["FLAGS2CONFIRMEDTIME"] = factory.createScalar(static_cast<uint8_t>(currentElement_FLAGS2_CONFIRMED_TIME));
    // numSV
    auto currentElement_numSV = (msg + ctr)->numSV;
    outArray[ctr]["NumSV"] = factory.createScalar(currentElement_numSV);
    // lon
    auto currentElement_lon = (msg + ctr)->lon;
    outArray[ctr]["Lon"] = factory.createScalar(currentElement_lon);
    // lat
    auto currentElement_lat = (msg + ctr)->lat;
    outArray[ctr]["Lat"] = factory.createScalar(currentElement_lat);
    // height
    auto currentElement_height = (msg + ctr)->height;
    outArray[ctr]["Height"] = factory.createScalar(currentElement_height);
    // hMSL
    auto currentElement_hMSL = (msg + ctr)->hMSL;
    outArray[ctr]["HMSL"] = factory.createScalar(currentElement_hMSL);
    // hAcc
    auto currentElement_hAcc = (msg + ctr)->hAcc;
    outArray[ctr]["HAcc"] = factory.createScalar(currentElement_hAcc);
    // vAcc
    auto currentElement_vAcc = (msg + ctr)->vAcc;
    outArray[ctr]["VAcc"] = factory.createScalar(currentElement_vAcc);
    // velN
    auto currentElement_velN = (msg + ctr)->velN;
    outArray[ctr]["VelN"] = factory.createScalar(currentElement_velN);
    // velE
    auto currentElement_velE = (msg + ctr)->velE;
    outArray[ctr]["VelE"] = factory.createScalar(currentElement_velE);
    // velD
    auto currentElement_velD = (msg + ctr)->velD;
    outArray[ctr]["VelD"] = factory.createScalar(currentElement_velD);
    // gSpeed
    auto currentElement_gSpeed = (msg + ctr)->gSpeed;
    outArray[ctr]["GSpeed"] = factory.createScalar(currentElement_gSpeed);
    // heading
    auto currentElement_heading = (msg + ctr)->heading;
    outArray[ctr]["Heading"] = factory.createScalar(currentElement_heading);
    // sAcc
    auto currentElement_sAcc = (msg + ctr)->sAcc;
    outArray[ctr]["SAcc"] = factory.createScalar(currentElement_sAcc);
    // headAcc
    auto currentElement_headAcc = (msg + ctr)->headAcc;
    outArray[ctr]["HeadAcc"] = factory.createScalar(currentElement_headAcc);
    // pDOP
    auto currentElement_pDOP = (msg + ctr)->pDOP;
    outArray[ctr]["PDOP"] = factory.createScalar(currentElement_pDOP);
    // reserved1
    auto currentElement_reserved1 = (msg + ctr)->reserved1;
    outArray[ctr]["Reserved1"] = factory.createArray<ublox_f9r::gnssPVT::_reserved1_type::const_iterator, uint8_t>({currentElement_reserved1.size(),1}, currentElement_reserved1.begin(), currentElement_reserved1.end());
    // headVeh
    auto currentElement_headVeh = (msg + ctr)->headVeh;
    outArray[ctr]["HeadVeh"] = factory.createScalar(currentElement_headVeh);
    // magDec
    auto currentElement_magDec = (msg + ctr)->magDec;
    outArray[ctr]["MagDec"] = factory.createScalar(currentElement_magDec);
    // magAcc
    auto currentElement_magAcc = (msg + ctr)->magAcc;
    outArray[ctr]["MagAcc"] = factory.createScalar(currentElement_magAcc);
    }
    return std::move(outArray);
  } 
class UBLOX_F9R_EXPORT ublox_f9r_gnssPVT_message : public ROSMsgElementInterfaceFactory {
  public:
    virtual ~ublox_f9r_gnssPVT_message(){}
    virtual std::shared_ptr<MATLABPublisherInterface> generatePublisherInterface(ElementType type);
    virtual std::shared_ptr<MATLABSubscriberInterface> generateSubscriberInterface(ElementType type);
    virtual std::shared_ptr<MATLABRosbagWriterInterface> generateRosbagWriterInterface(ElementType type);
};  
  std::shared_ptr<MATLABPublisherInterface> 
          ublox_f9r_gnssPVT_message::generatePublisherInterface(ElementType type){
    if(type != eMessage){
        throw std::invalid_argument("Wrong input, Expected eMessage");
    }
    return std::make_shared<ROSPublisherImpl<ublox_f9r::gnssPVT,ublox_f9r_msg_gnssPVT_common>>();
  }
  std::shared_ptr<MATLABSubscriberInterface> 
         ublox_f9r_gnssPVT_message::generateSubscriberInterface(ElementType type){
    if(type != eMessage){
        throw std::invalid_argument("Wrong input, Expected eMessage");
    }
    return std::make_shared<ROSSubscriberImpl<ublox_f9r::gnssPVT,ublox_f9r::gnssPVT::ConstPtr,ublox_f9r_msg_gnssPVT_common>>();
  }
#include "ROSbagTemplates.hpp" 
  std::shared_ptr<MATLABRosbagWriterInterface>
         ublox_f9r_gnssPVT_message::generateRosbagWriterInterface(ElementType type){
    if(type != eMessage){
        throw std::invalid_argument("Wrong input, Expected eMessage");
    }
    return std::make_shared<ROSBagWriterImpl<ublox_f9r::gnssPVT,ublox_f9r_msg_gnssPVT_common>>();
  }
#include "register_macro.hpp"
// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
CLASS_LOADER_REGISTER_CLASS(ublox_f9r_msg_gnssPVT_common, MATLABROSMsgInterface<ublox_f9r::gnssPVT>)
CLASS_LOADER_REGISTER_CLASS(ublox_f9r_gnssPVT_message, ROSMsgElementInterfaceFactory)
#ifdef _MSC_VER
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif //_MSC_VER
//gen-1