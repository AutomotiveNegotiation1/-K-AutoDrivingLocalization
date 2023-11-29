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

class Imupos {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.b_acc_o = null;
      this.b_gyro = null;
      this.mode = null;
      this.kf_psi = null;
      this.gyro_psi = null;
      this.cent_pos_est = null;
      this.cent_vel_est = null;
      this.state_o = null;
      this.acc_b_phi = null;
      this.acc_b_theta = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('b_acc_o')) {
        this.b_acc_o = initObj.b_acc_o
      }
      else {
        this.b_acc_o = [];
      }
      if (initObj.hasOwnProperty('b_gyro')) {
        this.b_gyro = initObj.b_gyro
      }
      else {
        this.b_gyro = [];
      }
      if (initObj.hasOwnProperty('mode')) {
        this.mode = initObj.mode
      }
      else {
        this.mode = 0.0;
      }
      if (initObj.hasOwnProperty('kf_psi')) {
        this.kf_psi = initObj.kf_psi
      }
      else {
        this.kf_psi = 0.0;
      }
      if (initObj.hasOwnProperty('gyro_psi')) {
        this.gyro_psi = initObj.gyro_psi
      }
      else {
        this.gyro_psi = 0.0;
      }
      if (initObj.hasOwnProperty('cent_pos_est')) {
        this.cent_pos_est = initObj.cent_pos_est
      }
      else {
        this.cent_pos_est = [];
      }
      if (initObj.hasOwnProperty('cent_vel_est')) {
        this.cent_vel_est = initObj.cent_vel_est
      }
      else {
        this.cent_vel_est = [];
      }
      if (initObj.hasOwnProperty('state_o')) {
        this.state_o = initObj.state_o
      }
      else {
        this.state_o = 0.0;
      }
      if (initObj.hasOwnProperty('acc_b_phi')) {
        this.acc_b_phi = initObj.acc_b_phi
      }
      else {
        this.acc_b_phi = 0.0;
      }
      if (initObj.hasOwnProperty('acc_b_theta')) {
        this.acc_b_theta = initObj.acc_b_theta
      }
      else {
        this.acc_b_theta = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type Imupos
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [b_acc_o]
    bufferOffset = _arraySerializer.float64(obj.b_acc_o, buffer, bufferOffset, null);
    // Serialize message field [b_gyro]
    bufferOffset = _arraySerializer.float64(obj.b_gyro, buffer, bufferOffset, null);
    // Serialize message field [mode]
    bufferOffset = _serializer.float64(obj.mode, buffer, bufferOffset);
    // Serialize message field [kf_psi]
    bufferOffset = _serializer.float64(obj.kf_psi, buffer, bufferOffset);
    // Serialize message field [gyro_psi]
    bufferOffset = _serializer.float64(obj.gyro_psi, buffer, bufferOffset);
    // Serialize message field [cent_pos_est]
    bufferOffset = _arraySerializer.float64(obj.cent_pos_est, buffer, bufferOffset, null);
    // Serialize message field [cent_vel_est]
    bufferOffset = _arraySerializer.float64(obj.cent_vel_est, buffer, bufferOffset, null);
    // Serialize message field [state_o]
    bufferOffset = _serializer.float64(obj.state_o, buffer, bufferOffset);
    // Serialize message field [acc_b_phi]
    bufferOffset = _serializer.float64(obj.acc_b_phi, buffer, bufferOffset);
    // Serialize message field [acc_b_theta]
    bufferOffset = _serializer.float64(obj.acc_b_theta, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type Imupos
    let len;
    let data = new Imupos(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [b_acc_o]
    data.b_acc_o = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [b_gyro]
    data.b_gyro = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [mode]
    data.mode = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [kf_psi]
    data.kf_psi = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [gyro_psi]
    data.gyro_psi = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [cent_pos_est]
    data.cent_pos_est = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [cent_vel_est]
    data.cent_vel_est = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [state_o]
    data.state_o = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [acc_b_phi]
    data.acc_b_phi = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [acc_b_theta]
    data.acc_b_theta = _deserializer.float64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    length += 8 * object.b_acc_o.length;
    length += 8 * object.b_gyro.length;
    length += 8 * object.cent_pos_est.length;
    length += 8 * object.cent_vel_est.length;
    return length + 64;
  }

  static datatype() {
    // Returns string type for a message object
    return 'ipe/Imupos';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'b7b3c908ac554cd386d59718bc4de7e4';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    std_msgs/Header header
    float64[] b_acc_o
    float64[] b_gyro
    float64 mode
    float64 kf_psi
    float64 gyro_psi
    float64[] cent_pos_est
    float64[] cent_vel_est
    float64 state_o
    float64 acc_b_phi
    float64 acc_b_theta
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
    const resolved = new Imupos(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.b_acc_o !== undefined) {
      resolved.b_acc_o = msg.b_acc_o;
    }
    else {
      resolved.b_acc_o = []
    }

    if (msg.b_gyro !== undefined) {
      resolved.b_gyro = msg.b_gyro;
    }
    else {
      resolved.b_gyro = []
    }

    if (msg.mode !== undefined) {
      resolved.mode = msg.mode;
    }
    else {
      resolved.mode = 0.0
    }

    if (msg.kf_psi !== undefined) {
      resolved.kf_psi = msg.kf_psi;
    }
    else {
      resolved.kf_psi = 0.0
    }

    if (msg.gyro_psi !== undefined) {
      resolved.gyro_psi = msg.gyro_psi;
    }
    else {
      resolved.gyro_psi = 0.0
    }

    if (msg.cent_pos_est !== undefined) {
      resolved.cent_pos_est = msg.cent_pos_est;
    }
    else {
      resolved.cent_pos_est = []
    }

    if (msg.cent_vel_est !== undefined) {
      resolved.cent_vel_est = msg.cent_vel_est;
    }
    else {
      resolved.cent_vel_est = []
    }

    if (msg.state_o !== undefined) {
      resolved.state_o = msg.state_o;
    }
    else {
      resolved.state_o = 0.0
    }

    if (msg.acc_b_phi !== undefined) {
      resolved.acc_b_phi = msg.acc_b_phi;
    }
    else {
      resolved.acc_b_phi = 0.0
    }

    if (msg.acc_b_theta !== undefined) {
      resolved.acc_b_theta = msg.acc_b_theta;
    }
    else {
      resolved.acc_b_theta = 0.0
    }

    return resolved;
    }
};

module.exports = Imupos;
