// Auto-generated. Do not edit!

// (in-package traffic_sign.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class traffic_sign_msg {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.traffic_sign_type = null;
    }
    else {
      if (initObj.hasOwnProperty('traffic_sign_type')) {
        this.traffic_sign_type = initObj.traffic_sign_type
      }
      else {
        this.traffic_sign_type = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type traffic_sign_msg
    // Serialize message field [traffic_sign_type]
    bufferOffset = _serializer.int64(obj.traffic_sign_type, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type traffic_sign_msg
    let len;
    let data = new traffic_sign_msg(null);
    // Deserialize message field [traffic_sign_type]
    data.traffic_sign_type = _deserializer.int64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 8;
  }

  static datatype() {
    // Returns string type for a message object
    return 'traffic_sign/traffic_sign_msg';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'b39d3b00117d7861701b14c6e1e0db30';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int64 traffic_sign_type
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new traffic_sign_msg(null);
    if (msg.traffic_sign_type !== undefined) {
      resolved.traffic_sign_type = msg.traffic_sign_type;
    }
    else {
      resolved.traffic_sign_type = 0
    }

    return resolved;
    }
};

module.exports = traffic_sign_msg;
