#include "detectlane.h"
#include "stdio.h"

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

DetectLane::DetectLane() {
    cvCreateTrackbar("LowH", "Threshold", &minThreshold[0], 179);
    cvCreateTrackbar("HighH", "Threshold", &maxThreshold[0], 179);

    cvCreateTrackbar("LowS", "Threshold", &minThreshold[1], 255);
    cvCreateTrackbar("HighS", "Threshold", &maxThreshold[1], 255);

    cvCreateTrackbar("LowV", "Threshold", &minThreshold[2], 255);
    cvCreateTrackbar("HighV", "Threshold", &maxThreshold[2], 255);

    cvCreateTrackbar("Shadow Param", "Threshold", &shadowParam, 255);

    cvCreateTrackbar("Car max speed", "Threshold", &carMaxSpeed, 50);
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

    Mat birdView, lane, debugView;
    birdView = Mat::zeros(img.size(), CV_8UC3);
    debugView = Mat::zeros(img.size(), CV_8UC3);
    lane = Mat::zeros(img.size(), CV_8UC3);

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
        }
    }

    for (int i = 1; i < rightLane.size(); i++) {
        if (rightLane[i] != null) {
            circle(lane, rightLane[i], 1, Scalar(255,0,0), 2, 8, 0 );
        }
    }

    imshow("Lane Detect", lane);
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


