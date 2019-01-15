#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <std_msgs/Int64.h>

#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>

#include "detectlane.h"
#include "carcontrol.h"

bool STREAM = true;

VideoCapture capture("video.avi");
DetectLane *detect;
CarControl *car;
int skipFrame = 1;
int traffic_sign_effect = 0;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    Mat out;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        // cv::imshow("View", cv_ptr->image);
        waitKey(1);
        detect->setCarStatus(car->velocityNow, (double)car->preError, car->timeDiff);
        detect->update(cv_ptr->image);

        car->laneWidth = (double) detect->percentEscapeObstacles / 100.0 * (detect->laneWidth);
        car->errorAngleDetect = detect->errorAngle;

        car->driverCar(detect->getLeftLane(), detect->getRightLane(), detect->getMax());
        // ROS_INFO("Time flip: %lf", car->now.elapsed());
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

void videoProcess()
{
    Mat src;
    while (true)
    {
        capture >> src;
        if (src.empty()) break;
        
        // imshow("View", src);
        detect->update(src);
        // waitKey(30);
    }
}

int max_effect_times = 150;
void trafficSignCallback(const std_msgs::Int64::ConstPtr traffic_sign_type) {
    long long type = traffic_sign_type->data;
    switch(type) {
        case 2:
            max_effect_times = detect->amountStepToTurning;
            if(car->goodChoise != car->chooseLeft)
                ROS_INFO("Nhan dien bien bao re trai\n");
            car->goodChoise = car->chooseLeft;
            traffic_sign_effect = max_effect_times;
            car->setVelocity(10, detect->carMaxSpeedTurning);
            detect->turning = true;
            break;
        case 3:
            max_effect_times = detect->amountStepToTurning;
            if(car->goodChoise != car->chooseRight)
                ROS_INFO("Nhan dien bien bao re phai\n");
            car->goodChoise = car->chooseRight;
            traffic_sign_effect = max_effect_times;
            car->setVelocity(10, detect->carMaxSpeedTurning);
            detect->turning = true;
            break;
        default:
            if(traffic_sign_effect > 1) {
                traffic_sign_effect--;
            } else if(traffic_sign_effect == 1 ) {
                ROS_INFO("Tap trung di thang\n");
                traffic_sign_effect--;
                car->goodChoise = car->chooseStraight;
                car->setVelocity(10, DetectLane::carMaxSpeed);
                detect->turning = false;
            }
    }
}

int main(int argc, char **argv)
{
    Mat imgThresholded, dst;
    ros::init(argc, argv, "image_listener");
    // cv::namedWindow("img");
    // cv::namedWindow("debug");
    // cv::namedWindow("debug2");
    // cv::namedWindow("debug3");
    // cv::namedWindow("View");
    // cv::namedWindow("Binary");
    cv::namedWindow("Threshold");
    // cv::namedWindow("Bird View");
    // cv::namedWindow("Lane Detect");

    detect = new DetectLane();
    car = new CarControl();

    if (STREAM) {
        // cv::startWindowThread();

        ros::NodeHandle nh;
        image_transport::ImageTransport it(nh);
        image_transport::Subscriber sub = it.subscribe("team503_image", 1, imageCallback);
        ros::Subscriber sub2 = nh.subscribe("team503_detect_traffic_sign/message", 2, trafficSignCallback);
        ros::spin();
    } else {
        videoProcess();
    }
    cv::destroyAllWindows();
}
