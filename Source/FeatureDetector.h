#pragma once
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "MyTools.h"


class FeatureDetector
{
public:
	FeatureDetector(void);
	~FeatureDetector(void);
	void test();
	void exit();

	void FeatureDetector::init(char*);
	std::vector<cv::Point> addCircle(int setid,int cid, std::vector<cv::Point> pointSet);

	void drawCircle2();
	
private:
	std::vector<std::vector<cv::Point>> contourSet;
	std::vector<cv::Point*> pSet;
	std::vector<Node*> maps;
	cv::Mat shapes;
	std::vector<int> features;
	std::vector<Circle> circles;
	MyTools tools;

	void traverseMap();
	Circle insertCircle(cv::Mat shape, std::vector<int> set);
	Circle insertCircle(cv::Mat shape, std::deque<int> set);
	cv::Mat drawSet(std::vector<int> set);
	cv::Mat drawSet(std::deque<int> set);
	void separateShape(std::vector<int> ids);
};