void DetectLane::detectLeftRight(const vector<vector<Point> > &points)
{
    static vector<Point> lane1, lane2;
    lane1.clear();
    lane2.clear();
    this->otherLane.clear();

    leftLane.clear();
    rightLane.clear();
    for (int i = 0; i < BIRDVIEW_HEIGHT / slideThickness; i ++)
    {
        leftLane.push_back(null);
        rightLane.push_back(null);
    }

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
                for (int k = 0; k < points[i + m].size(); k ++)
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

    for (int i = 0; i < points.size(); i++)
    {
        for (int j = 0; j < points[i].size(); j++)
        {
            if (pointMap[i][j] > max2 && (i != posMax.x || j != posMax.y) && postPoint[i][j] == -1)
            {
                max2 = pointMap[i][j];
                posMax2 = Point2i(i, j);
            }
        }
    }

    // for (int i = 0; i < points.size(); i++) {
    //     printf("%3d %2d : ", (int)points[i].size(), i);
    //     for (int j = 0; j < points[i].size(); j++) {
    //         printf(" %3d-%2d; ", pointMap[i][j], points[i][j].x);
    //     }
    //     printf("\n");
    // }

    if (max1 == -1) return;

    int lastX1, lastX2;
    while (max1 >= 1)
    {
        lane1.push_back(points[posMax.x][posMax.y]);
        if (max1 == 1) break;

        posMax.y = prePoint[posMax.x][posMax.y];
        posMax.x += 1;
        
        max1--;
    }

    while (max2 >= 1)
    {
        lane2.push_back(points[posMax2.x][posMax2.y]);
        if (max2 == 1) break;

        posMax2.y = prePoint[posMax2.x][posMax2.y];
        posMax2.x += 1;

        max2--;
    }

//////////////////////////////////////////////////////////
    vector< vector< Point > > stain;
    vector< vector< Point > > stain2;
    vector< Point > stainStart;
    vector< Point > stainEnd;
    int tmpMax = 0;
    for (int i = 0; i < points.size()-9; i++) {
        for (int j = 0; j < points[i].size(); j++) {
            if ( pointMap[i][j] > 6 ) {
                vector< Point > newStain;
                tmpMax = pointMap[i][j];
                Point tmpPoint = Point2i(i, j);
                stainEnd.push_back(points[tmpPoint.x][tmpPoint.y]);
                newStain.push_back(points[tmpPoint.x][tmpPoint.y]);
                while( tmpMax > 2 ) {
                    // printf("%d : %d -> ", tmpPoint.x, tmpPoint.y);

                    tmpPoint.y = prePoint[tmpPoint.x][tmpPoint.y];
                    tmpPoint.x++;
                    tmpMax--;
                    newStain.push_back(points[tmpPoint.x][tmpPoint.y]);
                    pointMap[tmpPoint.x][tmpPoint.y] = 6;
                }
                stainStart.push_back(points[tmpPoint.x][tmpPoint.y]);
                stain.push_back(newStain);
                // printf("\n");
            }
        }

        // for (int j = points[i].size() - 1; i >= 0; j--) {
        //     if ( pointMap[i][j] > 5 ) {
        //         vector< Point > newStain;
        //         tmpMax = pointMap[i][j];
        //         Point tmpPoint = Point2i(i, j);
        //         stainEnd.push_back(points[tmpPoint.x][tmpPoint.y]);
        //         newStain.push_back(points[tmpPoint.x][tmpPoint.y]);
        //         while( tmpMax > 2 ) {
        //             // printf("%d : %d -> ", tmpPoint.x, tmpPoint.y);

        //             tmpPoint.y = prePoint[tmpPoint.x][tmpPoint.y];
        //             tmpPoint.x++;
        //             tmpMax--;
        //             newStain.push_back(points[tmpPoint.x][tmpPoint.y]);
        //             pointMap[tmpPoint.x][tmpPoint.y] = 5;
        //         }
        //         stainStart.push_back(points[tmpPoint.x][tmpPoint.y]);
        //         stain.push_back(newStain);
        //         // printf("\n");
        //     }
        // }
    }

    // printf("leng of Strain : %d   => ", (int) stain.size());
    // for(int i = 0 ; i < stain.size(); i++) {
    //     printf("%d : leng %2d = ", i, (int)stain.size());
    //     for(int j = 0; j < stain[i].size(); j++){
    //         printf("%d > ", stain[i][j].x);
    //     }
    //     printf("\n");
    // }
    // for(int i = 0 ; i < stainStart.size(); i++) {
    //     printf(" %2d:%3d & ", stainStart[i].x, stainStart[i].y);
    // }
    // printf("\n");
    // for(int i = 0 ; i < stainStart.size(); i++) {
    //     printf(" %2d:%3d & ", stainEnd[i].x, stainEnd[i].y);
    // }
    // printf("\n");

    int idRightLane = -1, idLeftLane = -1, xRight, xLeft, xRightEnd, xLeftEnd;
    Point start, end;
    for (int i = 0; i < stain.size(); i++) {
        start = stain[i][stain[i].size() - 1];
        end = stain[i][0];
        if (start.y > 200) {
            if (start.x > 120) {
                if (idRightLane == -1 || (start.x < xRight) ||
                    (start.x == xRight && end.x < xRightEnd))
                {
                    idRightLane = i;
                    xRight = start.x;
                    xRightEnd = end.x;
                }
            } else if (start.x < 120) {
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

    // if (idLeftLane == -1 && idRightLane == -1) {
    //     for (int i = 0; i < stainStart; i++) {
    //         if ()
    //     }
    // }

    if(idLeftLane != -1) {
        this->maxL = stain[idLeftLane].size();
        for (int i = 0; i < stain[idLeftLane].size(); i++) {
            leftLane[floor(stain[idLeftLane][i].y / slideThickness)] = stain[idLeftLane][i];
        }
    } else {
        this->maxL = 0;
    }
    if(idRightLane != -1) {
        this->maxR = stain[idRightLane].size();
        for (int i = 0; i < stain[idRightLane].size(); i++) {
            rightLane[floor(stain[idRightLane][i].y / slideThickness)] = stain[idRightLane][i];
        }
    } else {
        this->maxR = 0;
    }

    if(idRightLane == -1 && idLeftLane != -1) {

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
}

std::vector<int> DetectLane::getMax(){
    std::vector<int> v;
    v.push_back(this->maxL);
    v.push_back(this->maxR);
    return v;
}
