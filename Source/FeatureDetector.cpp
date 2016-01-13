#include "FeatureDetector.h"

FeatureDetector::FeatureDetector(void)
{
}
FeatureDetector::~FeatureDetector(void)
{
}

void FeatureDetector::test()
{
	cv::Mat img1;
	cv::Mat img2;
	cv::Mat img3;

	img1 = cv::Mat::zeros(500,500,CV_8U);
	img2 = cv::Mat::zeros(500,500,CV_8U);
	img3 = cv::Mat::zeros(500,500,CV_8U);

	cv::circle(img1,cv::Point(200,200),200,cv::Scalar(255,255,255),2,8);
	cv::circle(img2,cv::Point(300,300),200,cv::Scalar(255,255,255),2,8);


	img3 = img1 & img2;

	double min, max;
	cv::Point minpt, maxpt;
	cv::minMaxLoc(img3,&min, &max, &minpt,&maxpt, cv::Mat());
	cv::circle(img3,maxpt,50,cv::Scalar(255,0,0),2,8);
	std::cout << minpt.x << "," << minpt.y << std::endl;
	std::cout << maxpt.x << "," << maxpt.y << std::endl;
	std::cout << min << "," << max << std::endl;

	cv::namedWindow("img1",CV_WINDOW_FREERATIO);
	cv::imshow("img1",img1);
	cv::namedWindow("img2",CV_WINDOW_FREERATIO);
	cv::imshow("img2",img2);
	cv::namedWindow("img3",CV_WINDOW_FREERATIO);
	cv::imshow("img3",img3);

	cv::waitKey();

}
void FeatureDetector::init(char* imgname)
{
	MyTools tools;
	//Initialize the image
	std::ifstream file;
	file.open(imgname);
	if(file.is_open())
	{

		shapes = cv::imread(imgname,CV_BGR2GRAY);
		cv::Mat drawing = cv::Mat::zeros(shapes.size(),CV_8U);
		cv::Mat reducedImage = cv::Mat::zeros(shapes.size(),CV_8U);

		//Not sure why but for some reason when I use canny I get a clear image of all the shapes but when I
		//run findContours() on shape1 it seems to change the image.
		//Finds the verticies of the image
		std::vector<std::vector<cv::Point>> contours;
		std::vector<cv::Vec4i> heirarchy;
		
		cv::Canny(shapes,shapes,200,200,3);

		//Finds contours of the image
		//Basically verticies that are next to each other.
		//And draws it onto a drawing board with blue coloring
		cv::findContours(shapes,contours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cv::Point());
		contours = tools.removeOutliers(contours);
		contourSet = contours;
		for(unsigned i = 0; i < contours.size(); i++)
			cv::drawContours(drawing,contours,i,cv::Scalar(255,255,255),0,8,heirarchy,0,cv::Point());

		//Store the necessities
		for(unsigned i = 0; i < contours.size(); i++)
		{
			for(unsigned j = 0; j < contours[i].size(); j++)
				pSet.push_back(new cv::Point(contours[i][j]));
		}
		//Reduce the set size
		pSet = tools.reduction(pSet,12); //12 = radius of reduction size

		//Create the nodes based on pSet
		//We also intialize the id of each node
		for(int i = 0; i < pSet.size(); i++)
		{
			maps.push_back(new Node(*pSet[i]));
			maps[i]->id = new int(i);
		}
		
		//Apply Prim and Dijkstra
		tools.mapNodesPrim(maps);
		//tools.mapNodesDijkstra(0,map);//0 = source node

		//Draw the shape on reducedImage
		for(unsigned i = 0; i < maps.size(); i++)
			cv::circle(reducedImage,*pSet[i],1,cv::Scalar(255,255,255),-1,8);
		for(int i = 0; i < maps.size(); i++)
		{
			for(int j=0; j < maps[i]->edge.size(); j++)
				cv::line(reducedImage,maps[i]->pt,maps[i]->edge[j]->pt,cv::Scalar(255,255,255),2,8);
		}

		//Close the shape
		tools.closeImage(maps,reducedImage);
		//tools.closeImageDijkstra(map,reducedImage);

		////////////////////////////////////
		//////////////////////////////
		///////////////////////


		//IT WILL NOT WORK FOR OPEN IMAGES!
		//For some reason whatsoever flood filling the image makes color go away
		
		//cv::floodFill(reducedImage,cv::Point(0,0),cv::Scalar(255,255,255),0,cv::Scalar(0,0,0,0),cv::Scalar(0,0,0,0),4);
		//cv::Canny(reducedImage,reducedImage,200,200,3);
		//cv::findContours(reducedImage,contours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cv::Point());
		//
		//for(unsigned i = 0; i < contours.size(); i++)
		//	cv::drawContours(reducedImage,contours,i,cv::Scalar(255,255,255),1,8,heirarchy,0,cv::Point());
		

		//3.5 seems to be about the thickness of the pencil
		//A bit weird behavior here where the randomized set makes the reduction more stable but the reduction is also greater
		//std::cout << set.size() << std::endl;

		contourSet = contours;
		shapes = reducedImage;
		
		//cv::namedWindow("drawing",CV_WINDOW_FREERATIO);
		//cv::imshow("drawing",drawing);
		//cv::namedWindow("reducedImage",CV_WINDOW_FREERATIO);
		//cv::imshow("reducedImage",reducedImage);

		//cv::waitKey(0);
		file.close();
	}
	else
	{
		std::cout << "failed to open" << imgname << std::endl;
	}
}
void FeatureDetector::exit()
{
	for(int i = maps.size(); i > 0; i--)
	{
		delete maps[i-1]->id;
		delete maps[i-1];
		maps.erase(maps.begin() + (i-1));
	}

	for(int i = pSet.size(); i > 0; i--)
	{
		delete pSet[i-1];
		pSet.erase(pSet.begin() + (i-1));
	}
}

