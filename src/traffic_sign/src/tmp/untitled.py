import cv2
import numpy as np
from math import hypot, fabs
from PIL import Image, ImageDraw
import imutils


lower_blue = np.array([100, 100, 100])
upper_blue = np.array([110, 255, 255])
lower_red = np.array([160, 100, 100])
upper_reb = np.array([180, 255, 255])
# dieu khien gioi han diem khac khi so sanh
minDiff = 5000
match = -1
w = 420
h = 420
SZ = 20
CLASS_N = 10
detectedTrafficSign = None
ReferenceTitles = np.array(["30", "40", "Cam30", "MotChieu", "NoTurnLeft",
                            "NoTurnRight", "STOP", "Trangden", "TurnLeft", "TurnRight"])
model = cv2.ml.SVM_load('svm_data.dat')
camera = cv2.VideoCapture('4.avi')
location = open('location.txt', 'w+')
location.write('\n')
fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v')
out = cv2.VideoWriter('output.mp4', fourcc, 60.0, (640, 480))


def savelocation(index, largestRect):
    location.write(str(index) + ' ' + '1 ' + str(largestRect[1][0]) + ' ' + str(
        largestRect[1][1]) + ' ' + str(largestRect[3][0]) + ' ' + str(largestRect[3][1]) + '\n')


def identifyTrafficSign(image):
    hog = get_hog()
    image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    im = [image for i in range(10)]
    #im = image
    deskewed = list(map(deskew, im))
    test = []
    for img in deskewed:
        test.append(hog.compute(img))
    test = np.squeeze(test)
    result = []
    try:
        h, result = model.predict(test)
    except:
        return None
    result = int(result[1])
    return result


def get_hog():
    winSize = (20, 20)
    blockSize = (8, 8)
    blockStride = (4, 4)
    cellSize = (8, 8)
    nbins = 9
    derivAperture = 1
    winSigma = -1.
    histogramNormType = 0
    L2HysThreshold = 0.2
    gammaCorrection = 1
    nlevels = 64
    signedGradient = True
    hog = cv2.HOGDescriptor(winSize, blockSize, blockStride, cellSize, nbins, derivAperture,
                            winSigma, histogramNormType, L2HysThreshold, gammaCorrection, nlevels, signedGradient)
    return hog
    affine_flags = cv2.WARP_INVERSE_MAP | cv2.INTER_LINEAR


def deskew(img):
    m = cv2.moments(img)
    if abs(m['mu02']) < 1e-2:
        return img.copy()
    skew = m['mu11'] / m['mu02']
    M = np.float32([[1, skew, -0.5 * SZ * skew], [0, 1, 0]])
    img = cv2.warpAffine(img, M, (SZ, SZ), flags=cv2.WARP_INVERSE_MAP | cv2.INTER_LINEAR)
    return img

# get 4 diem trong four_point_transform1


def order_points(pts):
    rect = np.zeros((4, 2), dtype="float32")

    s = pts.sum(axis=1)
    rect[0] = pts[np.argmin(s)]
    rect[2] = pts[np.argmax(s)]

    diff = np.diff(pts, axis=1)
    rect[1] = pts[np.argmin(diff)]
    rect[3] = pts[np.argmax(diff)]
    return rect

# get toa do 4 diem


def four_point_transform1(image, pts):
    rect = order_points(pts)
    (tl, tr, br, bl) = rect

    maxWidth = int(30)
    maxHeight = int(30)

    dst = np.array([
        [0, 0],
        [maxWidth - 1, 0],
        [maxWidth - 1, maxHeight - 1],
        [0, maxHeight - 1]], dtype="float32")

    M = cv2.getPerspectiveTransform(rect, dst)
    warped = cv2.warpPerspective(image, M, (maxWidth, maxHeight))

    return warped

# ham tinh ti le khung hinh


def egde(matrix):
    a = matrix[0][0] - matrix[1][0]
    b = matrix[0][1] - matrix[1][1]
    c = matrix[1][0] - matrix[2][0]
    d = matrix[1][1] - matrix[2][1]
    x = hypot(a, b)
    y = hypot(c, d)
    return fabs(x - y)


def tracking_color(image):
    frameArea = image.shape[0] * image.shape[1]
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    kernel = np.ones((3, 3), np.uint8)
    kernel_close = np.ones((3, 3), np.uint8)
    mask1 = cv2.inRange(hsv, lower_blue, upper_blue)
    mask2 = cv2.inRange(hsv, lower_red, upper_reb)
    mask = cv2.bitwise_xor(mask1, mask2)
    mask = cv2.dilate(mask2, kernel, iterations=1)
    #mask = cv2.erode(mask,kernel_close,iterations = 1)
    #mask = cv2.dilate(mask,kernel_close,iterations = 1)

    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
    mask = cv2.morphologyEx(mask, cv2.MORPH_GRADIENT, kernel)
    #cv2.imshow('mask', mask)
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]

    largestArea = 0
    largestRect = np.zeros((4, 2), dtype="int32")

    if len(cnts) > 0:
        for cnt in cnts:
            rect = cv2.minAreaRect(cnt)
            box = cv2.boxPoints(rect)
            box = np.int0(box)
            sideOne = np.linalg.norm(box[0] - box[1])
            sideTwo = np.linalg.norm(box[0] - box[3])
            area = sideOne * sideTwo
            if area > largestArea:
                largestArea = area
                largestRect = box
    if (largestArea > 399) and egde(largestRect) < 12:

        return largestRect
    largestRect = np.zeros((4, 2), dtype="int32")
    return largestRect


