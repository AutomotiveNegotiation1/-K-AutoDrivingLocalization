// Auto-generated. Do not edit!

// (in-package ipe.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class Uwbpos {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.tag_center_vel_est = null;
      this.tag_pos_est = null;
      this.tag_pos_b = null;
      this.UWBErrSum = null;
      this.heading_est = null;
      this.Nanchor = null;
      this.init_flag = null;
      this.zt_b = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('tag_center_vel_est')) {
        this.tag_center_vel_est = initObj.tag_center_vel_est
      }
      else {
        this.tag_center_vel_est = [];
      }
      if (initObj.hasOwnProperty('tag_pos_est')) {
        this.tag_pos_est = initObj.tag_pos_est
      }
      else {
        this.tag_pos_est = [];
      }
      if (initObj.hasOwnProperty('tag_pos_b')) {
        this.tag_pos_b = initObj.tag_pos_b
      }
      else {
        this.tag_pos_b = [];
      }
      if (initObj.hasOwnProperty('UWBErrSum')) {
        this.UWBErrSum = initObj.UWBErrSum
      }
      else {
        this.UWBErrSum = 0.0;
      }
      if (initObj.hasOwnProperty('heading_est')) {
        this.heading_est = initObj.heading_est
      }
      else {
        this.heading_est = 0.0;
      }
      if (initObj.hasOwnProperty('Nanchor')) {
        this.Nanchor = initObj.Nanchor
      }
      else {
        this.Nanchor = 0.0;
      }
      if (initObj.hasOwnProperty('init_flag')) {
        this.init_flag = initObj.init_flag
      }
      else {
        this.init_flag = 0.0;
      }
      if (initObj.hasOwnProperty('zt_b')) {
        this.zt_b = initObj.zt_b
      }
      else {
        this.zt_b = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type Uwbpos
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [tag_center_vel_est]
    bufferOffset = _arraySerializer.float64(obj.tag_center_vel_est, buffer, bufferOffset, null);
    // Serialize message field [tag_pos_est]
    bufferOffset = _arraySerializer.float64(obj.tag_pos_est, buffer, bufferOffset, null);
    // Serialize message field [tag_pos_b]
    bufferOffset = _arraySerializer.float64(obj.tag_pos_b, buffer, bufferOffset, null);
    // Serialize message field [UWBErrSum]
    bufferOffset = _serializer.float64(obj.UWBErrSum, buffer, bufferOffset);
    // Serialize message field [heading_est]
    bufferOffset = _serializer.float64(obj.heading_est, buffer, bufferOffset);
    // Serialize message field [Nanchor]
    bufferOffset = _serializer.float64(obj.Nanchor, buffer, bufferOffset);
    // Serialize message field [init_flag]
    bufferOffset = _serializer.float64(obj.init_flag, buffer, bufferOffset);
    // Serialize message field [zt_b]
    bufferOffset = _serializer.float64(obj.zt_b, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type Uwbpos
    let len;
    let data = new Uwbpos(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [tag_center_vel_est]
    data.tag_center_vel_est = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [tag_pos_est]
    data.tag_pos_est = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [tag_pos_b]
    data.tag_pos_b = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [UWBErrSum]
    data.UWBErrSum = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [heading_est]
    data.heading_est = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [Nanchor]
    data.Nanchor = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [init_flag]
    data.init_flag = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [zt_b]
    data.zt_b = _deserializer.float64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    length += 8 * object.tag_center_vel_est.length;
    length += 8 * object.tag_pos_est.length;
    length += 8 * object.tag_pos_b.length;
    return length + 52;
  }

  static datatype() {
    // Returns string type for a message object
    return 'ipe/Uwbpos';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'b5e368833065ec31ffd718a3d27ee9f6';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    std_msgs/Header header
    float64[] tag_center_vel_est
    float64[] tag_pos_est
    float64[] tag_pos_b
    float64 UWBErrSum
    float64 heading_est
    float64 Nanchor
    float64 init_flag
    float64 zt_b
    ================================================================================
    MSG: std_msgs/Header
    # Standard metadata for higher-level stamped data types.
    # This is generally used to communicate timestamped data 
    # in a particular coordinate frame.
    # 
    # sequence ID: consecutively increasing ID 
    uint32 seq
    #Two-integer timestamp that is expressed as:
    # * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
    # * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
    # time-handling sugar is provided by the client library
    time stamp
    #Frame this data is associated with
    string frame_id
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new Uwbpos(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.tag_center_vel_est !== undefined) {
      resolved.tag_center_vel_est = msg.tag_center_vel_est;
    }
    else {
      resolved.tag_center_vel_est = []
    }

    if (msg.tag_pos_est !== undefined) {
      resolved.tag_pos_est = msg.tag_pos_est;
    }
    else {
      resolved.tag_pos_est = []
    }

    if (msg.tag_pos_b !== undefined) {
      resolved.tag_pos_b = msg.tag_pos_b;
    }
    else {
      resolved.tag_pos_b = []
    }

    if (msg.UWBErrSum !== undefined) {
      resolved.UWBErrSum = msg.UWBErrSum;
    }
    else {
      resolved.UWBErrSum = 0.0
    }

    if (msg.heading_est !== undefined) {
      resolved.heading_est = msg.heading_est;
    }
    else {
      resolved.heading_est = 0.0
    }

    if (msg.Nanchor !== undefined) {
      resolved.Nanchor = msg.Nanchor;
    }
    else {
      resolved.Nanchor = 0.0
    }

    if (msg.init_flag !== undefined) {
      resolved.init_flag = msg.init_flag;
    }
    else {
      resolved.init_flag = 0.0
    }

    if (msg.zt_b !== undefined) {
      resolved.zt_b = msg.zt_b;
    }
    else {
      resolved.zt_b = 0.0
    }

    return resolved;
    }
};

module.exports = Uwbpos;
