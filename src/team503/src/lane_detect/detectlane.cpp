#include "detectlane.h"
#include "stdio.h"
#include <math.h>
#include <exception>
#include <string>

int min(int a, int b)
{
    return a < b ? a : b;
}

int DetectLane::slideThickness = 5;
int DetectLane::BIRDVIEW_WIDTH = 240;
int DetectLane::BIRDVIEW_HEIGHT = 320;
int DetectLane::VERTICAL = 0;
int DetectLane::HORIZONTAL = 1;
int DetectLane::carMaxSpeed = 50;
Point DetectLane::null = Point();

int VECTOR_HEIGHT = DetectLane::BIRDVIEW_HEIGHT / DetectLane::slideThickness;
#define PI 3.14159265
const double deg = 180 / PI;
const double rad = PI / 180;

DetectLane::DetectLane() {
    cvCreateTrackbar("LowH", "Threshold", &minThreshold[0], 179);
    cvCreateTrackbar("HighH", "Threshold", &maxThreshold[0], 179);

    cvCreateTrackbar("LowS", "Threshold", &minThreshold[1], 255);
    cvCreateTrackbar("HighS", "Threshold", &maxThreshold[1], 255);

    cvCreateTrackbar("LowV", "Threshold", &minThreshold[2], 255);
    cvCreateTrackbar("HighV", "Threshold", &maxThreshold[2], 255);

    cvCreateTrackbar("Car max speed", "Threshold", &carMaxSpeed, 50);
    cvCreateTrackbar("Car max speed turning", "Threshold", &carMaxSpeedTurning, 50);

    cvCreateTrackbar("percent lane escappe", "Threshold", &percentEscapeObstacles, 100);

    cvCreateTrackbar("weightRun", "Threshold", &weightRun, 50);
    cvCreateTrackbar("weightTurn", "Threshold", &weightTurn, 50);
    cvCreateTrackbar("weightErrorAngle", "Threshold", &weightErrorAngle, 100);

    cvCreateTrackbar("maxErrorLaneDetected", "Threshold", &maxErrorLaneDetected, 20);
    cvCreateTrackbar("minCountMatchLaneDetected", "Threshold", &minCountMatchLaneDetected, 20);

    cvCreateTrackbar("amountStepToTurning", "Threshold", &amountStepToTurning, 300);

    initDisplacement();
}

DetectLane::~DetectLane(){}

vector<Point> DetectLane::getLeftLane()
{
    return leftLane;
}

vector<Point> DetectLane::getRightLane()
{
    return rightLane;
}

