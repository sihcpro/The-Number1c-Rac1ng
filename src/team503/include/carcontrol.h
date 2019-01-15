#ifndef CARCONTROL_H
#define CARCONTROL_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <ros/ros.h>
#include "std_msgs/Float32.h"

#include <vector>
#include <math.h>
#include <ctime>
#include <time.h>
#include <chrono>

#include "detectlane.h"

using namespace std;
using namespace cv;


class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const { 
        return std::chrono::duration_cast<second_>
            (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};

class CarControl 
{
public:
    CarControl();
    ~CarControl();
    void driverCar(const vector<Point> &left, const vector<Point> &right, const vector<int> &maxV);

    static const int chooseRight = 1;
    static const int chooseLeft = 2;
    static const int chooseStraight = 0;
    int goodChoise = chooseStraight;

    void setVelocity(float, float);
    Timer now;
    double timeDiff = 0;
    double velocityNow = 0;
    double distance = 0;
    double errorAngleDetect = 0;
    float preError;
    float laneWidth = 20;

    void setLandWidth(float);

private:
    float errorAngle(const Point &dst);
    ros::NodeHandle node_obj1;
    ros::NodeHandle node_obj2;
    
    ros::Publisher steer_publisher;
    ros::Publisher speed_publisher;

    Point carPos;


    double minVelocity = 10;
    double maxVelocity = 50;
    double velocity = 0;
    double aRun = 5;
    double aStop = 15;


    float kP;
    float kI;
    float kD;

    int t_kP;
    int t_kI;
    int t_kD;

};

#endif
