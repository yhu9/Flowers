#pragma once
#include <iostream>
#include <fstream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdlib.h>

using namespace std;
using namespace cv;
class Node
{
public:
	Node(void);
	Node(Point);
	~Node(void);

	Point pt;
	
	vector<Node*> edge;
	int prev;
	int dist;
	int* id;
};

