#pragma once
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

class Node
{
public:
	Node(void);
	Node(cv::Point);
	~Node(void);

	cv::Point pt;
	
	std::vector<Node*> edge;
	int prev;
	int dist;
	int* id;
};