# can bang anh, mau red
def equalize_Hist(img):
    #yuv=cv2.cvtColor(img, cv2.COLOR_BGR2YUV)
    # yuv[:,:,0]=cv2.equalizeHist(yuv[:,:,0])
    #img = cv2.cvtColor(yuv, cv2.COLOR_YUV2BGR)
    img[:, :, 2] = cv2.equalizeHist(img[:, :, 2])
    '''
    for c in range(1,2):
        img[:,:,c]=cv2.equalizeHist(img[:,:,c])
           '''
    return img

# ham phat hien su dung cascade


def detect(im, xml):
    digit_cascade = cv2.CascadeClassifier(xml)
    digits = digit_cascade.detectMultiScale(im)
    return digits


if __name__ == '__main__':
    dem = 0
    index = 0
    array_soframe = []
    array_toado = []

    while (camera.isOpened()):
        (grabbed, frame) = camera.read()

        if not grabbed:
            print("No input image")
            break

        # resize kich thuoc khung hinh
        frame = imutils.resize(frame, width=640)
        frame1 = frame
        #out = savevideo((frame.shape[0],frame.shape[1]))
        # quay khung hinh 270 do
        #frame = imutils.rotate(frame, 270)

        # Bien chua toa do 4 diem cua khung hinh
        largestRect_cascade = np.zeros((4, 2), dtype="int32")
        largestRect_color = np.zeros((4, 2), dtype="int32")

        # bat mau trong khung hinh
        largestRect_color = tracking_color(frame)
        if largestRect_color.any() != 0:
            warped = four_point_transform1(frame1, [largestRect_color][0])
            detectedTrafficSign = identifyTrafficSign(warped)
        # can bang sang khung hinh
        #frame = equalize_Hist(frame)

        # bien dem so khung hinh
        index += 1
        # su dung ham phat hien tra ve x,y,width,height
        digits = detect(frame, 'mc.xml')
        # bien tam chua khung hinh
        cnt = np.zeros((4, 2), dtype="int32")
        if len(digits) > 0:
            for region in digits:
                cnt[0][0] = region[0]
                cnt[0][1] = region[1] + region[3]
                cnt[1][0] = region[0]
                cnt[1][1] = region[1]
                cnt[2][0] = region[0] + region[2]
                cnt[2][1] = region[1]
                cnt[3][0] = region[0] + region[2]
                cnt[3][1] = region[1] + region[3]

                cut = frame[region[1]:cnt[0][1], region[0]:cnt[2][0]]
                hsv = cv2.cvtColor(cut, cv2.COLOR_BGR2HSV)
                mask1 = cv2.inRange(hsv, lower_blue, upper_blue)
                mask2 = cv2.inRange(hsv, lower_red, upper_reb)
                diff = cv2.countNonZero(mask2)

                if diff > 199:
                    largestRect_cascade = cnt
                    break

        largestRect = np.zeros((4, 2), dtype="int32")

        if largestRect_cascade.any() != 0:
            dem += 1
            savelocation(index, largestRect_cascade)
            array_soframe.append(1)
            array_toado.append(largestRect_cascade)
            cv2.drawContours(frame, [largestRect_cascade], 0, (0, 0, 255), 2)
            cv2.putText(frame, ReferenceTitles[2], tuple(
                largestRect_cascade[2]), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2)
            # print('HAAR',dem)
            print(index)
        elif detectedTrafficSign == 2 and largestRect_color.any() != 0:
            dem += 1
            savelocation(index, largestRect_color)
            cv2.drawContours(frame, [largestRect_color], 0, (0, 0, 255), 2)
            cv2.putText(frame, ReferenceTitles[detectedTrafficSign], tuple(
                largestRect_color[2]), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2)
            # print('SVM',dem)

        else:
            array_soframe.append(0)
            array_toado.append(largestRect_cascade)
        '''
        if detectedTrafficSign ==3 and largestRect_color.any()!=0:
            dem+=1
            cv2.drawContours(frame,[largestRect_color],0,(0,0,255),2)
            cv2.putText(frame, ReferenceTitles[detectedTrafficSign], tuple(largestRect_color[2]), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2)
            print(dem)
        elif largestRect_cascade.any()!=0:
            dem+=1
            array_soframe.append(1)
            array_toado.append(largestRect_cascade)
            cv2.drawContours(frame,[largestRect_cascade],0,(0,0,255),2)
            cv2.putText(frame, ReferenceTitles[3], tuple(largestRect_cascade[2]), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2)
            print(dem)
        '''

        out.write(frame)
        cv2.imshow("Original", frame)

        if cv2.waitKey(1) & 0xFF is ord('q'):
            cv2.destroyAllWindows()
            print("Stop programm and close all windows")
            break
    location.close()
    '''
    location = open('location.txt','r+')
    location.write(str(index)+'\n')
    location.close()
    '''
