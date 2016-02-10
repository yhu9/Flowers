#include "MyTools.h"

MyTools::MyTools(){}
MyTools::~MyTools(){}

using namespace cv;
using namespace std;

//Some useful trigonometry and geometry tools
Point MyTools::findClosestPoint(int x, int y, vector<Point> contour)
{
	cout << "MyTools::findClosestPoint" << endl;
	double smallestDistance = -1, tmp = 0;
	Point closestPoint;
	for(unsigned i = 0; i < contour.size(); i++)
	{
		tmp = sqrt(pow((double)(contour[i].x - x),2) + pow((double)(contour[i].y - y),2));
		if(tmp < smallestDistance || smallestDistance == -1)
		{
			smallestDistance = tmp;
			closestPoint = contour[i];
		}
	}
	return closestPoint;
}

Point MyTools::findClosestPoint(Point center, vector<Node> contour)
{
	cout << "MyTools::findClosestPoint" << endl;
	double smallestDistance = -1, tmp = 0;
	Point closestPoint;
	for(unsigned i = 0; i < contour.size(); i++)
	{
		tmp = sqrt(pow((double)(contour[i].pt.x - center.x),2) + pow((double)(contour[i].pt.y - center.y),2));
		if(tmp < smallestDistance || smallestDistance == -1)
		{
			smallestDistance = tmp;
			closestPoint = contour[i].pt;
		}
	}
	return closestPoint;
}
int MyTools::findClosestPoint(Point point, vector<Point> contour)
{
	cout << "MyTools::findClosestPoint" << endl;
	double smallestDistance = -1, tmp = 0;
	int closestPoint = -1;
	for(unsigned i = 0; i < contour.size(); i++)
	{
		tmp = sqrt(pow((double)(contour[i].x - point.x),2) + pow((double)(contour[i].y - point.y),2));
		if(tmp < smallestDistance || smallestDistance == -1)
		{
			smallestDistance = tmp;
			closestPoint = i;
		}
	}
	return closestPoint;
}
Point MyTools::findMidPoint(Point v1, Point v2)
{
	cout << "MyTools::findMidPoint" << endl;
	Point midPoint;
	double x,y;
	x = (v1.x + v2.x)/2;
	y = (v1.y + v2.y)/2;
	midPoint = Point(x,y);
	return midPoint;
}
void MyTools::outlineContour(vector<vector<Point> > contour, Mat img)
{
	cout << "MyTools::outlineContour" << endl;
	Point prev = contour[0][0];
	for(unsigned i = 0; i < contour.size(); i++)
	{
		for(unsigned j = 1; j < contour[i].size() - 1; j++)
		{
			line(img,prev,contour[i][j],Scalar(255,255,255),2,8);
			prev = Point(contour[i][j].x,contour[i][j].y);
		}
	}
}
double MyTools::findDistance(Point v1, Point v2)
{
	double distance = -1;

	distance = sqrt(pow((double)(v2.x - v1.x), 2) + pow((double)v2.y - (double)v1.y,2));
	return distance;
}
double MyTools::findAngleOfRay(Point start, Point end)
{
  cout << "MyTools::findAngleOfRay" << endl;
	double theta = 0;
	if(end.x == start.x)
		return theta;
	else
		theta = atan((double)(end.y - start.y) / (double)(end.x - start.x));

	return theta;
}
Vec2i MyTools::makeVector(Point start,Point end)
{
	cout << "MyTools::findDirectionOfRay" << endl;
	Vec2i vec(end.x - start.x, end.y - start.y);
	return vec;
}
double MyTools::findSlope(Point v1, Point v2)
{
	cout << "MyTools::findSlope" << endl;
	double slope = (double)(v2.y - v1.y)/(double)(v2.x - v1.x);
	return slope;
}

