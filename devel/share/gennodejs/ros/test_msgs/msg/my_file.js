// Auto-generated. Do not edit!

// (in-package test_msgs.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class my_file {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.robot_igite_academy = null;
    }
    else {
      if (initObj.hasOwnProperty('robot_igite_academy')) {
        this.robot_igite_academy = initObj.robot_igite_academy
      }
      else {
        this.robot_igite_academy = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type my_file
    // Serialize message field [robot_igite_academy]
    bufferOffset = _serializer.float32(obj.robot_igite_academy, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type my_file
    let len;
    let data = new my_file(null);
    // Deserialize message field [robot_igite_academy]
    data.robot_igite_academy = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'test_msgs/my_file';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '069c9e64d21688e60165de9024f3f89c';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32 robot_igite_academy
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new my_file(null);
    if (msg.robot_igite_academy !== undefined) {
      resolved.robot_igite_academy = msg.robot_igite_academy;
    }
    else {
      resolved.robot_igite_academy = 0.0
    }

    return resolved;
    }
};

module.exports = my_file;
