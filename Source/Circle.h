#pragma once
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


class Circle
{
public:
												
	std::vector<cv::Point> intersection;		//shows the index of the circle intersections with the shape
	std::vector<cv::Point> cPoints;

	cv::Mat shape;
	cv::Point center;							//shows the center point of the circle
	double radius;
	double area;

	double calcArea();
	void createPoints();
	cv::Point findPoint(cv::Vec2i ray);

	Circle(void);
	Circle(int,int,double);
	Circle(cv::Point, double);
	~Circle(void);
};

