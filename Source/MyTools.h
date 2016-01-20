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
	double findAngleOfRay(Point, Point);
	Vec2i findDirectionOfRay(Point start,Point end);
	double findDistance(Point v1, Point v2);
	Point findMidPoint(Point v1,Point v2);

	//Returns either a point or the index of the closest point to a given point
	Point findClosestPoint(int,int,vector<Point>);
	Point findClosestPoint(Point center, vector<Node> contour);
	int findPointId(Point pt,vector<Point> pointSet);
	int findClosestPoint(Point,vector<Point>);
	double findSlope(Point, Point);

	Point findCentroid(vector<Point*> pointSet);
	Point findCentroid(vector<int> id, vector<Node*> map);

	vector<Point> findIntersections(Mat img1, Mat img2);
	
	//For checking things about image placement
	bool findPtInShape(Point,vector<Point>);
	bool doesIntersect(Mat img1, Mat img2);

	//For pre-processing the image
	void closeOpenContours(Mat,vector<vector<Point> >);
	vector<vector<Point> > removeOutliers(vector<vector<Point> >);
	vector<Point*> reduction(vector<Point*> set, int reductionSize);
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

};
