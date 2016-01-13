#include "MyTools.h"

MyTools::MyTools(){}
MyTools::~MyTools(){}

//Some useful trigonometry and geometry tools
cv::Point MyTools::findClosestPoint(int x, int y, std::vector<cv::Point> contour)
{
	double smallestDistance = -1, tmp = 0;
	cv::Point closestPoint;
	for(unsigned i = 0; i < contour.size(); i++)
	{
		tmp = std::sqrt(std::pow((double)(contour[i].x - x),2) + std::pow((double)(contour[i].y - y),2));
		if(tmp < smallestDistance || smallestDistance == -1)
		{
			smallestDistance = tmp;
			closestPoint = contour[i];
		}
	}
	return closestPoint;
}

cv::Point MyTools::findClosestPoint(cv::Point center, std::vector<Node> contour)
{
	double smallestDistance = -1, tmp = 0;
	cv::Point closestPoint;
	for(unsigned i = 0; i < contour.size(); i++)
	{
		tmp = std::sqrt(std::pow((double)(contour[i].pt.x - center.x),2) + std::pow((double)(contour[i].pt.y - center.y),2));
		if(tmp < smallestDistance || smallestDistance == -1)
		{
			smallestDistance = tmp;
			closestPoint = contour[i].pt;
		}
	}
	return closestPoint;
}
int MyTools::findClosestPoint(cv::Point point, std::vector<cv::Point> contour)
{
	double smallestDistance = -1, tmp = 0;
	int closestPoint = -1;
	for(unsigned i = 0; i < contour.size(); i++)
	{
		tmp = std::sqrt(std::pow((double)(contour[i].x - point.x),2) + std::pow((double)(contour[i].y - point.y),2));
		if(tmp < smallestDistance || smallestDistance == -1)
		{
			smallestDistance = tmp;
			closestPoint = i;
		}
	}
	return closestPoint;
}
cv::Point MyTools::findMidPoint(cv::Point v1, cv::Point v2)
{
	cv::Point midPoint;
	double x,y;
	x = (v1.x + v2.x)/2;
	y = (v1.y + v2.y)/2;
	midPoint = cv::Point(x,y);
	return midPoint;
}
void MyTools::outlineContour(std::vector<std::vector<cv::Point>> contour, cv::Mat img)
{
	cv::Point prev = contour[0][0];
	for(unsigned i = 0; i < contour.size(); i++)
	{
		for(unsigned j = 1; j < contour[i].size() - 1; j++)
		{
			cv::line(img,prev,contour[i][j],cv::Scalar(255,255,255),2,8);
			prev = cv::Point(contour[i][j].x,contour[i][j].y);
		}
	}
}
double MyTools::findDistance(cv::Point v1, cv::Point v2)
{
	double distance = -1;

	distance = sqrt(pow((double)(v2.x - v1.x), 2) + pow((double)v2.y - (double)v1.y,2));
	return distance;
}
double MyTools::findAngleOfRay(cv::Point start, cv::Point end)
{
	double theta = 0;
	if(end.x == start.x)
		return theta;
	else
		theta = std::atan((double)(end.y - start.y) / (double)(end.x - start.x));

	return theta;
}
cv::Vec2i MyTools::findDirectionOfRay(cv::Point start,cv::Point end)
{
	cv::Vec2i vec(end.x - start.x, end.y - start.y);
	return vec;
}
double MyTools::findSlope(cv::Point v1, cv::Point v2)
{
	double slope = (double)(v2.y - v1.y)/(double)(v2.x - v1.x);
	return slope;
}

