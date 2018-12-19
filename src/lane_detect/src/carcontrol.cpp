#include "carcontrol.h"
// #include "detectlane.h"

CarControl::CarControl()
{
    carPos.x = 120;
    carPos.y = 300;
    steer_publisher = node_obj1.advertise<std_msgs::Float32>("Team1_steerAngle",10);
    speed_publisher = node_obj2.advertise<std_msgs::Float32>("Team1_speed",10);
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
    return atan(dx / dy) * 180 / pi;
}

void CarControl::driverCar(const vector<Point> &left, const vector<Point> &right, const vector<int> &maxV)
{
    int maxBoth = max(maxV[0], maxV[1]);
    int i = left.size() - 11;
    float error = preError;
    while (left[i] == DetectLane::null && right[i] == DetectLane::null) {
        i--;
        if (i < 0) return;
    }
    if ( maxBoth > 30 ) {
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
                error = 0;
        }
    }

    int velocity = 0;
    if( maxBoth > 50 ) {
        velocity = maxVelocity;
    } else if ( maxBoth > 30 ) {
        velocity = maxVelocity * 0.9;
    } else if ( maxBoth > 20 ) {
        velocity = maxVelocity * 0.7;
    } else if ( maxBoth > 15 ) {
        velocity = maxVelocity * 0.4;
    } else {
        velocity = minVelocity;
    }

    std_msgs::Float32 angle;
    std_msgs::Float32 speed;

    angle.data = error;
    speed.data = velocity;

    steer_publisher.publish(angle);
    speed_publisher.publish(speed);
} 
