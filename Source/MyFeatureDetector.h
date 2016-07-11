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

	///////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

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
        
		MyTools tools;
public:
		vector<double> features;

        double extractAreaOfCircles();          //complete
        double extractAreaOfBoundRect();		//complete
        double extractAreaOfBoundHull();		//complete
        double extractNumberOfHullNodes();		//complete
        double extractNumberOfSkeletonNodes();	//complete
        double extractHullLength();				//complete
        double extractSkeletonLength();			//complete
        double extractAverageDegree();			//complete
        double extractAverageAngle();			//complete
        double extractAreaOfFirstCircle();		//complete
        double extractDegreeOfFirstCircle();	//complete
		void addFeature(double);
		vector<Circle> getCircles();


        int feature_averageDistanceOfImageNodesInSpace();
        int feature_imageDiameter();

        int feature_averageAreaOfCircles();
        int feature_numberOfClusters();
        int feature_skeletonDiameter();
        int feature_skeletonModularity();
        int feature_skeletonLength();

		///////////////////////////////////////////////////////////////////////////////////////////
		void drawSkeleton(int fudge); 
		void drawCircle();     
		void drawCircle2();
		void printSkeletonMap();
        void showShape(int time);
		///////////////////////////////////////////////////////////////////////////////////////////
private:
	//Functions
	Circle insertCircle(Mat shape, Point seed);
	Circle insertCircle2(Mat shape, Circle prev, Point seed);
    Mat drawSet(vector<int> set);
	Mat drawSet(deque<int> set);
	//void separateShape(vector<int> ids);
};

