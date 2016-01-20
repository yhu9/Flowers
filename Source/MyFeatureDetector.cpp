#include "MyFeatureDetector.h"

using namespace cv;
using namespace std;

MyFeatureDetector::MyFeatureDetector(void)
{
}
MyFeatureDetector::~MyFeatureDetector(void)
{
}

//test() tests all the function in the MyFeatureDetector class
void MyFeatureDetector::test()
{

}
void MyFeatureDetector::init(char* imgname)
{
	MyTools tools;
	//Initialize the image
	ifstream file;
	file.open(imgname);
	if(file.is_open())
	{

		shapes = imread(imgname,CV_BGR2GRAY);
		Mat drawing = Mat::zeros(shapes.size(),CV_8U);
		Mat reducedImage = Mat::zeros(shapes.size(),CV_8U);


		namedWindow("raw image", CV_WINDOW_FREERATIO);
		imshow("raw image", shapes);

		waitKey(0);
		//Not sure why but for some reason when I use canny I get a clear image of all the shapes but when I
		//run findContours() on shape1 it seems to change the image.
		//Finds the verticies of the image
		vector<vector<Point> > contours;
		vector<Vec4i> heirarchy;

		//Run Canny which highlights the verticies of the shape
		Canny(shapes,shapes,200,200,3);
		cout << "canny run successfully" << endl;

		//Finds contours of the image
		//Basically verticies that are next to each other.
		//And draws it onto a drawing board with blue coloring
		findContours(shapes,contours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,Point());
		cout << "contours found" << endl;
		contours = tools.removeOutliers(contours);
		
		contourSet = contours;

		//Draw contour on drawing
		for(unsigned i = 0; i < contours.size(); i++)
			drawContours(drawing,contours,i,Scalar(255,255,255),0,8,heirarchy,0,Point());

		//Store contours created in previous step in pSet of class
		for(unsigned i = 0; i < contours.size(); i++)
		{
			for(unsigned j = 0; j < contours[i].size(); j++)
				pSet.push_back(new Point(contours[i][j]));
		}

		//Reduce the set size of pSet or else we may end up with too many points in the set
		//we do not use variable contours anymore
		pSet = tools.reduction(pSet,12); //12 = radius of reduction size
		cout << "reduce the set" << endl;

		//Create the nodes based on pSet
		//We also intialize the id of each node
		//The nodes are stored in a class variable called maps
		for(unsigned i = 0; i < pSet.size(); i++)
		{
			maps.push_back(new Node(*pSet[i]));
			maps[i]->id = new int(i);
		}
		cout << "Nodes successfully created" << endl;
		
		//Apply Prim
		//Previously we did apply Dijkstra but there was no need for it
		tools.mapNodesPrim(maps);
		cout << "Nodes mapped using prim" << endl;

		//Draw the shape on reducedImage all we do here is thicken each point in pSet
		for(unsigned i = 0; i < maps.size(); i++)
			circle(reducedImage,*pSet[i],1,Scalar(255,255,255),-1,8);
		for(unsigned i = 0; i < maps.size(); i++)
		{
			for(unsigned j=0; j < maps[i]->edge.size(); j++)
				line(reducedImage,maps[i]->pt,maps[i]->edge[j]->pt,Scalar(255,255,255),2,8);
		}
		cout << "image drawn on reduced set" << endl;

		//Close the shape
		//tools.closeImage(maps,reducedImage);

		//tools contain the functions made by Masa Hu
		////////////////////////////////////
		//////////////////////////////
		///////////////////////

		contourSet = contours;
		shapes = reducedImage;
		
		//if you'd like you can show the basic images we created
		namedWindow("drawing",CV_WINDOW_FREERATIO);
		imshow("drawing",drawing);
		namedWindow("reducedImage",CV_WINDOW_FREERATIO);
		imshow("reducedImage",reducedImage);

		waitKey(0);
		file.close();
	}
	else
	{
		cout << "failed to open" << imgname << endl;
	}
}
void MyFeatureDetector::exit()
{
	for(unsigned i = maps.size(); i > 0; i--)
	{
		delete maps[i-1]->id;
		delete maps[i-1];
		maps.erase(maps.begin() + (i-1));
	}

	for(unsigned i = pSet.size(); i > 0; i--)
	{
		delete pSet[i-1];
		pSet.erase(pSet.begin() + (i-1));
	}
}