int MyTools::findPointId(Point pt, vector<Point> pointSet)
{
	cout << "MyTools::findPointId" << endl;
	bool ptFound = false;
	int id = -1;
	for(unsigned i = 0; i < pointSet.size() && !ptFound; i++)
	{
		if(pt.x == pointSet[i].x && pt.y == pointSet[i].y)
		{
			id = i;
			ptFound = true;
		}
	}

	return id;
}

Point MyTools::findCentroid(vector<Point*> pointSet)
{
	cout << "MyTools::findCentroid" << endl;
	double x = 0;
	double y = 0;
	unsigned i = 0;
	for(i = 0; i < pointSet.size(); i++)
	{
		x += pointSet[i]->x;
		y += pointSet[i]->y;
	}

	x = x/i;
	y = y/i;

	return Point(x,y);
}
Point MyTools::findCentroid(vector<int> id, vector<Node*> map)
{
	cout << "MyTools::findCentroid" << endl;
	double x = 0;
	double y = 0;
	unsigned i = 0;
	for(i = 0; i < id.size(); i++)
	{
		x += map[id[i]]->pt.x;
		y += map[id[i]]->pt.y;
	}

	x = x/i;
	y = y/i;

	return Point(x,y);
}

////////////////////////////////////////////////////////////////////////////////
//Map functions
int MyTools::findNodeId(Point pt, vector<Node*> map)
{
	cout << "MyTools::findNodeId" << endl;
	//We try to look for the id of the closest node in the map that matches the id.
	//The max distance the closest node can be is 19
	int dist = 3;
	for(unsigned j = 0; j < 20; j++,dist++)
	{
		for(unsigned i = 0; i < map.size(); i++)
		{
			if(findDistance(pt, map[i]->pt) < dist)
				return i;
		}
	}
	return -1;
}

//Finds the path as a vector of ints from the src to the destination in the map. Applies dijkstra
//to do this. We also bother to add all nodes that aren't part of the closest path but also in between 
//the src and dest paths. 
deque<int> MyTools::findClosestPath(int src, int dest, vector<Node*> map)
{
	cout << "MyTools::findClosestPath" << endl;

	mapNodesDijkstra(src,map);
	deque<int> path;
	vector<int> node3;
	path.push_front(dest);
	Node tmp = *map[dest];

	//Add all nodes in the shortest path from src to dest
	//in order of first to last
	do
	{
		if(map[tmp.prev]->edge.size() >= 3)
			node3.push_back(tmp.prev);

		path.push_front(tmp.prev);
		tmp = *map[tmp.prev];

	}while(tmp.prev != src);
	path.push_back(src);
	//Go through all nodes in the map and check all nodes that start from a 1 edge node in the map and 
	//check if it leads to a 3 edge node in the path.
	for(unsigned i = 0; i < map.size(); i++)
	{
		//Check if node is a 1 edge node
		if(map[i]->edge.size() == 1)
		{
			bool check = false;
			deque<int> tmpPath;
		
			tmp = *map[i];
			tmpPath.push_front(i);

			//Follow the path of the 1 edge node all the way to a 3 edge node
			//add the nodes into a tmpPath
			bool insideCircle = false;
			do
			{
				if(tmp.prev == -1)
					insideCircle = true;
				else
				{
					int currentNodeId = tmp.prev;

					if(map[tmp.prev]->edge.size() < 3)
						tmpPath.push_front(tmp.prev);
					tmp = *map[tmp.prev];

					if(tmp.edge.size() >= 3)
					{
						for(unsigned a = 0; a < node3.size(); a++)
						{
							if(currentNodeId == node3[a])
								check = true;
						}
					}
				}
			}while(tmp.edge.size() < 3 && insideCircle == false);

			//If the 3 edge node is in the original path (tmpPath U path)
			if(check)
			{
				for(unsigned tmpPathID = 0; tmpPathID < tmpPath.size(); tmpPathID++)
				{
					path.push_back(tmpPath[tmpPathID]);
				}
			}
		}
	}

	return path;
}
void MyTools::traversePath(int src, int prev, int dest, vector<Node*> map)
{
	cout << "MyTools::traversePath" << endl;
	/*for(unsigned i = 0; i < map[src]->edge.size(); i++)
	{
		if(map[src]->edge[i]->id == prev)
		{
		}
		else if(map[src]->edge[i]->id = dest)
		{
		}
		else
		{
			cout << 
			traversePath(map[src]->edge[i]->id,src,dest,map);
		}
	}*/
}

