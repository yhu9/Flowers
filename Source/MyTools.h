#include <opencv2\opencv.hpp>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <deque>
#include <stdlib.h>
#include "Circle.h"
#include "Node.h"

class MyTools
{
public:
	MyTools();
	~MyTools();

	void outlineContour(std::vector<std::vector<cv::Point>>,cv::Mat);
	double findAngleOfRay(cv::Point, cv::Point);
	cv::Vec2i findDirectionOfRay(cv::Point start,cv::Point end);
	double findDistance(cv::Point v1, cv::Point v2);
	cv::Point findMidPoint(cv::Point v1,cv::Point v2);

	//Returns either a point or the index of the closest point to a given point
	cv::Point findClosestPoint(int,int,std::vector<cv::Point>);
	cv::Point findClosestPoint(cv::Point center, std::vector<Node> contour);
	int findPointId(cv::Point pt,std::vector<cv::Point> pointSet);
	int findClosestPoint(cv::Point,std::vector<cv::Point>);
	double findSlope(cv::Point, cv::Point);

	cv::Point findCentroid(std::vector<cv::Point*> pointSet);
	cv::Point findCentroid(std::vector<int> id, std::vector<Node*> map);

	std::vector<cv::Point> findIntersections(cv::Mat img1, cv::Mat img2);
	
	//For checking things about image placement
	bool findPtInShape(cv::Point,std::vector<cv::Point>);
	bool doesIntersect(cv::Mat img1, cv::Mat img2);

	//For pre-processing the image
	void closeOpenContours(cv::Mat,std::vector<std::vector<cv::Point>>);
	std::vector<std::vector<cv::Point>> removeOutliers(std::vector<std::vector<cv::Point>>);
	std::vector<cv::Point*> reduction(std::vector<cv::Point*> set, int reductionSize);
	void closeImageDijkstra(std::vector<Node*> map, cv::Mat img);
	void closeImage(std::vector<Node*> map, cv::Mat img);

	//Map Functions
	int findNodeId(cv::Point pt, std::vector<Node*> map);
	std::vector<Node*> mapNodesDijkstra(int srcid, std::vector<Node*> map);
	std::vector<Node*> MyTools::mapNodesDijkstraLongest(int srcid, std::vector<Node*> set);
	std::vector<Node*> mapNodesPrim(std::vector<Node*> map);
	std::deque<int> findClosestPath(int src, int dest, std::vector<Node*> map);
	std::deque<int> findAPath(int src, int dest, std::vector<Node*> map);

	void traversePath(int src,int prev, int dest, std::vector<Node*> map);
	
	//work in progress
	std::vector<int*> findAllPaths(int src, int dest, std::vector<Node*> maps);

	//UNION,INTERSECTION,SUBTRACTION of Sets of points
	void unionOfPoints(std::vector<cv::Point>,std::vector<cv::Point>,std::vector<cv::Point>);
	void intersectionOfPoints(std::vector<cv::Point>,std::vector<cv::Point>,std::vector<cv::Point>);
	void subtractionOfPoints(std::vector<cv::Point>,std::vector<cv::Point>,std::vector<cv::Point>);
	std::vector<cv::Point> randomize(std::vector<cv::Point> set);
	
	//Complex image processing tools
	cv::Point jitterCircle(cv::Point c, double radius, cv::Mat shape);

};