void MyFeatureDetector::separateShape(vector<int> ids)
{
}
//We get an error if the inscribed circle somehow ends up waaay off the screen.

Mat MyFeatureDetector::drawSet(vector<int> set)
{
	sort(set.begin(),set.end());
	Mat shape;
	shape = Mat::zeros(shapes.size(),CV_8U);
	for(unsigned i = 0; i < set.size(); i++)
	{
		for(unsigned j = 0; j < maps[set[i]]->edge.size(); j++)
		{
			//Check to see if the adj node is in set.
			if(binary_search(set.begin(),set.end(),*maps[set[i]]->edge[j]->id))
				line(shape,maps[set[i]]->pt, maps[set[i]]->edge[j]->pt,Scalar(255,255,255),2,8);
		}
	}

	return shape;
}
Mat MyFeatureDetector::drawSet(deque<int> set)
{
	sort(set.begin(),set.end());
	Mat shape;
	shape = Mat::zeros(shapes.size(),CV_8U);
	for(unsigned i = 0; i < set.size(); i++)
	{
		for(unsigned j = 0; j < maps[set[i]]->edge.size(); j++)
		{
			//Check to see if the adj node is in set.
			if(binary_search(set.begin(),set.end(),*maps[set[i]]->edge[j]->id))
				line(shape,maps[set[i]]->pt, maps[set[i]]->edge[j]->pt,Scalar(255,255,255),2,8);
		}
	}

	return shape;
}
Circle MyFeatureDetector::insertCircle(Mat shape, vector<int> set)
{
	//Initialize variables
	//We may have to make sure the starting point "centroid" is actually inside the shape later.
	Circle myCircle;

	Point centroid = tools.findCentroid(pSet);
	bool largestCircle = false;
	bool t = false;
	double radius = 1;
	Point temp = centroid;
	Mat circle_img;
	///////////////////////////////////////////////////////

	while(largestCircle == false)
	{
		do
		{
			circle_img = Mat::zeros(shapes.size(),CV_8U);
			circle(circle_img,centroid,radius,Scalar(255,255,255),2,8);

			radius++;
			t = tools.doesIntersect(shapes, circle_img);
		}while(!t);

		temp = tools.jitterCircle(centroid, radius,shapes);

		if(temp.x == centroid.x && temp.y == centroid.y)
		{
			largestCircle = true;
			radius+=2;
		}
		else
			centroid = temp;
	}

	myCircle.center = centroid;
	myCircle.radius = radius;
	myCircle.shape = Mat::zeros(shapes.size(), CV_8U);
	circle(myCircle.shape,centroid,radius,Scalar(255,255,255),2,8);

	return myCircle;
}
Circle MyFeatureDetector::insertCircle(Mat shape, deque<int> set)
{
	//Initialize variables
	//We may have to make sure the starting point "centroid" is actually inside the shape later.
	Circle myCircle;

	Point centroid = tools.findCentroid(pSet);
	bool largestCircle = false;
	bool t = false;
	double radius = 1;
	Point temp = centroid;
	Mat circle_img;
	///////////////////////////////////////////////////////

	while(largestCircle == false)
	{
		do
		{
			circle_img = Mat::zeros(shapes.size(),CV_8U);
			circle(circle_img,centroid,radius,Scalar(255,255,255),2,8);

			radius++;
			t = tools.doesIntersect(shapes, circle_img);
		}while(!t);

		temp = tools.jitterCircle(centroid, radius,shapes);

		if(temp.x == centroid.x && temp.y == centroid.y)
		{
			largestCircle = true;
			radius+=2;
		}
		else
			centroid = temp;
	}

	myCircle.center = centroid;
	myCircle.radius = radius;
	myCircle.shape = Mat::zeros(shapes.size(), CV_8U);
	circle(myCircle.shape,centroid,radius,Scalar(255,255,255),2,8);

	return myCircle;
}
void MyFeatureDetector::drawCircle2()
{
	//initialize set for 1st iteration of insertCircle
	vector<int> mapsID;
	for(unsigned i = 0; i < maps.size(); i++)
		mapsID.push_back(i);
	
	Circle myCircle = insertCircle(shapes, mapsID);

	Mat intersect = Mat::zeros(shapes.size(),CV_8U);		
	intersect = myCircle.shape & shapes;											//For testing

	vector<Point> intersectPoints;
	intersectPoints = tools.findIntersections(myCircle.shape,shapes);			//Find intersections

	//For testing
	/*for(unsigned i = 0; i < intersectPoints.size(); i++)
	{
		circle(shapes,intersectPoints[i],20,Scalar(255,255,255),2,8);
	}*/

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	vector<int> id;
	for(unsigned i = 0; i < intersectPoints.size(); i++)
	{
		id.push_back(tools.findNodeId(intersectPoints[i],maps));
		cout << id[i] << endl;
	}

	for(unsigned i = 0; i < id.size(); i++)
	{
		for(unsigned j = 0; j < id.size(); j++)
		{
			if(i == j){}
			else
				line(shapes,maps.at(id[i])->pt,maps.at(id[j])->pt,Scalar(255,255,255),2,8);
		}
	}

	deque<int> set2;
	deque<int> subshapes;


	
	subshapes = tools.findClosestPath(id[1],id[2],maps);
	

	//vector<int*> mySets = tools.findAllPaths(id[1],id[2],maps);
	
	//vector<Node> testSet;
	//Node tmp = *maps.at(id[0]);
	//Node prev = tmp;

	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	Mat subShape1;
	subShape1 = Mat::zeros(shapes.size(),CV_8U);
	subShape1 = drawSet(subshapes);
	int x, y;
	double distx, disty, slope;
	distx = maps[id[1]]->pt.x - maps[id[2]]->pt.x;
	disty = maps[id[1]]->pt.y - maps[id[2]]->pt.y;
	slope = disty/distx;
	
	for(unsigned i = 0; i < 10; i++)
	{
		if(maps[id[1]]->pt.x < maps[id[2]]->pt.x)
			x = maps[id[1]]->pt.x + abs(distx/10) * i;
		else
			x = maps[id[2]]->pt.x + abs(distx/10) * i;

		y = slope*(x-maps[id[1]]->pt.x) + maps[id[2]]->pt.y;

		Point(x,y);

		maps.push_back(new Node(Point(x,y)));
	}

	line(subShape1,maps[id[1]]->pt,maps[id[2]]->pt,Scalar(255,255,255),2,8);
	Circle circle2 = insertCircle(subShape1,subshapes);
	circle(subShape1,circle2.center,circle2.radius,Scalar(255,255,255),2,8);

	//Mat intersect2 = Mat::zeros(shapes.size(),CV_8U);		
	//intersect2 = circle2.shape & shapes;											//For testing

	//vector<Point> intersectPoints2;
	//intersectPoints2 = tools.findIntersections(circle2.shape, shapes);			//Find intersections

	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////


	//for(unsigned i = 0; i < subshapes.size(); i ++)
	//{
	//	if(i == 0)
	//	{}
	//	else
	//		line(subShape1,maps[subshapes[i]]->pt,maps[subshapes[i - 1]]->pt,Scalar(255,255,255),2,8);

	//	//circle(subShape1,maps[subshapes[i]]->pt,2,Scalar(255,255,0),2,8);
	//	
	//}

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	Mat test = Mat::zeros(shapes.size(), CV_8U);
	test = myCircle.shape + shapes + subShape1;

	namedWindow("testing",CV_WINDOW_FREERATIO);
	namedWindow("intersect", CV_WINDOW_FREERATIO);
	namedWindow("subShape", CV_WINDOW_FREERATIO);

	imshow("intersect",intersect);
	imshow("testing",test);
	imshow("subShape",subShape1);

	waitKey();
	
}

vector<Point> MyFeatureDetector::addCircle(int setid, int cid, vector<Point> pointSet)
{
	circles[cid].createPoints();
	for(unsigned i = 0; i < circles[cid].cPoints.size(); i++)
	{
		pointSet.push_back(circles[cid].cPoints[i]);
	}

	return pointSet;
}

