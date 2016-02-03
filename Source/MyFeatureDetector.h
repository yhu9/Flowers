#pragma once
#include <iostream>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include "MyTools.h"

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
	vector<Point> addCircle(int setid,int cid, vector<Point> pointSet);

	void drawCircle();
	
private:
	//Variables
	vector<vector<Point> > contourSet;
	vector<Point*> pSet;
	vector<Node*> maps;
	Mat shapes;
	vector<int> features;
	vector<Circle> circles;
	MyTools tools;

public:
	//Functions
	void traverseMap();
	Circle insertCircle(Mat shape, vector<int> set);
	Circle insertCircle(Mat shape, Point seed);
	Mat drawSet(vector<int> set);
	Mat drawSet(deque<int> set);
	void separateShape(vector<int> ids);
};

