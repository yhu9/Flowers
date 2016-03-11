#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <deque>
#include <stdlib.h>
#include "Circle.h"
#include "Node.h"

using namespace std;
using namespace cv;

class MyTools
{
public:
	MyTools();
	~MyTools();


	void outlineContour(vector<vector<Point> >,Mat);
	//Math Tools
	
	//Vector tools
	Vec2i makeVector(Point start, Point end);
	Vec2d normalize(Vec2i vector);
	Vec2i simplifyVector(Vec2i vector);

	//Geometry tools
	double findAngleOfRay(Point, Point);
	double findDistance(Point v1, Point v2);
	Point findMidPoint(Point v1,Point v2);
	double findSlope(Point, Point);

	//Returns either a point or the index of the closest point to a given point
	Point findClosestPoint(int,int,vector<Point>);
	Point findClosestPoint(Point center, vector<Node> contour);
	int findPointId(Point pt,vector<Point> pointSet);
	int findClosestPoint(Point,vector<Point>);
	
	Point findCentroid(vector<Point*> pointSet);
	Point findCentroid(vector<int> id, vector<Node*> map);

		
	//For checking things about image placement
	bool findPtInShape(Point,vector<Point>);	//does not work
	bool doesIntersect(Mat img1, Mat img2);		//checks to see if img1 and img2 intersect
	bool isInside(Point pt, Mat img);		//checks to see if pt is inside img
	bool isInside2(Point pt, Mat img);
        bool isOn(Point pt, Mat img);			//checks to see if pt is on img
	bool isInsideAndNotOn(Point pt, Mat img);		//checks to see if pt is inside and Not on img
	vector<Point> findIntersections(Mat img1, Mat img2);	//finds the intersections of img1 and img2
	

	//For pre-processing the image
	void closeOpenContours(Mat,vector<vector<Point> >);
	vector<vector<Point> > removeOutliers(vector<vector<Point> >);
	vector<Point*> reduction(vector<Point*> set, int reductionSize);
	vector<Point> reduction(vector<Point> set, int reductionSize);
        void closeImageDijkstra(vector<Node*> map, Mat img);
	void closeImage(vector<Node*> map, Mat img);

	//Map Functions
	int findNodeId(Point pt, vector<Node*> map);
	vector<Node*> mapNodesDijkstra(int srcid, vector<Node*> map);
	vector<Node*> mapNodesDijkstraLongest(int srcid, vector<Node*> set);
	vector<Node*> mapNodesPrim(vector<Node*> map);
	deque<int> findClosestPath(int src, int dest, vector<Node*> map);
	deque<int> findAPath(int src, int dest, vector<Node*> map);

	void traversePath(int src,int prev, int dest, vector<Node*> map);
	
	//work in progress
	vector<int*> findAllPaths(int src, int dest, vector<Node*> maps);

	//UNION,INTERSECTION,SUBTRACTION of Sets of points
	void unionOfPoints(vector<Point>,vector<Point>,vector<Point>);
	void intersectionOfPoints(vector<Point>,vector<Point>,vector<Point>);
	void subtractionOfPoints(vector<Point>,vector<Point>,vector<Point>);
	vector<Point> randomize(vector<Point> set);
	
	//Complex image processing tools
	Point jitterCircle(Point c, double radius, Mat shape);
        Point jitterCircle(Point c, double radius, Point c2, double radius2, Mat shape);
};
