#! /usr/bin/python

import roslib
roslib.load_manifest('traffic_sign')
import rospy

STREAM = False

if __name__ == '__main__':
    if STREAM:
        pass
    else:
        print 'ok'
