#!/usr/bin/env python2

import roslib
roslib.load_manifest('traffic_sign')
import rospy

STREAM = False

if __name__ == '__main__':
    if STREAM:
        pass
    else:
        print 'ok'

def funny():
    print "ha ha"