//Work in progress
vector<int*> MyTools::findAllPaths(int src, int dest, vector<Node*> map)
{
	cout << "MyTools::findAllPaths" << endl;
	int* path = new int[map.size() - 1];
	vector<int*> paths;
	bool* visited = new bool[map.size() - 1];

	for(unsigned i = 0; i < map.size() -1; i++)
		visited[i] = false;


	//traversePath(map,src,dest,visited,path,pathid,paths);

	delete [] visited;
	delete [] path;
	return paths;
}
///////////////////////////////////////////////////////////////////////////////////////////

vector<Node*> MyTools::mapNodesDijkstra(int srcid, vector<Node*> set)
{
	cout << "MyTools::mapNodesDijkstra" << endl;
	vector<int> myQ;

	for(unsigned i = 0; i < set.size(); i++)
	{
		
		set[i]->dist = 1000000000;
		set[i]->prev = -1;

		myQ.push_back(i);
	}

	set[srcid]->dist = 0;
	
	//All nodes initially in the q
	int id;
	while(!myQ.empty())
	{
		//Find the smallest distance from source
		//remove the shortest path node from myQ
		int smallest = 1000000000;
		for(unsigned i = 0; i < myQ.size(); i++)
		{
			if(set[myQ[i]]->dist < smallest)
			{
				smallest = set[myQ[i]]->dist;
				id = i;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		int length = 0;
		
		for(unsigned i = 0; i < set[myQ[id]]->edge.size(); i++)
		{
			//For each adjacent node to set[myQ[id]]
			//Update dist <- length
			length = set[myQ[id]]->dist + findDistance(set[myQ[id]]->edge[i]->pt,set[myQ[id]]->pt);
				
			if(length < set[myQ[id]]->edge[i]->dist)
			{
				set[myQ[id]]->edge[i]->dist = length;
				set[myQ[id]]->edge[i]->prev = myQ[id];
			}
		}
		myQ.erase(myQ.begin() + id);
	}

	return set;
}
vector<Node*> MyTools::mapNodesPrim(vector<Node*> set)
{
	cout << "MyTools::mapNodesPrim" << endl;
	vector<int> reachedSet;
	vector<int> unreachedSet;

	for(unsigned i = 0; i < set.size(); i++)
	{
		if(i != 0)
			unreachedSet.push_back(i);
	}
	//We can randomize the starting point as well to see if the MST changes.
	reachedSet.push_back(0);


	while(unreachedSet.size() > 0)
	{
		int smallest = 1000000;
		int idi, idj;
		for(unsigned i = 0; i < reachedSet.size(); i++)
		{
			for(unsigned j = 0; j < unreachedSet.size(); j++)
			{
				int dist = findDistance(set[reachedSet[i]]->pt,set[unreachedSet[j]]->pt);
				if(dist < smallest)
				{
					smallest = dist;
					idi = i;
					idj = j;
				}
			}
		}

		set[reachedSet[idi]]->edge.push_back(set[unreachedSet[idj]]);
		set[unreachedSet[idj]]->edge.push_back(set[reachedSet[idi]]);

		reachedSet.push_back(unreachedSet.at(idj));
		unreachedSet.erase(unreachedSet.begin() + idj);
	
	}

	return set;
}


//Set Mathematics
void MyTools::unionOfPoints(vector<Point> set1,vector<Point> set2, vector<Point> output)
{
	cout << "MyTools::unionOfPoints" << endl;
	if(!output.empty())
		output.clear();

	for(unsigned i = 0; i < set1.size(); i++)
		output.push_back(set1[i]);

	for(unsigned i = 0; i < set2.size(); i++)
		output.push_back(set2[i]);

}
void MyTools::intersectionOfPoints(vector<Point> set1,vector<Point> set2, vector<Point> output)
{
	cout << "MyTools::intersectionOfPoints" << endl;
	if(!output.empty())
		output.clear();

	for(unsigned i = 0; i < set1.size() ;i++)
	{
		for(unsigned j = 0; j < set2.size(); j++)
		{
			if(set1[i].x == set2[j].x && set1[i].y == set2[j].y)
				output.push_back(set1[i]);
		}
	}
}
void MyTools::subtractionOfPoints(vector<Point> set1,vector<Point> set2, vector<Point> output)
{
	cout << "MyTools::subtractionOfPoints" << endl;
	if(!output.empty())
		output.clear();
	for(unsigned i = 0; i < set1.size(); i++)
	{
		for(unsigned j = 0; j < set2.size(); j++)
		{
			if(output[i].x == set2[i].x && output[i].y == set2[i].y)
				;
			else
				output.push_back(set1[i]);
		}
	}
}
bool MyTools::doesIntersect(Mat img1, Mat img2)
{
	//cout << "MyTools:doesIntersect" << endl;
	//Should later make sure that all image types are of type CV_8U to make the comparison
	Mat img3;
	img3 = Mat::zeros(img1.size().width,img1.size().height,CV_8U);
	img3 = img1 & img2;

	Point minpt, maxpt;
	minMaxLoc(img3,NULL,NULL,&minpt,&maxpt);

	//Draws the intersection of img1 and img2 on img3 and if it finds
	//a bright spot we say there is an intersection.

	//namedWindow("img1",CV_WINDOW_FREERATIO);
	//namedWindow("img2",CV_WINDOW_FREERATIO);
	//namedWindow("img3",CV_WINDOW_FREERATIO);

	//imshow("img1",img1);
	//imshow("img2",img2);
	//imshow("img3",img3);

	//waitKey();

	if(minpt.x == maxpt.x && minpt.y == maxpt.y)
		return false;
	else
		return true;
}

//Tools to transform shape or point post processing
Point MyTools::jitterCircle(Point c, double radius, Mat shape)
{
	Mat circle_img;
	int x = c.x;
	int y = c.y;
	circle_img = Mat::zeros(shape.size(),CV_8U);
	//imshow("",shape);
	//waitKey();
	
	//Trial Number2
	for(double radian = 0; radian < 6.2; radian+=0.1)
	{
		c.x = x + 5 * cos(radian);
		c.y = y + 5 * sin(radian);

		circle(circle_img,c,radius,Scalar(255,255,255),1,8);
		if(doesIntersect(shape,circle_img))
		{
				c.x = x;
				c.y = y;
		}
		else
		{
			//circle(shape,c,radius,Scalar(255,255,255),2,8);
			//imshow("",shape);
			//waitKey();
		
			return c;
		}

		circle_img = Mat::zeros(circle_img.size(),CV_8U);
	}

	return c;
}
vector<Point> MyTools::findIntersections(Mat img1, Mat img2)
{
	vector<Point> intersectPoints;
	Mat intersection;
	intersection = Mat::zeros(img1.size(),CV_8U);
	intersection = img1 & img2;
	Point minpt, maxpt;

	do
	{
		minMaxLoc(intersection,NULL,NULL,&minpt,&maxpt);
		if(minpt.x != maxpt.x && minpt.y != maxpt.y)
			intersectPoints.push_back(maxpt);

		circle(intersection, maxpt,40,Scalar(0,0,0),-1,8);

	}while(minpt.x != maxpt.x && minpt.y != maxpt.y);

	//If no intersect points are found then an empty set is returned
	return intersectPoints;
}


//pre-processing
//removeOutliers uses fitEllipse to find a region where the cluster of points are fairly close to each other
void MyTools::closeOpenContours(Mat img, vector<vector<Point> > contour)
{
	cout <<"MyTools::closeOpenContours" << endl;

}
vector< vector<Point> > MyTools::removeOutliers(vector<vector<Point> > contour)
{
	cout << "MyTools::removeOutliers" << endl;

	vector<Point> pointSet;
	for(unsigned i = 0; i < contour.size(); i++)
	{
		for(unsigned j = 0; j < contour[i].size(); j++)
			pointSet.push_back(contour[i][j]);
	}

	if(pointSet.size() > 20)
	{

		cout << pointSet.size() << endl;
	RotatedRect ellipse = fitEllipse(pointSet);

	vector<int> distances;
	for(unsigned i = 0; i < pointSet.size(); i++)
	{
		distances.push_back(MyTools::findDistance(ellipse.center,pointSet[i]));
	}

	sort(distances.begin(),distances.end());
	double q1,q3,IQR;
	q1 = distances[(int)(pointSet.size()/4)];
	q3 = distances[(int)(pointSet.size()/4 * 3)];
	//cout << q1 << "," << q3 << endl;
	IQR = q3-q1;
	
	unsigned i = 0;
	while(i < contour.size())
	{
		unsigned j = 0;
		while(j < contour[i].size())
		{
			if(MyTools::findDistance(ellipse.center,contour[i][j]) > q3 + 1.5*IQR)
			{
				//cout << MyTools::findDistance(ellipse.center,contour[i][j]) << endl;
				contour[i].erase(contour[i].begin() + j);
			}
			else
				j++;
		}
		i++;
	}
	}
	else
	{
		cout << "error: set size is too small" << endl;
	}
	
	return contour;
}
vector<Point*> MyTools::reduction(vector<Point*> set, int reductionSize)
{
	cout << "MyTools::reduction" << endl;
	vector<int> setid;
	Mat drawing;
	
	int x, y, count;
	for(unsigned i = 0; i < set.size(); i++)
	{
		x = 0;
		y = 0;
		count = 0;
		for(unsigned j = 0; j < set.size(); j++)
		{
			if(i == j)
			{}
			else
			{
				if(findDistance(*set[i],*set[j]) < reductionSize)
				{
					count++;
					x += set[j]->x;
					y += set[j]->y;
					setid.push_back(j);
				}
			}
		}

		if(count >= 4)
		{
			sort(setid.begin(),setid.end());
			for(unsigned a = setid.size(); a > 0; a--)
			{
				if((unsigned)setid[a - 1] >= set.size())
				{
					cout << setid[a - 1] << " setid" << endl;
					cout << set.size() << " setsize" << endl;
				}
				else
				{
					delete set[setid[a - 1]];
					set.erase(set.begin() + setid[a - 1]);
				}
			}
			x = x / count;
			y = y / count;
			set.push_back(new Point(x,y));
			setid.clear();
		}
	}
	return set;
}
vector<Point> MyTools::randomize(vector<Point> set)
{
	cout << "MyTools::randomize" << endl;
	vector<Point> randomSet;
	int randid;
	cout << set.size() << endl;
	for(unsigned i = set.size(); i > 0; i--)
	{
		randid = rand() % set.size();
		Point temp = set[randid];
		set.erase(set.begin() + randid);
		randomSet.push_back(temp);
	}
	//cout << randomSet.size() << endl;
	return randomSet;
}

void MyTools::closeImageDijkstra(vector<Node*> set, Mat img) //The image must have Dijkstra run on it first.
{
	cout << "MyTools::closeImageDijkstra" << endl;
	vector<Node> setx;
	//Push all nodes with only 1 edge into setx
	for(unsigned i = 0; i < set.size(); i++)
	{
		if(set[i]->edge.size() == 1 && set[i]->dist != 0)
			setx.push_back(*set[i]);
	}

	int largest = 0;
	unsigned id1 = 0;
	int id2 = 0;
	int closest = 500000;

	for(unsigned i = 0; i < setx.size(); i++)
	{
		if(setx[i].dist > largest)
		{
			largest = setx[i].dist;
			id1 = i;
		}
	}
	
	for(unsigned i = 0; i < setx.size(); i++)
	{
		double dist = findDistance(setx[id1].pt,setx[i].pt);
		if(dist < closest && id1 != i)
		{
			closest = dist;
			id2 = i;
		}
	}

	line(img,setx[id1].pt,setx[id2].pt,Scalar(255,255,0),2,8,0);
}
void MyTools::closeImage(vector<Node*> set, Mat img)
{
	cout << "MyTools::closeImage" << endl;
	//Push back all nodes with only 1 edge into setx
	vector<int> setx;
	vector<int> sety;
	for(unsigned i = 0; i < set.size(); i++)
	{
		if(set[i]->edge.size() == 1)
			setx.push_back(i);
		if(set[i]->edge.size() >= 3)
			sety.push_back(i);
	}

	//erase all nodes that are super close to a node with 3 edges
	for(unsigned i = 0; i < setx.size(); i++)
	{
		for(unsigned j = 0; j < sety.size(); j++)
		{
			if(i >= setx.size())
			{}
			else
			{
				double dist = findDistance(set[setx[i]]->pt,set[sety[j]]->pt);
				if(dist < 10)
				{
					setx.erase(setx.begin() + i);
					j = 0;
				}
			}
		}
	}

	//We do the above step so that we don't find two edges that might happen to be close to a messy area
	//find the indexes of the two closest nodes with one edge
	int id1 = 0;
	int id2 = 0;
	double smallest = 50000;
	for(unsigned i = 0; i < setx.size(); i++)
	{
		for(unsigned j = 0; j < setx.size(); j++)
		{
			if( i == j)
			{
			}
			else
			{
				double dist = findDistance(set[setx[i]]->pt,set[setx[j]]->pt);
				if(dist < smallest)
				{
					smallest = dist;
					id1 = i;
					id2 = j;
				}
			}
		}
	}

	//Connect the two nodes using the indexes.
	set[setx[id1]]->edge.push_back(set[setx[id2]]);
	set[setx[id2]]->edge.push_back(set[setx[id1]]);
	line(img,set[setx[id1]]->pt,set[setx[id2]]->pt,Scalar(255,255,0),2,8,0);
}

bool MyTools::isInside(Point pt, Mat img)
{
	Point tempPoint(pt.x, 0);
	Mat tempImg = Mat::zeros(img.size(), CV_8U);
	line(tempImg, pt, tempPoint, Scalar(255,255,255), 1,8);

	if(findIntersections(tempImg, img).size() % 2 == 1)
		return true;
	
	return false;
}

Vec2d MyTools::normalize(Vec2i vector)
{  
        if(vector[0] == 0 && vector[1] == 0)
        {
            return vector;
        }
	Vec2d normalizedVector;
	double length = sqrt(vector[0] * vector[0] + vector[1] * vector[1]);
	normalizedVector[0] = vector[0]/length;
	normalizedVector[1] = vector[1]/length;

	return normalizedVector;
}

bool MyTools::isOn(Point pt, Mat img)
{
	//if(pt.x < 0 || pt.y < 0){
	//	return false;
	//	cout << "invalid pt gotten" << endl;
	//}
	Scalar intensity = img.at<uchar>(pt);

	return (intensity.val[0] == 255);

}

bool MyTools::isInsideAndNotOn(Point pt, Mat img)
{
	return isInside(pt,img) && !isOn(pt,img);
}

Vec2i MyTools::simplifyVector(Vec2i vector)
{
	Vec2i simplifiedVector;
	unsigned y = abs(vector[0]);
	unsigned z = abs(vector[1]);
	if(z > y)
		{
			unsigned temp = y;
			y = z;
			z = temp;
		}
	while(z != 0)
	{
		unsigned x = y % z;
		y = z;
		z = x;
	}

	simplifiedVector[0] = (int)(vector[0] / y);
	simplifiedVector[1] = (int)(vector[1] / y);

	return simplifiedVector;
}










