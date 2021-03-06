#!/usr/bin/env python2

import roslib
roslib.load_manifest('Team503')
import rospy
import rospkg

from sensor_msgs.msg import CompressedImage, Image
from traffic_sign.msg import traffic_sign_msg
from std_msgs.msg import Int64

import cv2, time
import numpy as np
from math import hypot, fabs

import tensorflow as tf

STREAM = False

# from CNN_3channels_1conv import deepnn
# CNN_MODEL_DIR = "/model/traffic_sign_detect/3cnn_1conv_10epoch_s28.ckpt"
# IMG_SIZE = 28

from CNN_3channels_4conv import deepnn
CNN_MODEL_DIR = "/model/traffic_sign_detect/3cnn_4conv.ckpt"
IMG_SIZE = 56

LABEL = {
    1: 'Dung',
    2: 'Re trai',
    3: 'Re phai',
    4: 'Cam re trai',
    5: 'Cam re phai',
    6: 'Mot chieu',
    7: 'Toc do toi da',
    8: 'Others'
}


rospack = rospkg.RosPack()
path = rospack.get_path('Team503')

x_placeholder = tf.placeholder(tf.float32, [None, IMG_SIZE, IMG_SIZE, 3])
y_conv, keep_prob = deepnn(x_placeholder)
predict = tf.argmax(y_conv, 1)

sess = tf.Session()
saver = tf.train.Saver()
print path + CNN_MODEL_DIR
saver.restore(sess, path + CNN_MODEL_DIR)





def detect(frame):
    # Use Gaussian Blur to reduce high frequency noise
    # and allow us to focus on the structural objects
    # inside the frame
    blurred = cv2.GaussianBlur(frame, (3, 3), 0)
    # @TODO: implement "Bilateral Filtering" and see if it's cost

    # Convert BGR to HSV
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

    # Threshold the HSV image to get only red
    red1 = cv2.inRange(hsv, (0, 100, 100), (15, 255, 255))
    red2 = cv2.inRange(hsv, (160, 100, 120), (180, 255, 255))
    red_mask = cv2.add(red1, red2)

    # Threshold the HSV image to get only blue
    blue_mask = cv2.inRange(hsv, (100, 120, 100), (120, 255, 255))

    mask = cv2.add(red_mask, blue_mask)
    # cv2.imshow("mask", mask)

    # Erode to reduce noise and dilate to focus
    mask = cv2.erode(mask, None, iterations=1)
    mask = cv2.dilate(mask, None, iterations=3)

    # Find contours in the mask
    # cnts = cv2.findContours(image = mask.copy(),
    # mode = cv2.RETR_EXTERNAL,
    # method = cv2.CHAIN_APPROX_SIMPLE)[-2]
    cnts = cv2.findContours(image=mask, mode=cv2.RETR_EXTERNAL,
                            method=cv2.CHAIN_APPROX_SIMPLE)[-2]

    # Proceed if at least one contour was found
    if len(cnts) > 0:
        # Draw all contours and fill the contour interiors -> mask
        cv2.drawContours(image=mask, contours=cnts, contourIdx=-1,
                         color=255, thickness=-1)
        mask = cv2.dilate(mask, None, iterations=3)
        mask = cv2.erode(mask, None, iterations=3)

    # Draw a rectangle outside each contour
    cnts = cv2.findContours(image=mask.copy(),
                            mode=cv2.RETR_EXTERNAL,
                            method=cv2.CHAIN_APPROX_SIMPLE)[-2]
    i = 0
    label = []
    for cnt in cnts:
        x, y, w, h = cv2.boundingRect(cnt)
        if w > 20 and h > 20 and float(h) / w > 0.8 and \
                float(h) / w < 1.5:
            try:
                window = frame[y:y + h, x:x + w]
                i += 1
                window = cv2.resize(window, (IMG_SIZE, IMG_SIZE))
                # cv2.imshow("window %d" % i, window)

                # window = cv2.cvtColor(
                #     window, cv2.COLOR_HSV2RGB)  # hsv2rgb
                label = sess.run(predict,
                                 feed_dict={
                                     x_placeholder: [window],
                                     keep_prob: 1.0})

                if label[0] != 8:
                    # logging.warning(label[0], str(_y_conv))
                    print label
                    cv2.rectangle(
                        frame,
                        (x, y),
                        (x + w, y + h),
                        (0, 255, 100), 1)
                    cv2.putText(
                        frame, LABEL[label[0]],
                        (x, y),
                        cv2.FONT_HERSHEY_SIMPLEX,
                        0.3, (0, 255, 0), 1, cv2.LINE_AA)
            except Exception as e:
                print(e)

    # cv2.imshow("frame", frame)
    # cv2.waitKey(25)
    # cv2.imshow("mask", mask) # debug purpose @TODO: remove
    return label


VERBOSE = False

class detect_sign:
    def __init__(self):
        self.image_sub = rospy.Subscriber("Team503_image/compressed", CompressedImage, self.callback, queue_size=1)
        self.pub = rospy.Publisher('detect_traffic_sign/message', Int64, queue_size=2)
        # self.msg = traffic_sign_msg()
        # self.default_empty_sign = 0
        # self.msg.traffic_sign_type = self.default_empty_sign
        self.msg = Int64(0)
        self.pub.publish(self.msg)
        print 'init'

    def callback(self, ros_data):
        # cv2.imgshow('sihc', data)
        try:
            np_arr = np.fromstring(ros_data.data, np.uint8)
            image_np = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

            # cv2.imshow("sihc before", image_np)
            # cv2.waitKey(1)
            sign = detect(image_np)
            self.notice(sign)

            # largestRect_cascade = np.zeros((4, 2), dtype="int32")
            # largestRect_color = np.zeros((4, 2), dtype="int32")

            # largestRect_color = tracking_color(image_np)
            # if largestRect_color.any() != 0:
            #     warped = four_point_transform1(image_np, [largestRect_color][0])
            #     detectedTrafficSign = identifyTrafficSign(warped)
            #     cv2.drawContours(frameimage_np, [largestRect_color], 0, (0, 0, 255), 2)
            #     cv2.putText(image_np, ReferenceTitles[detectedTrafficSign], tuple(
            #     largestRect_color[2]), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2)
            #     print 'detected!'

            # cv2.imshow('sihc after', image_np)
            # cv2.waitKey(1)
        except Exception as e:
            print 'Callback Fail!'
            print e

        # print 'callback'

    def notice(self, sign):
        try:
            if sign and sign != [0]:
                self.msg = Int64(sign[0])
            else:
                self.msg = Int64(0)
            self.pub.publish(self.msg)
        except Exception as e:
            print 'Publish msg Fail!'
            print e


def main():
    print 'main'
    rospy.init_node('detect_traffic_sign')
    ds = detect_sign()
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print "Shutting down"
    cv2.destroyAllWindows()

def run_detect(path):
    cap = cv2.VideoCapture(path)
    while (cap.isOpened()):
        # Take each frame
        ret, frame = cap.read()
        if ret is True:
            detect(frame)
        # if cv2.waitKey(25) & 0xFF == ord('q'):
        #     break
        else:
            break
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    if STREAM:
        print 'ok thoi ma'
        main(sys.argv)
    else:
        run_detect("MVI_1049.avi")