void FeatureDetector::separateShape(std::vector<int> ids)
{
}
//We get an error if the inscribed circle somehow ends up waaay off the screen.

cv::Mat FeatureDetector::drawSet(std::vector<int> set)
{
	std::sort(set.begin(),set.end());
	cv::Mat shape;
	shape = cv::Mat::zeros(shapes.size(),CV_8U);
	for(int i = 0; i < set.size(); i++)
	{
		for(int j = 0; j < maps[set[i]]->edge.size(); j++)
		{
			//Check to see if the adj node is in set.
			if(std::binary_search(set.begin(),set.end(),*maps[set[i]]->edge[j]->id))
				cv::line(shape,maps[set[i]]->pt, maps[set[i]]->edge[j]->pt,cv::Scalar(255,255,255),2,8);
		}
	}

	return shape;
}
cv::Mat FeatureDetector::drawSet(std::deque<int> set)
{
	std::sort(set.begin(),set.end());
	cv::Mat shape;
	shape = cv::Mat::zeros(shapes.size(),CV_8U);
	for(int i = 0; i < set.size(); i++)
	{
		for(int j = 0; j < maps[set[i]]->edge.size(); j++)
		{
			//Check to see if the adj node is in set.
			if(std::binary_search(set.begin(),set.end(),*maps[set[i]]->edge[j]->id))
				cv::line(shape,maps[set[i]]->pt, maps[set[i]]->edge[j]->pt,cv::Scalar(255,255,255),2,8);
		}
	}

	return shape;
}
Circle FeatureDetector::insertCircle(cv::Mat shape, std::vector<int> set)
{
	//Initialize variables
	//We may have to make sure the starting point "centroid" is actually inside the shape later.
	Circle myCircle;

	cv::Point centroid = tools.findCentroid(pSet);
	bool largestCircle = false;
	bool t = false;
	double radius = 1;
	cv::Point temp = centroid;
	cv::Mat circle;
	///////////////////////////////////////////////////////

	while(largestCircle == false)
	{
		do
		{
			circle = cv::Mat::zeros(shapes.size(),CV_8U);
			cv::circle(circle,centroid,radius,cv::Scalar(255,255,255),2,8);

			radius++;
			t = tools.doesIntersect(shapes, circle);
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
	myCircle.shape = cv::Mat::zeros(shapes.size(), CV_8U);
	cv::circle(myCircle.shape,centroid,radius,cv::Scalar(255,255,255),2,8);

	return myCircle;
}
Circle FeatureDetector::insertCircle(cv::Mat shape, std::deque<int> set)
{
	//Initialize variables
	//We may have to make sure the starting point "centroid" is actually inside the shape later.
	Circle myCircle;

	cv::Point centroid = tools.findCentroid(pSet);
	bool largestCircle = false;
	bool t = false;
	double radius = 1;
	cv::Point temp = centroid;
	cv::Mat circle;
	///////////////////////////////////////////////////////

	while(largestCircle == false)
	{
		do
		{
			circle = cv::Mat::zeros(shapes.size(),CV_8U);
			cv::circle(circle,centroid,radius,cv::Scalar(255,255,255),2,8);

			radius++;
			t = tools.doesIntersect(shapes, circle);
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
	myCircle.shape = cv::Mat::zeros(shapes.size(), CV_8U);
	cv::circle(myCircle.shape,centroid,radius,cv::Scalar(255,255,255),2,8);

	return myCircle;
}
void FeatureDetector::drawCircle2()
{
	//initialize set for 1st iteration of insertCircle
	std::vector<int> mapsID;
	for(int i = 0; i < maps.size(); i++)
		mapsID.push_back(i);
	
	Circle myCircle = insertCircle(shapes, mapsID);

	cv::Mat intersect = cv::Mat::zeros(shapes.size(),CV_8U);		
	intersect = myCircle.shape & shapes;											//For testing

	std::vector<cv::Point> intersectPoints;
	intersectPoints = tools.findIntersections(myCircle.shape,shapes);			//Find intersections

	//For testing
	/*for(int i = 0; i < intersectPoints.size(); i++)
	{
		cv::circle(shapes,intersectPoints[i],20,cv::Scalar(255,255,255),2,8);
	}*/

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	std::vector<int> id;
	for(int i = 0; i < intersectPoints.size(); i++)
	{
		id.push_back(tools.findNodeId(intersectPoints[i],maps));
		std::cout << id[i] << std::endl;
	}

	for(int i = 0; i < id.size(); i++)
	{
		for(int j = 0; j < id.size(); j++)
		{
			if(i == j){}
			else
				cv::line(shapes,maps.at(id[i])->pt,maps.at(id[j])->pt,cv::Scalar(255,255,255),2,8);
		}
	}

	std::deque<int> set2;
	std::deque<int> subShapes;
	Node* tmp = &Node(cv::Point(1,1));

	
	subShapes = tools.findClosestPath(id[1],id[2],maps);
	

	//std::vector<int*> mySets = tools.findAllPaths(id[1],id[2],maps);
	
	//std::vector<Node> testSet;
	//Node tmp = *maps.at(id[0]);
	//Node prev = tmp;

	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	cv::Mat subShape1;
	subShape1 = cv::Mat::zeros(shapes.size(),CV_8U);
	subShape1 = drawSet(subShapes);
	int x, y;
	double distx, disty, slope;
	distx = maps[id[1]]->pt.x - maps[id[2]]->pt.x;
	disty = maps[id[1]]->pt.y - maps[id[2]]->pt.y;
	slope = disty/distx;
	
	for(int i = 0; i < 10; i++)
	{
		if(maps[id[1]]->pt.x < maps[id[2]]->pt.x)
			x = maps[id[1]]->pt.x + std::abs(distx/10) * i;
		else
			x = maps[id[2]]->pt.x + std::abs(distx/10) * i;

		y = slope*(x-maps[id[1]]->pt.x) + maps[id[2]]->pt.y;

		cv::Point(x,y);

		maps.push_back(new Node(cv::Point(x,y)));
	}

	cv::line(subShape1,maps[id[1]]->pt,maps[id[2]]->pt,cv::Scalar(255,255,255),2,8);
	Circle circle2 = insertCircle(subShape1,subShapes);
	cv::circle(subShape1,circle2.center,circle2.radius,cv::Scalar(255,255,255),2,8);

	//cv::Mat intersect2 = cv::Mat::zeros(shapes.size(),CV_8U);		
	//intersect2 = circle2.shape & shapes;											//For testing

	//std::vector<cv::Point> intersectPoints2;
	//intersectPoints2 = tools.findIntersections(circle2.shape, shapes);			//Find intersections

	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////


	//for(int i = 0; i < subShapes.size(); i ++)
	//{
	//	if(i == 0)
	//	{}
	//	else
	//		cv::line(subShape1,maps[subShapes[i]]->pt,maps[subShapes[i - 1]]->pt,cv::Scalar(255,255,255),2,8);

	//	//cv::circle(subShape1,maps[subShapes[i]]->pt,2,cv::Scalar(255,255,0),2,8);
	//	
	//}

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	cv::Mat test = cv::Mat::zeros(shapes.size(), CV_8U);
	test = myCircle.shape + shapes + subShape1;

	cv::namedWindow("testing",CV_WINDOW_FREERATIO);
	cv::namedWindow("intersect", CV_WINDOW_FREERATIO);
	cv::namedWindow("subShape", CV_WINDOW_FREERATIO);

	cv::imshow("intersect",intersect);
	cv::imshow("testing",test);
	cv::imshow("subShape",subShape1);

	cv::waitKey();
	
}

std::vector<cv::Point> FeatureDetector::addCircle(int setid, int cid, std::vector<cv::Point> pointSet)
{
	circles[cid].createPoints();
	for(unsigned i = 0; i < circles[cid].cPoints.size(); i++)
	{
		pointSet.push_back(circles[cid].cPoints[i]);
	}

	return pointSet;
}

