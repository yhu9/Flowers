#pragma once
#include <iostream>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <queue>
#include <stdlib.h>
#include "MyTools.h"
#include <time.h>
using namespace std;
using namespace cv;

class MyFeatureDetector
{
public:
	MyFeatureDetector(void);
	~MyFeatureDetector(void);
	void test(char*);
	void exit();

	bool init(char*);
	//vector<Point> addCircle(int setid,int cid, vector<Point> pointSet);

	void drawCircle();     
        void drawCircle2();
private:
	//Sets
	vector<vector<Point> > contourSet;      //contourSet holds the points as findContours found it
	vector<Point*> pSet;                    //pSet holds the points the hull indexes
        vector<int> hull;                       //unfortunately I don't have much of a choice but to make it a set of indexes.
        vector<Circle> circles;

        //final shapes
	Mat shapes;
        Mat skeleton;
        Mat boundCircle;
        Mat boundRect;
        Mat boundHull;

        //structures
        vector<Node*> maps;                     //map of pSet
        vector<Node*> skeletonMap;           //map of circles
                                                //I could make a map for hull also but its just a single linked list
        
        //output variables
	vector<int> features;                   //features of the image based on structures and other criteria

        //Objects we wish to use for the class
	MyTools tools;
private://private functions
        int feature0_areaOfCircles();
        int feature1_areaOfBoundRect();
        int feature2_areaOfBoundHull();
        int feature_numberOfNodesInHull();
        int feature_distanceOfCenterOfCircleToCenterOfImage();

        int feature_imageLength();
        int feature_imageArea();
        int feature_numberOfNodesInImage();
        int feature_averageDistanceOfImageNodesInSpace();
        int feature_imageDiameter();

        int feature3_numberOfCircles();
        int feature_degreeOfFirstCircle();
        int feature_averageAreaOfCircles();
        int feature_numberOfClusters();
        int feature_skeletonDiameter();
        int feature_skeletonAverageDegree();
        int feature_skeletonAverageAngle();
        int feature_skeletonModularity();
        int feature_skeletonLength();
public:
	//Functions
	//void traverseMap();
	//Circle insertCircle(Mat shape, vector<int> set);
	Circle insertCircle(Mat shape, Point seed);
	Circle insertCircle2(Mat shape, Circle prev, Point seed);
        void showShape();
        void drawSkeleton(int fudge); 
        Mat drawSet(vector<int> set);
	Mat drawSet(deque<int> set);
	//void separateShape(vector<int> ids);
};

