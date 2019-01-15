#ifndef DETECTLANE_H
#define DETECTLANE_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <ros/ros.h>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;
using namespace cv;

class DetectLane
{
public:
    DetectLane();
    ~DetectLane();

    void update(const Mat &src);
    
    vector<Point> getLeftLane();
    vector<Point> getRightLane();
    std::vector<int> getMax();

    static int slideThickness;

    static int BIRDVIEW_WIDTH;
    static int BIRDVIEW_HEIGHT;

    static int VERTICAL;
    static int HORIZONTAL;
    static int carMaxSpeed;

    static Point null; // 

    void setCarStatus(double, double, double);
    float laneWidth = 30;

    bool turning = false;
    double errorAngle = 0;
    int weightErrorAngle = 3;
    int percentEscapeObstacles = 70;
    int carMaxSpeedTurning = 35;
    int amountStepToTurning = 150;

private:
    Mat preProcess(const Mat &src);

    Mat morphological(const Mat &imgHSV);
    Mat birdViewTranform(const Mat &source);
    void fillLane(Mat &src);
    vector<Mat> splitLayer(const Mat &src, int dir = VERTICAL);
    vector<vector<Point> > centerRoadSide(const vector<Mat> &src, int dir = VERTICAL);
    void detectLeftRight(const vector<vector<Point> > &points);
    void detectLeftRight2(const vector<vector<Point> > &points);
    Mat laneInShadow(const Mat &src);

    void initDisplacement();
    void jumpDisplacement(const vector<Point>&, vector<Point>&);
    void getLaneOnOldLane(const vector< vector<Point> >&, const vector<Point>&, vector<Point>&, int&);
    void fillLane(const vector< vector<Point> >&, vector<Point>&, vector<Point>&);

    int minThreshold[3] = {0, 0, 180};
    int maxThreshold[3] = {179, 30, 255};
    int minShadowTh[3] = {90, 43, 36};
    int maxShadowTh[3] = {120, 81, 171};
    int minLaneInShadow[3] = {90, 43, 97};
    int maxLaneInShadow[3] = {120, 80, 171};
    int minLaneInShadowSihc[3] = {100, 70, 50};
    int maxLaneInShadowSihc[3] = {130, 100, 150};
    int binaryThreshold = 180;

    int skyLine = 85;
    int shadowParam = 40;
    int maxL;
    int maxR;

    double carSpeed;
    double carTurningAngle;
    double carTimeDiff;
    double carDisplacement;

    int weightTurn = 2;
    int weightRun  = 7;

    int maxErrorLaneDetected = 5;
    int minCountMatchLaneDetected = 7;

    vector< vector<Point> > displacement;
    vector<Point> leftLane, rightLane;
    vector<Point> leftLane2, rightLane2;
    vector<Point> leftLaneOld, rightLaneOld;
    vector<Point> leftLaneDetected, rightLaneDetected;
    int leftLaneCount, rightLaneCount;
    int leftLane2Count, rightLane2Count;
    vector< vector<Point> > otherLane;
};

#endif
