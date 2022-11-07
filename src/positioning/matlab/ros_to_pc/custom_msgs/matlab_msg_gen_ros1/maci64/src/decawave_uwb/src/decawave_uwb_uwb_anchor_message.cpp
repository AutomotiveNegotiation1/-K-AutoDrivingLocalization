// Copyright 2019-2021 The MathWorks, Inc.
// Common copy functions for decawave_uwb/uwb_anchor
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
#include "decawave_uwb/uwb_anchor.h"
#include "visibility_control.h"
#include "MATLABROSMsgInterface.hpp"
#include "ROSPubSubTemplates.hpp"
class DECAWAVE_UWB_EXPORT decawave_uwb_msg_uwb_anchor_common : public MATLABROSMsgInterface<decawave_uwb::uwb_anchor> {
  public:
    virtual ~decawave_uwb_msg_uwb_anchor_common(){}
    virtual void copy_from_struct(decawave_uwb::uwb_anchor* msg, const matlab::data::Struct& arr, MultiLibLoader loader); 
    //----------------------------------------------------------------------------
    virtual MDArray_T get_arr(MDFactory_T& factory, const decawave_uwb::uwb_anchor* msg, MultiLibLoader loader, size_t size = 1);
};
  void decawave_uwb_msg_uwb_anchor_common::copy_from_struct(decawave_uwb::uwb_anchor* msg, const matlab::data::Struct& arr,
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
        //AnchorID
        const matlab::data::TypedArray<uint32_t> AnchorID_arr = arr["AnchorID"];
        size_t nelem = AnchorID_arr.getNumberOfElements();
        	msg->AnchorID.resize(nelem);
        	std::copy(AnchorID_arr.begin(), AnchorID_arr.begin()+nelem, msg->AnchorID.begin());
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'AnchorID' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'AnchorID' is wrong type; expected a uint32.");
    }
    try {
        //ActiveNum
        const matlab::data::TypedArray<uint8_t> ActiveNum_arr = arr["ActiveNum"];
        msg->ActiveNum = ActiveNum_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'ActiveNum' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'ActiveNum' is wrong type; expected a uint8.");
    }
    try {
        //Range
        const matlab::data::TypedArray<double> Range_arr = arr["Range"];
        size_t nelem = Range_arr.getNumberOfElements();
        	msg->Range.resize(nelem);
        	std::copy(Range_arr.begin(), Range_arr.begin()+nelem, msg->Range.begin());
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Range' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Range' is wrong type; expected a double.");
    }
    try {
        //Pos
        const matlab::data::StructArray Pos_arr = arr["Pos"];
        auto msgClassPtr_Pos = getCommonObject<geometry_msgs::Pose>("geometry_msgs_msg_Pose_common",loader);
        msgClassPtr_Pos->copy_from_struct(&msg->Pos,Pos_arr[0],loader);
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'Pos' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'Pos' is wrong type; expected a struct.");
    }
  }
  //----------------------------------------------------------------------------
  MDArray_T decawave_uwb_msg_uwb_anchor_common::get_arr(MDFactory_T& factory, const decawave_uwb::uwb_anchor* msg,
       MultiLibLoader loader, size_t size) {
    auto outArray = factory.createStructArray({size,1},{"MessageType","Header","AnchorID","ActiveNum","Range","Pos"});
    for(size_t ctr = 0; ctr < size; ctr++){
    outArray[ctr]["MessageType"] = factory.createCharArray("decawave_uwb/uwb_anchor");
    // header
    auto currentElement_header = (msg + ctr)->header;
    auto msgClassPtr_header = getCommonObject<std_msgs::Header>("std_msgs_msg_Header_common",loader);
    outArray[ctr]["Header"] = msgClassPtr_header->get_arr(factory, &currentElement_header, loader);
    // AnchorID
    auto currentElement_AnchorID = (msg + ctr)->AnchorID;
    outArray[ctr]["AnchorID"] = factory.createArray<decawave_uwb::uwb_anchor::_AnchorID_type::const_iterator, uint32_t>({currentElement_AnchorID.size(),1}, currentElement_AnchorID.begin(), currentElement_AnchorID.end());
    // ActiveNum
    auto currentElement_ActiveNum = (msg + ctr)->ActiveNum;
    outArray[ctr]["ActiveNum"] = factory.createScalar(currentElement_ActiveNum);
    // Range
    auto currentElement_Range = (msg + ctr)->Range;
    outArray[ctr]["Range"] = factory.createArray<decawave_uwb::uwb_anchor::_Range_type::const_iterator, double>({currentElement_Range.size(),1}, currentElement_Range.begin(), currentElement_Range.end());
    // Pos
    auto currentElement_Pos = (msg + ctr)->Pos;
    auto msgClassPtr_Pos = getCommonObject<geometry_msgs::Pose>("geometry_msgs_msg_Pose_common",loader);
    outArray[ctr]["Pos"] = msgClassPtr_Pos->get_arr(factory, &currentElement_Pos, loader);
    }
    return std::move(outArray);
  } 
class DECAWAVE_UWB_EXPORT decawave_uwb_uwb_anchor_message : public ROSMsgElementInterfaceFactory {
  public:
    virtual ~decawave_uwb_uwb_anchor_message(){}
    virtual std::shared_ptr<MATLABPublisherInterface> generatePublisherInterface(ElementType type);
    virtual std::shared_ptr<MATLABSubscriberInterface> generateSubscriberInterface(ElementType type);
    virtual std::shared_ptr<MATLABRosbagWriterInterface> generateRosbagWriterInterface(ElementType type);
};  
  std::shared_ptr<MATLABPublisherInterface> 
          decawave_uwb_uwb_anchor_message::generatePublisherInterface(ElementType type){
    if(type != eMessage){
        throw std::invalid_argument("Wrong input, Expected eMessage");
    }
    return std::make_shared<ROSPublisherImpl<decawave_uwb::uwb_anchor,decawave_uwb_msg_uwb_anchor_common>>();
  }
  std::shared_ptr<MATLABSubscriberInterface> 
         decawave_uwb_uwb_anchor_message::generateSubscriberInterface(ElementType type){
    if(type != eMessage){
        throw std::invalid_argument("Wrong input, Expected eMessage");
    }
    return std::make_shared<ROSSubscriberImpl<decawave_uwb::uwb_anchor,decawave_uwb::uwb_anchor::ConstPtr,decawave_uwb_msg_uwb_anchor_common>>();
  }
#include "ROSbagTemplates.hpp" 
  std::shared_ptr<MATLABRosbagWriterInterface>
         decawave_uwb_uwb_anchor_message::generateRosbagWriterInterface(ElementType type){
    if(type != eMessage){
        throw std::invalid_argument("Wrong input, Expected eMessage");
    }
    return std::make_shared<ROSBagWriterImpl<decawave_uwb::uwb_anchor,decawave_uwb_msg_uwb_anchor_common>>();
  }
#include "register_macro.hpp"
// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
CLASS_LOADER_REGISTER_CLASS(decawave_uwb_msg_uwb_anchor_common, MATLABROSMsgInterface<decawave_uwb::uwb_anchor>)
CLASS_LOADER_REGISTER_CLASS(decawave_uwb_uwb_anchor_message, ROSMsgElementInterfaceFactory)
#ifdef _MSC_VER
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif //_MSC_VER
//gen-1