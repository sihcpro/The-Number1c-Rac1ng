#include "carcontrol.h"
#include <ctime>

CarControl::CarControl()
{
    now = Timer();
    carPos.x = 120;
    carPos.y = 300;
    steer_publisher = node_obj1.advertise<std_msgs::Float32>("team503_steerAngle",10);
    speed_publisher = node_obj2.advertise<std_msgs::Float32>("team503_speed",10);
}

CarControl::~CarControl() {}

float CarControl::errorAngle(const Point &dst)
{
    if (dst.x == carPos.x) return 0;
    if (dst.y == carPos.y) return (dst.x < carPos.x ? -90 : 90);
    double pi = acos(-1.0);
    double dx = dst.x - carPos.x;
    double dy = carPos.y - dst.y; 
    if (dx < 0) return -atan(-dx / dy) * 180 / pi;
    return atan(dx / dy) * 180 / pi + errorAngleDetect;
}

void CarControl::setVelocity(float minV, float maxV) {
    minVelocity = minV;
    maxVelocity = maxV;
}

void CarControl::setLandWidth(float width){
    laneWidth = width;
}

void CarControl::driverCar(const vector<Point> &left, const vector<Point> &right, const vector<int> &maxV)
{
    // ROS_INFO("velocity = %lf", velocity);
    // ROS_INFO("time change : %lf", now.elapsed());
    timeDiff = now.elapsed();
    if(velocity > velocityNow) {
        velocityNow = min(velocity, velocityNow + aRun * timeDiff);
    } else {
        velocityNow = max(velocity, velocityNow - aStop * timeDiff);
    }
    // ROS_INFO("Speed : %lf    and TurningAngle: %f", velocityNow, preError);
    distance = velocityNow * timeDiff;
    now.reset();

    int maxBoth = max(maxV[0], maxV[1]);
    int i = left.size() - 11;
    float error = preError;

    while (left[i] == DetectLane::null && right[i] == DetectLane::null) {
        i--;
        if (i < 0) return;
    }
    if ( maxBoth > 35 ) {
        if (left[i] != DetectLane::null && right[i] !=  DetectLane::null)
        {
            error = errorAngle((left[i] + right[i]) / 2);
        } 
        else if (right[i] != DetectLane::null && maxV[1] > 30)
        {
            error = errorAngle(right[i] - Point(laneWidth / 2, 0));
        }
        else
        {
            error = errorAngle(left[i] + Point(laneWidth / 2, 0));
        }
    } else {
        switch( goodChoise ) {
            case chooseLeft:
                if (left[i] != DetectLane::null) {
                    error = errorAngle(left[i] + Point(laneWidth / 2, 0));
                    break;
                }
            case chooseRight:
                if (right[i] != DetectLane::null) {
                    error = errorAngle(right[i] - Point(laneWidth / 2, 0));
                    break;
                }  // if chose right but can't right!
                else if (left[i] != DetectLane::null) {
                    error = errorAngle(left[i] + Point(laneWidth / 2, 0));
                    break;
                }
            case chooseStraight:
            default:
                if (left[i] != DetectLane::null && right[i] !=  DetectLane::null)
                {
                    error = errorAngle((left[i] + right[i]) / 2);
                } 
                else if (right[i] != DetectLane::null && maxV[1] > 30)
                {
                    error = errorAngle(right[i] - Point(laneWidth / 2, 0));
                }
                else
                {
                    error = errorAngle(left[i] + Point(laneWidth / 2, 0));
                }
        }
    }

    if( maxBoth > 50 ) {
        velocity = maxVelocity;
    } else if ( maxBoth > 30 ) {
        velocity = maxVelocity * 0.9;
    } else if ( maxBoth > 20 ) {
        velocity = maxVelocity * 0.7;
    } else if ( maxBoth > 17 ) {
        velocity = maxVelocity * 0.4;
    } else {
        velocity = minVelocity;
    }

    velocity = min((double)DetectLane::carMaxSpeed, velocity);

    std_msgs::Float32 angle;
    std_msgs::Float32 speed;

    preError = error;
    angle.data = error;
    speed.data = velocity;

    steer_publisher.publish(angle);
    speed_publisher.publish(speed);
} 