int MyTools::findPointId(cv::Point pt, std::vector<cv::Point> pointSet)
{
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

cv::Point MyTools::findCentroid(std::vector<cv::Point*> pointSet)
{
	double x = 0;
	double y = 0;
	int i = 0;
	for(i = 0; i < pointSet.size(); i++)
	{
		x += pointSet[i]->x;
		y += pointSet[i]->y;
	}

	x = x/i;
	y = y/i;

	return cv::Point(x,y);
}
cv::Point MyTools::findCentroid(std::vector<int> id, std::vector<Node*> map)
{
	double x = 0;
	double y = 0;
	int i = 0;
	for(i = 0; i < id.size(); i++)
	{
		x += map[id[i]]->pt.x;
		y += map[id[i]]->pt.y;
	}

	x = x/i;
	y = y/i;

	return cv::Point(x,y);
}

////////////////////////////////////////////////////////////////////////////////
//Map functions
int MyTools::findNodeId(cv::Point pt, std::vector<Node*> map)
{
	//We try to look for the id of the closest node in the map that matches the id.
	//The max distance the closest node can be is 19
	int dist = 3;
	for(int j = 0; j < 20; j++,dist++)
	{
		for(int i = 0; i < map.size(); i++)
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
std::deque<int> MyTools::findClosestPath(int src, int dest, std::vector<Node*> map)
{
	mapNodesDijkstra(src,map);
	std::deque<int> path;
	std::vector<int> node3;
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
	for(int i = 0; i < map.size(); i++)
	{
		//Check if node is a 1 edge node
		if(map[i]->edge.size() == 1)
		{
			bool check = false;
			std::deque<int> tmpPath;
		
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
						for(int a = 0; a < node3.size(); a++)
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
				for(int tmpPathID = 0; tmpPathID < tmpPath.size(); tmpPathID++)
				{
					path.push_back(tmpPath[tmpPathID]);
				}
			}
		}
	}

	return path;
}
void MyTools::traversePath(int src, int prev, int dest, std::vector<Node*> map)
{
	/*for(int i = 0; i < map[src]->edge.size(); i++)
	{
		if(map[src]->edge[i]->id == prev)
		{
		}
		else if(map[src]->edge[i]->id = dest)
		{
		}
		else
		{
			std::cout << 
			traversePath(map[src]->edge[i]->id,src,dest,map);
		}
	}*/
}

//Work in progress
std::vector<int*> MyTools::findAllPaths(int src, int dest, std::vector<Node*> map)
{
	int* path = new int[map.size() - 1];
	std::vector<int*> paths;
	bool* visited = new bool[map.size() - 1];
	int pathid = 0;

	for(int i = 0; i < map.size() -1; i++)
		visited[i] = false;


	//traversePath(map,src,dest,visited,path,pathid,paths);

	delete [] visited;
	delete [] path;
	return paths;
}
///////////////////////////////////////////////////////////////////////////////////////////

std::vector<Node*> MyTools::mapNodesDijkstra(int srcid, std::vector<Node*> set)
{
	std::vector<int> myQ;

	for(int i = 0; i < set.size(); i++)
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
		for(int i = 0; i < myQ.size(); i++)
		{
			if(set[myQ[i]]->dist < smallest)
			{
				smallest = set[myQ[i]]->dist;
				id = i;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		int length = 0;
		
		for(int i = 0; i < set[myQ[id]]->edge.size(); i++)
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
std::vector<Node*> MyTools::mapNodesPrim(std::vector<Node*> set)
{
	std::vector<int> reachedSet;
	std::vector<int> unreachedSet;

	for(int i = 0; i < set.size(); i++)
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
		for(int i = 0; i < reachedSet.size(); i++)
		{
			for(int j = 0; j < unreachedSet.size(); j++)
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
void MyTools::unionOfPoints(std::vector<cv::Point> set1,std::vector<cv::Point> set2, std::vector<cv::Point> output)
{
	if(!output.empty())
		output.clear();

	for(int i = 0; i < set1.size(); i++)
		output.push_back(set1[i]);

	for(int i = 0; i < set2.size(); i++)
		output.push_back(set2[i]);

}
void MyTools::intersectionOfPoints(std::vector<cv::Point> set1,std::vector<cv::Point> set2, std::vector<cv::Point> output)
{
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
void MyTools::subtractionOfPoints(std::vector<cv::Point> set1,std::vector<cv::Point> set2, std::vector<cv::Point> output)
{
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
bool MyTools::doesIntersect(cv::Mat img1, cv::Mat img2)
{
	//Should later make sure that all image types are of type CV_8U to make the comparison
	cv::Mat img3;
	img3 = cv::Mat::zeros(img1.size().width,img1.size().height,CV_8U);
	img3 = img1 & img2;

	cv::Point minpt, maxpt;
	cv::minMaxLoc(img3,NULL,NULL,&minpt,&maxpt);

	//Draws the intersection of img1 and img2 on img3 and if it finds
	//a bright spot we say there is an intersection.

	//cv::namedWindow("img1",CV_WINDOW_FREERATIO);
	//cv::namedWindow("img2",CV_WINDOW_FREERATIO);
	//cv::namedWindow("img3",CV_WINDOW_FREERATIO);

	//cv::imshow("img1",img1);
	//cv::imshow("img2",img2);
	//cv::imshow("img3",img3);

	//cv::waitKey();

	if(minpt.x == maxpt.x && minpt.y == maxpt.y)
		return false;
	else
		return true;
}

//Tools to transform shape or point post processing
cv::Point MyTools::jitterCircle(cv::Point c, double radius, cv::Mat shape)
{
	cv::Mat circle;
	int x = c.x;
	int y = c.y;
	circle = cv::Mat::zeros(shape.size(),CV_8U);
	//cv::imshow("",shape);
	//cv::waitKey();
	
	//Trial Number2
	for(double radian = 0; radian < 6.2; radian+=0.1)
	{
		c.x = x + 5 * cos(radian);
		c.y = y + 5 * sin(radian);

		cv::circle(circle,c,radius,cv::Scalar(255,255,255),1,8);
		if(doesIntersect(shape,circle))
		{
				c.x = x;
				c.y = y;
		}
		else
		{
			//cv::circle(shape,c,radius,cv::Scalar(255,255,255),2,8);
			//cv::imshow("",shape);
			//cv::waitKey();
		
			return c;
		}

		circle = cv::Mat::zeros(circle.size(),CV_8U);
	}

	return c;
}
std::vector<cv::Point> MyTools::findIntersections(cv::Mat img1, cv::Mat img2)
{
	std::vector<cv::Point> intersectPoints;
	cv::Mat intersection;
	intersection = cv::Mat::zeros(img1.size(),CV_8U);
	intersection = img1 & img2;

	cv::Point minpt, maxpt;

	do
	{
		cv::minMaxLoc(intersection,NULL,NULL,&minpt,&maxpt);
		if(minpt.x != maxpt.x && minpt.y != maxpt.y)
			intersectPoints.push_back(maxpt);

		cv::circle(intersection, maxpt,40,cv::Scalar(0,0,0),-1,8);

	}while(minpt.x != maxpt.x && minpt.y != maxpt.y);

	return intersectPoints;
}


//pre-processing
//removeOutliers uses cv::fitEllipse to find a region where the cluster of points are fairly close to each other
void MyTools::closeOpenContours(cv::Mat img, std::vector<std::vector<cv::Point>> contour)
{

}
std::vector<std::vector<cv::Point>> MyTools::removeOutliers(std::vector<std::vector<cv::Point>> contour)
{
	std::vector<cv::Point> pointSet;
	for(unsigned i = 0; i < contour.size(); i++)
	{
		for(unsigned j = 0; j < contour[i].size(); j++)
			pointSet.push_back(contour[i][j]);
	}
	cv::RotatedRect ellipse = cv::fitEllipse(pointSet);

	std::vector<int> distances;
	for(unsigned i = 0; i < pointSet.size(); i++)
	{
		distances.push_back(MyTools::findDistance(ellipse.center,pointSet[i]));
	}

	std::sort(distances.begin(),distances.end());
	double q1,q3,IQR;
	q1 = distances[(int)(pointSet.size()/4)];
	q3 = distances[(int)(pointSet.size()/4 * 3)];
	//std::cout << q1 << "," << q3 << std::endl;
	IQR = q3-q1;
	
	int i = 0;
	while(i < contour.size())
	{
		int j = 0;
		while(j < contour[i].size())
		{
			if(MyTools::findDistance(ellipse.center,contour[i][j]) > q3 + 1.5*IQR)
			{
				//std::cout << MyTools::findDistance(ellipse.center,contour[i][j]) << std::endl;
				contour[i].erase(contour[i].begin() + j);
			}
			else
				j++;
		}
		i++;
	}
	
	return contour;
}
std::vector<cv::Point*> MyTools::reduction(std::vector<cv::Point*> set, int reductionSize)
{
	int i = 0;
	int j = 0;
	std::vector<int> setid;
	cv::Mat drawing;
	
	int x, y, count;
	for(int i = 0; i < set.size(); i++)
	{
		x = 0;
		y = 0;
		count = 0;
		for(int j = 0; j < set.size(); j++)
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
			std::sort(setid.begin(),setid.end());
			for(int a = setid.size(); a > 0; a--)
			{
				if(setid[a - 1] >= set.size())
				{
					std::cout << setid[a - 1] << " setid" << std::endl;
					std::cout << set.size() << " setsize" << std::endl;
				}
				else
				{
					delete set[setid[a - 1]];
					set.erase(set.begin() + setid[a - 1]);
				}
			}
			x = x / count;
			y = y / count;
			set.push_back(new cv::Point(x,y));
			setid.clear();
		}
	}
	return set;
}
std::vector<cv::Point> MyTools::randomize(std::vector<cv::Point> set)
{
	std::vector<cv::Point> randomSet;
	int randid;
	std::cout << set.size() << std::endl;
	for(int i = set.size(); i > 0; i--)
	{
		randid = std::rand() % set.size();
		cv::Point temp = set[randid];
		set.erase(set.begin() + randid);
		randomSet.push_back(temp);
	}
	//std::cout << randomSet.size() << std::endl;
	return randomSet;
}
void MyTools::closeImageDijkstra(std::vector<Node*> set, cv::Mat img) //The image must have Dijkstra run on it first.
{
	std::vector<Node> setx;
	//Push all nodes with only 1 edge into setx
	for(int i = 0; i < set.size(); i++)
	{
		if(set[i]->edge.size() == 1 && set[i]->dist != 0)
			setx.push_back(*set[i]);
	}

	int largest = 0;
	int id1 = 0;
	int id2 = 0;
	int closest = 500000;

	for(int i = 0; i < setx.size(); i++)
	{
		if(setx[i].dist > largest)
		{
			largest = setx[i].dist;
			id1 = i;
		}
	}
	
	for(int i = 0; i < setx.size(); i++)
	{
		double dist = findDistance(setx[id1].pt,setx[i].pt);
		if(dist < closest && id1 != i)
		{
			closest = dist;
			id2 = i;
		}
	}

	cv::line(img,setx[id1].pt,setx[id2].pt,cv::Scalar(255,255,0),2,8,0);
}
void MyTools::closeImage(std::vector<Node*> set, cv::Mat img)
{
	//Push back all nodes with only 1 edge into setx
	std::vector<int> setx;
	std::vector<int> sety;
	for(int i = 0; i < set.size(); i++)
	{
		if(set[i]->edge.size() == 1)
			setx.push_back(i);
		if(set[i]->edge.size() >= 3)
			sety.push_back(i);
	}

	//erase all nodes that are super close to a node with 3 edges
	for(int i = 0; i < setx.size(); i++)
	{
		for(int j = 0; j < sety.size(); j++)
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
	for(int i = 0; i < setx.size(); i++)
	{
		for(int j = 0; j < setx.size(); j++)
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
	cv::line(img,set[setx[id1]]->pt,set[setx[id2]]->pt,cv::Scalar(255,255,0),2,8,0);
}