void DetectLane::update(const Mat &src)
{
    Mat img = preProcess(src);
    
    // imshow("img", img);
    vector<Mat> layers1 = splitLayer(img);
    // imshow("debug3", layers1);
    vector<vector<Point> > points1 = centerRoadSide(layers1);
    // vector<Mat> layers2 = splitLayer(img, HORIZONTAL);
    // vector<vector<Point> > points2 = centerRoadSide(layers2, HORIZONTAL);

    detectLeftRight(points1);

    Mat birdView, lane, debugView, laneOld, lane2, laneDetected;
    birdView = Mat::zeros(img.size(), CV_8UC3);
    debugView = Mat::zeros(img.size(), CV_8UC3);
    lane = Mat::zeros(img.size(), CV_8UC3);
    laneOld = Mat::zeros(img.size(), CV_8UC3);
    lane2 = Mat::zeros(img.size(), CV_8UC3);
    laneDetected = Mat::zeros(img.size(), CV_8UC3);

    for (int i = 0; i < points1.size(); i++) {
        for (int j = 0; j < points1[i].size(); j++) {
            circle(birdView, points1[i][j], 1, Scalar(125,125,125), 2, 8, 0 );
        }
    }

    for (int i = 0; i < points1.size(); i++) {
        for (int j = 0; j < points1[i].size(); j++) {
            circle(birdView, points1[i][j], 1, Scalar(0,0,255), 2, 8, 0 );
        }
    }

    // for (int i = 0; i < points2.size(); i++)
    //  {
    //     for (int j = 0; j < points2[i].size(); j++)
    //     {
    //         circle(birdView, points2[i][j], 1, Scalar(0,255,0), 2, 8, 0 );
    //     }
    // }

    for (int i = 0; i < this->otherLane.size(); i++) {
        for(int j = 1; j < this->otherLane[i].size(); j++) {
            if (this->otherLane[i][j] != null) {
                circle(lane, this->otherLane[i][j], 1, Scalar(0,255,0), 2, 8, 0 );
            }
        }
    }

    for (int i = 1; i < leftLane.size(); i++) {
        if (leftLane[i] != null)
        {
            circle(lane, leftLane[i], 1, Scalar(0,0,255), 2, 8, 0 );
            circle(laneOld, leftLane[i], 1, Scalar(0,0,255), 2, 8, 0 );
        }
    }

    for (int i = 1; i < rightLane.size(); i++) {
        if (rightLane[i] != null) {
            circle(lane, rightLane[i], 1, Scalar(255,0,0), 2, 8, 0 );
            circle(laneOld, rightLane[i], 1, Scalar(255,0,0), 2, 8, 0 );
        }
    }

    imshow("Lane Detect", lane);

    // for (int i = 1; i < leftLaneOld.size(); i++) {
    //     if (leftLaneOld[i] != null)
    //     {
    //         circle(laneOld, leftLaneOld[i], 1, Scalar(100,100,200), 2, 8, 0 );
    //     }
    // }

    // for (int i = 1; i < rightLaneOld.size(); i++) {
    //     if (rightLaneOld[i] != null) {
    //         circle(laneOld, rightLaneOld[i], 1, Scalar(200,100,100), 2, 8, 0 );
    //     }
    // }

    // imshow("Lane Old", laneOld);

    // for (int i = 1; i < leftLane2.size(); i++) {
    //     if (leftLane2[i] != null)
    //     {
    //         circle(lane2, leftLane2[i], 1, Scalar(0,0,255), 2, 8, 0 );
    //     }
    // }

    // for (int i = 1; i < rightLane2.size(); i++) {
    //     if (rightLane2[i] != null) {
    //         circle(lane2, rightLane2[i], 1, Scalar(255,0,0), 2, 8, 0 );
    //     }
    // }

    // imshow("Lane Detect 2", lane2);

    for (int i = 1; i < leftLaneDetected.size(); i++) {
        if (leftLaneDetected[i] != null)
        {
            circle(laneDetected, leftLaneDetected[i], 1, Scalar(0,0,255), 2, 8, 0 );
        }
    }

    for (int i = 1; i < rightLaneDetected.size(); i++) {
        if (rightLaneDetected[i] != null) {
            circle(laneDetected, rightLaneDetected[i], 1, Scalar(255,0,0), 2, 8, 0 );
        }
    }

    imshow("Lane Detected", laneDetected);
}

Mat DetectLane::preProcess(const Mat &src)
{
    Mat imgThresholded, imgHSV, dst, debug, debug2;

    cvtColor(src, imgHSV, COLOR_BGR2HSV);
    // imshow("img", laneInShadow(src));
    // cvtColor(src, debug, COLOR_BGR2HSV);
    Mat shadow_lane = laneInShadow(src);

    // imshow("debug", shadow_lane);
    // imshow("debug2", both_lane);

    inRange(imgHSV,
            Scalar(minThreshold[0], minThreshold[1], minThreshold[2]),
            Scalar(maxThreshold[0], maxThreshold[1], maxThreshold[2]),
            imgThresholded);

    Mat both_lane;
    cv::add(shadow_lane, imgThresholded, both_lane);
    // imshow("debug3", both_lane);

    dst = birdViewTranform(both_lane);

    // imshow("Bird View", dst);

    fillLane(dst);

    // printf("\n", );

    // namedWindow("Binary");
    // imshow("Binary", imgThresholded);

    return dst;
}

Mat DetectLane::laneInShadow(const Mat &src)
{
    Mat shadowMask, shadow, imgHSV, shadowHSV, laneShadow;

    // cvtColor(src, imgHSV, COLOR_BGR2HSV);
    // inRange(imgHSV,
    //         Scalar(minShadowTh[0], minShadowTh[1], minShadowTh[2]),
    //         Scalar(maxShadowTh[0], maxShadowTh[1], maxShadowTh[2]),
    //         shadowMask);
    // imshow("debug", shadowMask);

    // src.copyTo(shadow, shadowMask);
    // cvtColor(shadow, shadowHSV, COLOR_BGR2HSV);
    // inRange(shadowHSV,
    //         Scalar(minLaneInShadow[0], minLaneInShadow[1], minLaneInShadow[2]), 
    //         Scalar(maxLaneInShadow[0], maxLaneInShadow[1], maxLaneInShadow[2]), 
    //         laneShadow);
    // imshow("debug2", laneShadow);

    Mat imgSihcDetect, sihcDetected;
    cvtColor(src, imgSihcDetect, COLOR_BGR2HSV);
    inRange(imgSihcDetect,
            Scalar(minLaneInShadowSihc[0], minLaneInShadowSihc[1], minLaneInShadowSihc[2]), 
            Scalar(maxLaneInShadowSihc[0], maxLaneInShadowSihc[1], maxLaneInShadowSihc[2]), 
            sihcDetected);
    // imshow("img", sihcDetected);

    return sihcDetected;
}

