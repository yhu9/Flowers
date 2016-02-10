#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

class Circle
{
public:
												
	vector<Point> intersection;		//shows the index of the circle intersections with the shape
	vector<Point> cPoints;

	Mat shape;
	Point center;							//shows the center point of the circle
	double radius;
	double area;

	double calcArea();
	void createPoints();
	Point findPoint(Vec2i ray);

	Circle(void);
	Circle(Point,double,Mat);
        Circle(int,int,double);
	Circle(Point, double);
	~Circle(void);
};

