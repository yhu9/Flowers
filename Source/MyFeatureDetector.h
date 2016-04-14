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
        vector<Point> pointSet;
        //final shapes
	Mat shapes;
        Mat skeleton;
        Mat boundCircle;
        Mat boundHull;
        Mat boundRect;

        //final shape structs
        Circle bCirc;
        Rect bRect;
        vector<int> hull;
        vector<Circle> circles;
        
        //structures
        vector<Node*> maps;                     //map of pSet
        vector<Node*> skeletonMap;           //map of circles
                                                //I could make a map for hull also but its just a single linked list
        
        //output variables
	vector<double> features;                   //features of the image based on structures and other criteria

        //Objects we wish to use for the class
	MyTools tools;
public://private functions
        double extractAreaOfCircles();          //complete
        double extractAreaOfBoundRect();
        double extractAreaOfBoundHull();
        double extractNumberOfHullNodes();
        double extractNumberOfSkeletonNodes();
        double extractHullLength();
        double extractSkeletonLength();
        double extractAverageDegree();
        double extractAverageAngle();
        double extractAreaOfFirstCircle();
        double extractDegreeOfFirstCircle();


        int feature_averageDistanceOfImageNodesInSpace();
        int feature_imageDiameter();

        int feature_degreeOfFirstCircle();
        int feature_averageAreaOfCircles();
        int feature_numberOfClusters();
        int feature_skeletonDiameter();
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