void DetectLane::fillLane(Mat &src)
{
    vector<Vec4i> lines;
    HoughLinesP(src, lines, 1, CV_PI/180, 1);
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line(src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255), 3, CV_AA);
    }
}

vector<Mat> DetectLane::splitLayer(const Mat &src, int dir)
{
    int rowN = src.rows;
    int colN = src.cols;
    std::vector<Mat> res;

    if (dir == VERTICAL)
    {
        for (int i = 0; i < rowN - slideThickness; i += slideThickness) {
            Mat tmp;
            Rect crop(0, i, colN, slideThickness);
            tmp = src(crop);
            res.push_back(tmp);
        }
    }
    else 
    {
        for (int i = 0; i < colN - slideThickness; i += slideThickness) {
            Mat tmp;
            Rect crop(i, 0, slideThickness, rowN);
            tmp = src(crop);
            res.push_back(tmp);
        }
    }
    
    return res;
}

vector<vector<Point> > DetectLane::centerRoadSide(const vector<Mat> &src, int dir)
{
    vector<std::vector<Point> > res;
    int inputN = src.size();
    for (int i = 0; i < inputN; i++) {
        std::vector<std::vector<Point> > cnts;
        std::vector<Point> tmp;
        findContours(src[i], cnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        int cntsN = cnts.size();
        if (cntsN == 0) {
            res.push_back(tmp);
            continue;
        }

        for (int j = 0; j < cntsN; j++) {
            int area = contourArea(cnts[j], false);
            if (area > 3) {
                Moments M1 = moments(cnts[j], false);
                Point2f center1 = Point2f(static_cast<float> (M1.m10 / M1.m00), static_cast<float> (M1.m01 / M1.m00));
                if (dir == VERTICAL) {
                    center1.y = center1.y + slideThickness*i;
                } 
                else
                {
                    center1.x = center1.x + slideThickness*i;
                }
                if (center1.x > 0 && center1.y > 0) {
                    tmp.push_back(center1);
                }
            }
        }
        res.push_back(tmp);
    }

    return res;
}

Mat DetectLane::morphological(const Mat &img)
{
    Mat dst;

    // erode(img, dst, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)) );
    // dilate( dst, dst, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)) );

    dilate(img, dst, getStructuringElement(MORPH_ELLIPSE, Size(10, 20)) );
    erode(dst, dst, getStructuringElement(MORPH_ELLIPSE, Size(10, 20)) );

    // blur(dst, dst, Size(3, 3));

    return dst;
}

void transform(Point2f* src_vertices, Point2f* dst_vertices, Mat& src, Mat &dst){
    Mat M = getPerspectiveTransform(src_vertices, dst_vertices);
    warpPerspective(src, dst, M, dst.size(), INTER_LINEAR, BORDER_CONSTANT);
}

Mat DetectLane::birdViewTranform(const Mat &src)
{
    Point2f src_vertices[4];

    int width = src.size().width;
    int height = src.size().height;

    src_vertices[0] = Point(0, skyLine);
    src_vertices[1] = Point(width, skyLine);
    src_vertices[2] = Point(width, height);
    src_vertices[3] = Point(0, height);

    Point2f dst_vertices[4];
    dst_vertices[0] = Point(0, 0);
    dst_vertices[1] = Point(BIRDVIEW_WIDTH, 0);
    dst_vertices[2] = Point(BIRDVIEW_WIDTH - 105, BIRDVIEW_HEIGHT);
    dst_vertices[3] = Point(105, BIRDVIEW_HEIGHT);

    Mat M = getPerspectiveTransform(src_vertices, dst_vertices);

    Mat dst(BIRDVIEW_HEIGHT, BIRDVIEW_WIDTH, CV_8UC3);
    warpPerspective(src, dst, M, dst.size(), INTER_LINEAR, BORDER_CONSTANT);

    return dst;
}

void DetectLane::setCarStatus(double speed, double turningAngle, double timeDiff) {
    carTurningAngle = turningAngle;
    carSpeed = speed;
    carTimeDiff = timeDiff;
    carDisplacement = carTimeDiff * carSpeed;
}

const int arp_w = DetectLane::BIRDVIEW_WIDTH;
const int arp_h = VECTOR_HEIGHT;
bool **arp;
void DetectLane::initDisplacement() {
    arp = new bool*[arp_h];
    for (int i = 0; i < arp_h; i++) {
        arp[i] = new bool[arp_w];
    }
    for (int i = 0; i < BIRDVIEW_HEIGHT; i++) {
        vector<Point> line;
        for (int j = 0; j < BIRDVIEW_WIDTH; j++) {

            line.push_back(Point2i(0,0 + i*0.01));
        }
        displacement.push_back(line);
    }

    for(int i = 0; i < arp_h; i++) {
        for(int j = 0; j < arp_w; j++) {
            arp[i][j] = false;
        }
    }
    ROS_INFO("initDisplacement DONE");
}

Point center = Point2i(DetectLane::BIRDVIEW_WIDTH / 2, DetectLane::BIRDVIEW_HEIGHT + 50);
void DetectLane::jumpDisplacement(const vector<Point> &oldV, vector<Point> &newV) {
    // carTimeDiff = 0.02;
    try {
        static int jx, jy;
        for(int i = 0; i < oldV.size(); i++) {
            Point p = oldV[i];
            if(p.y == 0)
                continue;
            double lx, ly;
            lx = (double)(p.x - center.x);
            ly = (double)(center.y - p.y);
            double tang;
            if(lx == 0) {
                // ROS_WARN("lx == 0");
                tang = 1000000000;
            } else {
                tang = ly / lx;
            }
            double angle = atan(tang) * deg + carTurningAngle * ((double)weightTurn / 100.0) * carDisplacement;
            // ROS_INFO("second %.5lf angle %lf", carTimeDiff, angle);
            if (angle < 0)
                angle += 180;
            double hypotenuse = sqrt(lx * lx + ly * ly);

            jx =            int(    cos(angle * rad) * hypotenuse + center.x);
            jy = center.y - int(abs(sin(angle * rad) * hypotenuse));

            // ROS_INFO("[%3d:%3d] [%3d:%3d] [%3d:%3d] | tang: %3.1lf | angle: %3.1lf-%3.1lf | sin = %3.1lf | cos = %3.1lf", 
            //     p.x, p.y, (int)lx, (int)ly, jx, jy, tang, atan(tang) * deg, angle,
            //     sin(angle * rad) * hypotenuse, cos(angle * rad) * hypotenuse);

            jy += int(displacement[p.x][p.y].y * weightRun * carDisplacement);
            if(jx >= 0 && jx < BIRDVIEW_WIDTH && jy >= 0 && jy < BIRDVIEW_HEIGHT) {
                newV.push_back(Point2i(jx, jy));
            }
        }
    } catch (exception& e) {
        ROS_WARN("jumpDisplacement broken : %s", e.what());
    }
}

void DetectLane::getLaneOnOldLane(const vector<vector<Point> > &points, const vector<Point> &oldLane, vector<Point>& newLane, int& laneCount) {
    bool check;
    int maxError = 2, error = 0, x, y;
    try {
        for (int i = oldLane.size() - 2; i >= 0; i--) {
            y = oldLane[i].y / slideThickness;
            // ROS_INFO("right y: %d", y);
            check = true;
            for (int j = 0; j < points[y].size(); j++) {
                if (abs(points[y][j].x - oldLane[i].x + error) < maxError) {
                    newLane[y] = points[y][j];
                    laneCount++;
                    error = oldLane[i].x - points[y][j].x;
                    error += (error == 0) ? 0 : (error > 0) ? 1 : -1;
                    maxError = 10;
                    check = false;
                    break;
                }
            }
            // if (check) {
            //     x = min(y+1, VECTOR_HEIGHT);
            //     // newLane[y] = Point2i(oldLane[i].y, oldLane[i].y);
            //     maxError = min(maxError + 1, abs(error) * 2 + 3);
            // }
        }

        // for (int i = 0; i < points.size(); i++) {
        //     for (int j= 0; j < points[i].size(); j++) {
        //         y = points[i][j].y / slideThickness;
        //         if (y < VECTOR_HEIGHT)
        //             arp[y][points[i][j].x] = true;
        //     }
        // }

        // for (int i = points.size() - 2; i >= 0; i--) {
        //     if (newLane[i] == null) {
        //         x = min(5, int(points.size()) - i - 1);
        //         check = true;
        //         for(int k = 1; k < x && check; k++) {
        //             if (newLane[i+k] != null) {
        //                 for(int j = 0; j < points[i].size(); j++) {
        //                     if (abs(newLane[i+1].x - points[i][j].x) < 5) {
        //                         newLane[i] = points[i][j];
        //                         laneCount++;
        //                         check = false;
        //                         break;
        //                     }
        //                 }
        //             }
        //         }
        //     }
        // }

        if (oldLane.size() == 0)
            return;
        x = 0;
        int s = oldLane.size();
        // ROS_INFO("error = %d", error);
        y = oldLane[x].y / slideThickness;
        // while(x < s) {
        //     if(newLane[y] == null) {
        //         newLane[y] = Point2i(oldLane[x].x, oldLane[x].y);
        //         laneCount++;
        //     }
        //     x++;
        // }
    } catch (exception& e) {
        ROS_WARN("getLaneOnOldLane broken : %s", e.what());
    }
}

void DetectLane::fillLane(const vector<vector<Point> > &points, vector<Point>& leftLane, vector<Point>& rightLane) {
    try {
        int maxError = 5;
        if(leftLane2Count == 0)
            return;
        for (int i = 0; i < points.size(); i++) {
            for (int j = 0; j < points[i].size(); j++) {
                Point p = points[i][j];
                // ROS_INFO("p.y = %d", p.y / slideThickness);
                if( leftLane2[p.y / slideThickness] == null ) {
                    for (int m = 1; m < min(i - 1, 8); m++) {
                        // ROS_INFO("i - m = %d", i - m);
                        bool check = false;
                        for (int k = 0; k < points[i - m].size(); k++) {
                            if (abs(points[i - m][k].x - points[i][j].x) < maxError) {
                                leftLane2[p.y / slideThickness] = points[i - m][k];
                            }
                        }
                        break;
                    }
                }
            }
        }
        ROS_INFO("fillLane DONE!");
    } catch (exception& e) {
        ROS_WARN("fillLane broken: %s", e.what());
    }
}

bool useLane2 = false;
bool useLaneDetected = true;
bool debugDetectLeftRight = false;
vector<Point> leftLaneOld2, rightLaneOld2;
void DetectLane::detectLeftRight(const vector<vector<Point> > &points)
{
    try {
        static vector<Point> lane1, lane2;

        lane1.clear();
        lane2.clear();
        this->otherLane.clear();

        leftLaneOld.clear();
        rightLaneOld.clear();

        // ROS_INFO("Left  %lf", carTurningAngle);
        jumpDisplacement(leftLane,  leftLaneOld);
        // ROS_INFO("Right %lf", carTurningAngle);
        jumpDisplacement(rightLane, rightLaneOld);

        // ROS_INFO("LaneOld size:  left %d   right %d", (int)leftLaneOld.size(), (int)rightLaneOld.size());

        leftLane2Count = 0;
        rightLane2Count = 0;
        leftLaneCount = 0;
        rightLaneCount = 0;

        leftLane.clear();
        rightLane.clear();
        // leftLane2.clear();
        // rightLane2.clear();
        leftLaneOld2.clear();
        rightLaneOld2.clear();
        leftLaneDetected.clear();
        rightLaneDetected.clear();
        for (int i = 0; i < BIRDVIEW_HEIGHT / slideThickness; i ++)
        {
            leftLane.push_back(null);
            rightLane.push_back(null);
            // leftLane2.push_back(null);
            // rightLane2.push_back(null);
            leftLaneOld2.push_back(null);
            rightLaneOld2.push_back(null);
            leftLaneDetected.push_back(null);
            rightLaneDetected.push_back(null);
        }

        for (int i = 0; i < leftLaneOld.size(); i++) {
            leftLaneOld2[leftLaneOld[i].y / slideThickness] = leftLaneOld[i];
        }
        for (int i = 0; i < rightLaneOld.size(); i++) {
            rightLaneOld2[rightLaneOld[i].y / slideThickness] = rightLaneOld[i];
        }
        // ROS_INFO("Lane Old size = %d %d", int(leftLaneOld.size()), int(rightLaneOld.size()));

        // ROS_INFO("leftLane2");
        // getLaneOnOldLane(points, leftLaneOld,  leftLane2, leftLane2Count);
        // ROS_INFO("rightLane2");
        // getLaneOnOldLane(points, rightLaneOld, rightLane2, rightLane2Count);
        // ROS_INFO("Done 2 Lane");

        // fillLane(points, leftLane2, rightLane2);

        int pointMap[points.size()][20];
        int prePoint[points.size()][20];
        int postPoint[points.size()][20];
        int dis = 10;
        int max1 = -1, max2 = -1;
        Point2i posMax, posMax2;

        memset(pointMap, 0, sizeof pointMap);

        for (int i = 0; i < points.size(); i++)
        {
            for (int j = 0; j < points[i].size(); j++)
            {
                pointMap[i][j] = 1;
                prePoint[i][j] = -1;
                postPoint[i][j] = -1;
            }
        }

        for (int i = points.size() - 2; i >= 0; i--)
        {
            for (int j = 0; j < points[i].size(); j++)
            {
                int err = 200;
                for (int m = 1; m < min(points.size() - 1 - i, 8); m++)
                {
                    bool check = false;
                    for (int k = 0; k < points[i + m].size(); k++)
                    {
                        if (abs(points[i + m][k].x - points[i][j].x) < dis && abs(points[i + m][k].x - points[i][j].x) < err) {
                            err = abs(points[i + m][k].x - points[i][j].x);
                            pointMap[i][j] = pointMap[i + m][k] + 1;
                            prePoint[i][j] = k;
                            postPoint[i + m][k] = j;
                            check = true;
                        }
                    }
                    break;
                }
                
                if (pointMap[i][j] > max1) 
                {
                    max1 = pointMap[i][j];
                    posMax = Point2i(i, j);
                }
            }
        }
        if (debugDetectLeftRight)
            ROS_INFO("Done 1");


        // for (int i = 0; i < points.size(); i++)
        // {
        //     for (int j = 0; j < points[i].size(); j++)
        //     {
        //         if (pointMap[i][j] > max2 && (i != posMax.x || j != posMax.y) && postPoint[i][j] == -1)
        //         {
        //             max2 = pointMap[i][j];
        //             posMax2 = Point2i(i, j);
        //         }
        //     }
        // }

        if (debugDetectLeftRight)
            ROS_INFO("Done 2");

        if (max1 == -1) {
            ROS_WARN("detectLeftRight not found any stain LANE!");
            return;
        }

        // while (max1 >= 1)
        // {
        //     lane1.push_back(points[posMax.x][posMax.y]);
        //     if (max1 == 1) break;

        //     if(0 < prePoint[posMax.x][posMax.y] && prePoint[posMax.x][posMax.y] < posMax.y) {
        //         posMax.y = prePoint[posMax.x][posMax.y];
        //         posMax.x += 1;

        //         max1--;
        //     } else {
        //         break;
        //     }
        // }

        // while (max2 >= 1)
        // {
        //     lane2.push_back(points[posMax2.x][posMax2.y]);
        //     if (max2 == 1) break;

        //     if(0 < prePoint[posMax2.x][posMax2.y] && prePoint[posMax2.x][posMax2.y] < posMax2.y) {
        //         posMax2.y = prePoint[posMax2.x][posMax2.y];
        //         posMax2.x += 1;

        //         max2--;
        //     } else {
        //         break;
        //     }
        // }

        // if (debugDetectLeftRight)
        //     ROS_INFO("Done 3");
    //////////////////////////////////////////////////////////
        vector< vector< Point > > stain;
        vector< Point > stainStart;
        vector< Point > stainEnd;
        int tmpMax = 0;
        for (int i = 0; i < points.size()-9; i++) {
            for (int j = 0; j < points[i].size(); j++) {
                if ( pointMap[i][j] > 8 ) {
                    vector< Point > newStain;
                    tmpMax = pointMap[i][j];
                    Point tmpPoint = Point2i(i, j);
                    stainEnd.push_back(points[tmpPoint.x][tmpPoint.y]);
                    newStain.push_back(points[tmpPoint.x][tmpPoint.y]);
                    while( tmpMax > 2 ) {
                        tmpPoint.y = prePoint[tmpPoint.x][tmpPoint.y];
                        tmpPoint.x++;
                        tmpMax--;
                        newStain.push_back(points[tmpPoint.x][tmpPoint.y]);
                        pointMap[tmpPoint.x][tmpPoint.y] = 6;
                    }
                    stainStart.push_back(points[tmpPoint.x][tmpPoint.y]);
                    stain.push_back(newStain);
                }
            }
        }

        if (debugDetectLeftRight)
            ROS_INFO("Done 4");

        Point p;
        int maxError = 7, y;
        for (int i = 0; i < stain.size(); i++) {
            int countMatch = 0, countMiss = 0;
            for (int j = 0; j < stain[i].size(); j++) {
                p = stain[i][j];
                y = p.y / slideThickness;
                if (abs(p.x - leftLaneOld2[y].x) < maxError) {
                    countMatch++;
                } else {
                    countMiss++;
                }
            }
            if (countMiss < 5 || countMatch > minCountMatchLaneDetected) {
                for (int j = 0; j < stain[i].size(); j++) {
                    p = stain[i][j];
                    y = p.y / slideThickness;
                    if (leftLaneDetected[y] == null || leftLaneDetected[y].x < stain[i][j].x) {
                        leftLaneDetected[y] = stain[i][j];
                        leftLaneCount++;
                    }
                }
            }
        }
        for (int i = 0; i < stain.size(); i++) {
            int countMatch = 0, countMiss = 0;
            for (int j = 0; j < stain[i].size(); j++) {
                p = stain[i][j];
                y = p.y / slideThickness;
                if (abs(p.x - rightLaneOld2[y].x) < maxError) {
                    countMatch++;
                } else {
                    countMiss++;
                }
            }
            if (countMiss < 5 || countMatch > minCountMatchLaneDetected) {
                for (int j = 0; j < stain[i].size(); j++) {
                    p = stain[i][j];
                    y = p.y / slideThickness;
                    if (rightLaneDetected[y] == null || rightLaneDetected[y].x > stain[i][j].x) {
                        rightLaneDetected[y] = stain[i][j];
                        rightLaneCount++;
                    }
                }
            }
        }

        if (debugDetectLeftRight)
            ROS_INFO("Done 5");

        int idRightLane = -1, idLeftLane = -1, xRight, xLeft, xRightEnd, xLeftEnd;
        errorAngle = 0;
        Point start, end;
        if (leftLaneCount <= minCountMatchLaneDetected || !useLaneDetected || turning) {
            if (rightLaneCount <= minCountMatchLaneDetected || !useLaneDetected || turning) {
                for (int i = 0; i < stain.size(); i++) {
                    start = stain[i][stain[i].size() - 1];
                    end = stain[i][0];
                    if (start.y > 200) {
                        if (start.x > 125) {
                            if (idRightLane == -1 || (start.x < xRight) ||
                                (start.x == xRight && end.x < xRightEnd))
                            {
                                idRightLane = i;
                                xRight = start.x;
                                xRightEnd = end.x;
                            }
                        } else if (start.x < 115) {
                            if (idLeftLane == -1  || (start.x > xLeft) ||
                                (start.x == xLeft && end.x > xLeftEnd))
                            {
                                idLeftLane = i;
                                xLeft = start.x;
                                xLeftEnd = end.x;
                            }
                        }
                    }
                }
            }

            else {
                int j = rightLaneDetected.size() - 1;
                while(rightLaneDetected[j] == null) j--;
                for (int i = 0; i < stain.size(); i++) {
                    start = stain[i][stain[i].size() - 1];
                    end = stain[i][0];
                    if (start.y > 200) {
                        if (start.x < rightLaneDetected[j].x - (laneWidth * 2 / 3)) {
                            if (idLeftLane == -1  || (start.x > xLeft) ||
                                (start.x == xLeft && end.x > xLeftEnd))
                            {
                                idLeftLane = i;
                                xLeft = start.x;
                                xLeftEnd = end.x;
                            }
                        }
                    }
                }
            }

        } else {
            if (rightLaneCount <= minCountMatchLaneDetected) {
                int j = leftLaneDetected.size() - 1;
                while(leftLaneDetected[j] == null) j--;
                for (int i = 0; i < stain.size(); i++) {
                    start = stain[i][stain[i].size() - 1];
                    end = stain[i][0];
                    if (start.y > 200) {
                        if (start.x > leftLaneDetected[j].x + (laneWidth * 2 / 3)) {
                            if (idRightLane == -1 || (start.x < xRight) ||
                                (start.x == xRight && end.x < xRightEnd))
                            {
                                idRightLane = i;
                                xRight = start.x;
                                xRightEnd = end.x;
                            }
                        }
                    }
                }
            } 

            else {
                int j1 = leftLaneDetected.size() - 1, j2 = rightLaneDetected.size() - 1;
                while(leftLaneDetected[j1] == null) j1--;
                while(rightLaneDetected[j2] == null) j2--;
                int xL1 = leftLaneDetected[j1].x, xR1 = rightLaneDetected[j2].x;
                int yL1 = leftLaneDetected[j1].y, yR1 = rightLaneDetected[j2].y;
                laneWidth = (0.9 * laneWidth + 0.1 * abs(xL1 - xR1));
                // ROS_INFO("laneWidth %f", laneWidth);
                j1 = j2 = 0;
                while(leftLaneDetected[j1] == null) j1++;
                while(rightLaneDetected[j2] == null) j2++;
                int xL2 = leftLaneDetected[j1].x, xR2 = rightLaneDetected[j2].x;
                int yL2 = leftLaneDetected[j1].y, yR2 = rightLaneDetected[j2].y;
                errorAngle = ((double)weightErrorAngle / 10000.0) * ((yL1 - yL2) * (xL2 - xL1) + (yR1 - yR2) * (xR2 - xR1));
                // ROS_INFO("errorAngle %lf", errorAngle);
            }
        }

        if (debugDetectLeftRight)
            ROS_INFO("Done 6");




        // ROS_INFO("Counta: %d | %d    id: %d | %d", leftLaneCount, rightLaneCount, idLeftLane, idRightLane);
        if(useLaneDetected && leftLaneCount > minCountMatchLaneDetected && !turning) {
            // leftLane = leftLane2;
            leftLane = leftLaneDetected;
            // int c = 0, c2 = 0;
            // for(int i = 0; i < leftLaneDetected.size(); i++)
            //     if (leftLaneDetected[i] == null) c++;
            // for(int i = 0; i < leftLane.size(); i++)
            //     if (leftLane[i] == null) c2++;
            // ROS_INFO("left compare: %d %d", c, c2);
        } else if(idLeftLane != -1) {
            if ((useLane2 || useLaneDetected) && !turning)
                ROS_INFO("No Left  Lane Detected!");
            this->maxL = stain[idLeftLane].size();
            for (int i = 0; i < stain[idLeftLane].size(); i++) {
                leftLane[floor(stain[idLeftLane][i].y / slideThickness)] = stain[idLeftLane][i];
            }
        } else {
            this->maxL = 0;
        }

        if(useLaneDetected && rightLaneCount > minCountMatchLaneDetected && !turning) {
            // rightLane = rightLane2;
            rightLane = rightLaneDetected;
            // int c = 0, c2 = 0;
            // for(int i = 0; i < rightLaneDetected.size(); i++)
            //     if (rightLaneDetected[i] == null) c++;
            // for(int i = 0; i < rightLane.size(); i++)
            //     if (rightLane[i] == null) c2++;
            // ROS_INFO("right compare: %d %d", c, c2);
        } else if(idRightLane != -1) {
            if ((useLane2 || useLaneDetected) && !turning)
                ROS_INFO("No Right Lane Detected!");
            this->maxR = stain[idRightLane].size();
            for (int i = 0; i < stain[idRightLane].size(); i++) {
                rightLane[floor(stain[idRightLane][i].y / slideThickness)] = stain[idRightLane][i];
            }
        } else {
            this->maxR = 0;
        }

        if(idRightLane == -1 && idLeftLane != -1) {
            ROS_WARN("NO LANE!!!");
        }

        for(int i = 0; i < stain.size(); i++) {
            if(i != idLeftLane && i != idRightLane) {
                this->otherLane.push_back(stain[i]);
            }
        }


        // vector<Point> subLane1(lane1.begin(), lane1.begin() + 5);
        // vector<Point> subLane2(lane2.begin(), lane2.begin() + 5);
        // Vec4f line1, line2;

        // fitLine(subLane1, line1, 2, 0, 0.01, 0.01);
        // fitLine(subLane2, line2, 2, 0, 0.01, 0.01);

        // int lane1X = (BIRDVIEW_WIDTH - line1[3]) * line1[0] / line1[1] + line1[2];
        // int lane2X = (BIRDVIEW_WIDTH - line2[3]) * line2[0] / line2[1] + line2[2];

        // if (lane1X < lane2X)
        // {
        //     for (int i = 0; i < lane1.size(); i++)
        //     {
        //         leftLane[floor(lane1[i].y / slideThickness)] = lane1[i];
        //     }
        //     this->maxL = lane1.size();
        //     for (int i = 0; i < lane2.size(); i++)
        //     {
        //         rightLane[floor(lane2[i].y / slideThickness)] = lane2[i];
        //     }
        //     this->maxR = lane2.size();
        // }
        // else
        // {
        //     for (int i = 0; i < lane2.size(); i++)
        //     {
        //         leftLane[floor(lane2[i].y / slideThickness)] = lane2[i];
        //     }
        //     this->maxL = lane2.size();
        //     for (int i = 0; i < lane1.size(); i++)
        //     {
        //         rightLane[floor(lane1[i].y / slideThickness)] = lane1[i];
        //     }
        //     this->maxR = lane1.size();
        // }
        // printf("max: %2d|%2d  LaneX : %3d|%3d\n", this->maxL, this->maxR, min(lane1X, lane2X), max(lane1X, lane2X));
        // printf("max: %2d|%2d\n", this->maxL, this->maxR);
        if (debugDetectLeftRight)
            ROS_INFO("detectLeftRight DONE!");
    } catch (exception& e) {
        ROS_WARN("detectLeftRight broken: %s", e.what());
    }

}

std::vector<int> DetectLane::getMax(){
    std::vector<int> v;
    v.push_back(this->maxL);
    v.push_back(this->maxR);
    return v;
}
