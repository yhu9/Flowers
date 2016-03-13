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

	void init(char*);
	//vector<Point> addCircle(int setid,int cid, vector<Point> pointSet);

	void drawCircle();     
        void drawCircle2();
private:
	//Variables
	vector<vector<Point> > contourSet;
	vector<Point*> pSet;
	vector<Node*> maps;
	Mat shapes;
        Mat boundCircle;
        Mat boundRect;
	vector<int> features;
	vector<Circle> circles;
	MyTools tools;

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

