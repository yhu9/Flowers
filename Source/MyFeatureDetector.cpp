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
void MyFeatureDetector::test(char* imgname)
{
  MyTools tools;
  //Initialize the image
  ifstream file;
  file.open(imgname);

  if(file.is_open())
    {

      shapes = imread(imgname,CV_8U);
      Mat detectedEdges = Mat::zeros(shapes.size(), CV_8U);
      Mat drawing = Mat::zeros(shapes.size(),CV_8U);
      Mat reducedImage = Mat::zeros(shapes.size(),CV_8U);


      namedWindow("raw image", CV_WINDOW_FREERATIO);
      imshow("raw image", shapes);

      //Not sure why but for some reason when I use canny I get a clear image of all the shapes but when I
      //run findContours() on shape1 it seems to change the image.
      //Finds the verticies of the image
      vector<vector<Point> > contours;
      vector<Vec4i> heirarchy;

      //Run Canny which highlights the verticies of the shape
      blur(shapes, detectedEdges, Size(3,3));
      namedWindow("blured image", CV_WINDOW_FREERATIO);
      imshow("blured image", detectedEdges);
		

      Canny(detectedEdges,detectedEdges,50,150,3);
      namedWindow("detected edges", CV_WINDOW_FREERATIO);
      imshow("detected edges", detectedEdges);

      cout << "canny run successfully" << endl;

      drawing = drawing + detectedEdges;
      namedWindow("drawing U detectedEdges", CV_WINDOW_FREERATIO);
      imshow("drawing U detectedEdges", drawing);

      //Finds contours of the image
      //Basically verticies that are next to each other.
      //And draws it onto a drawing board with blue coloring
      findContours(detectedEdges,contours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,Point());
      cout << "contours found" << endl;
      //contours = tools.removeOutliers(contours);
		
      file.close();
    }
  else
    cout << "failed to open" << imgname << endl;
	


}
void MyFeatureDetector::init(char* imgname)
{
  MyTools tools;
  //Initialize the image
  ifstream file;
  file.open(imgname);
	
  if(file.is_open())
    {

      shapes = imread(imgname,CV_8U);
      Mat detectedEdges = Mat::zeros(shapes.size(), CV_8U);
      Mat drawing = Mat::zeros(shapes.size(),CV_8U);
      Mat reducedImage = Mat::zeros(shapes.size(),CV_8U);

      //Not sure why but for some reason when I use canny I get a clear image of all the shapes but when I
      //run findContours() on shape1 it seems to change the image.
      //Finds the verticies of the image
      vector<vector<Point> > contours;
      vector<Vec4i> heirarchy;

      //Run Canny which highlights the verticies of the shape
      blur(shapes, detectedEdges, Size(3,3));

      Canny(detectedEdges,detectedEdges,50,150,3);
		
      //If you want to see the raw, blured, and detected edges
      //////////////////////////////////////////////////////////////////////////////////////////////////
      //namedWindow("raw image", CV_WINDOW_FREERATIO);						//
      //imshow("raw image", shapes);									//
      //namedWindow("blured image", CV_WINDOW_FREERATIO);						//
      //imshow("blured image", detectedEdges);							//
      //namedWindow("detected edges", CV_WINDOW_FREERATIO);						//
      //imshow("detected edges", detectedEdges);							//
      //waitKey(0);											//
      //cout << "canny run successfully" << endl;							//
      //////////////////////////////////////////////////////////////////////////////////////////////////

      //Finds contours of the image
      //Basically verticies that are next to each other.
      //And draws it onto a drawing board with blue coloring
      findContours(detectedEdges,contours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,Point());
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
      cout << "set size: " << pSet.size() << endl;
      pSet = tools.reduction(pSet,12); //12 = radius of reduction size
      cout << "set reduced to: " << pSet.size() << endl;

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
      tools.closeImage(maps,reducedImage);

      //tools contain the functions made by Masa Hu
      ////////////////////////////////////
      //////////////////////////////
      ///////////////////////

      contourSet = contours;
      shapes = reducedImage;
		
      //if you'd like you can show the basic images we created
      namedWindow("drawing",CV_WINDOW_FREERATIO);
      imshow("drawing",drawing);
      waitKey(0);
      //namedWindow("reducedImage",CV_WINDOW_FREERATIO);
      //imshow("reducedImage",reducedImage);

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

//insertCircle takes  a seed point and a shape and tries to insert a circle in the shape from the seed point.
//The circle will be the best fit upon jiterring a circle inside the shape.
//The function will return a Circle with a radius of -1 if the seed point isn't inside the shape.
Circle MyFeatureDetector::insertCircle(Mat shape, Point seed)
{
    bool isInside = tools.isInside(seed,shapes);
    bool isOn = tools.isOn(seed,shape);
  if(isInside == false || isOn == true){
    Circle falseCircle;
    falseCircle.radius = -1;
    return falseCircle;
  }
		
  //Initialize variables
  //We may have to make sure the starting point "centroid" is actually inside the shape later.
  Circle myCircle;

  bool largestCircle = false;
  bool t = false;
  double radius = 1;
  Point temp = seed;
  Mat circle_img;
  ///////////////////////////////////////////////////////

  while(largestCircle == false)
    {
      do
	{
	  circle_img = Mat::zeros(shape.size(),CV_8U);
	  circle(circle_img,seed,radius,Scalar(255,255,255),2,8);

	  radius++;
	  t = tools.doesIntersect(shape, circle_img);
	}while(!t);

      temp = tools.jitterCircle(seed, radius,shape);

      if(temp.x == seed.x && temp.y == seed.y)
	{
	  largestCircle = true;
	  radius+=2;
	}
      else
	seed = temp;
    }

  myCircle.center = seed;
  myCircle.radius = radius;
  myCircle.shape = Mat::zeros(shape.size(), CV_8U);
  circle(myCircle.shape,seed,radius,Scalar(255,255,255),2,8);

  return myCircle;
}


//Truth Table
//a = isOn Shape; b = isOn Circle; c = isInside Shape; d = isInside Circle
//------------------------------------------------------------------------
//a|b|c|d|result
//--------------
//0|0|0|0|exit
//0|0|0|1|invalid
//0|0|1|0|Q.add
//0|0|1|1|increment
//0|1|0|0|exit
//0|1|0|1|invalid
//0|1|1|0|invalid
//0|1|1|1|increment
//exit on all other states

void MyFeatureDetector::drawCircle()
{
  //namedWindow("circle", CV_WINDOW_FREERATIO);			
  namedWindow("shapes",CV_WINDOW_FREERATIO);			     
  namedWindow("circles to add", CV_WINDOW_FREERATIO);     
  Mat circlestoadd = Mat::zeros(shapes.size(), CV_8U);
  Mat newShape = Mat::zeros(shapes.size(),CV_8U);
  newShape = shapes | shapes;

  Point seed = tools.findCentroid(pSet);
  queue<Point> que;
  que.push(seed);
  int t = 1;
  while(que.size() > 0){
    //Pop one from que
    Point pt = que.front();
    que.pop();

    //insert circle  
    //check to see if you can insert the circle correctly. 
    vector<Point> intersectPoints;
    Circle myCircle = insertCircle(newShape, pt);
    if(myCircle.radius != -1)
	intersectPoints = tools.findIntersections(myCircle.shape,newShape);

    //find intersectpts and check how many there are

    cout << "iteration: " << t << endl;t++;
    	cout << "que size: " << que.size() << endl;
    	cout << "myCircle: " << myCircle.radius << endl;
	cout << "number of intersections: " << intersectPoints.size() << endl;

    if(intersectPoints.size() < 3)
      cout << "found less than 3 intersections" << endl;
    else if(intersectPoints.size() >= 3)
     {
	vector<Point> midpts;
	vector<Vec2i> vectors;
	vector<Vec2i> normalizedVectors;
	unsigned s = intersectPoints.size();

	//find midpts
	for(unsigned i = 0; i < s; i++)
	 	midpts.push_back(tools.findMidPoint(intersectPoints[i],intersectPoints[(i + 1) % s]));
	//find vectors
	for(unsigned i = 0; i < s; i++)
	  vectors.push_back(tools.makeVector(myCircle.center,midpts[i]));
	//Normalize vectors
	for(unsigned i = 0; i < s; i++)
	  normalizedVectors.push_back(tools.normalize(vectors[i]));

	for(unsigned i = 0; i < s; i++)
	 {
	    Point tempPt = midpts[i];
	    bool validPt = false;
	    while(!validPt)
	      {
		if(tools.isInsideAndNotOn(tempPt,newShape)
		   && !tools.isInside (tempPt,myCircle.shape))
		  {
		    validPt = true;
		    que.push(Point(tempPt.x,tempPt.y));
		  }
		//I suppose this could potentially fail if Your somehow end up always in the circle and not on the shape.
		//Karnaugh Map representation of this gives us the output.
                //We make sure normalized vectors does not have a vector of {0,0}
		else if(!tools.isOn(tempPt,newShape) && tools.isInside(tempPt,myCircle.shape))
		  {
                    
		    double x = tempPt.x + normalizedVectors[i][0];
		    double y = tempPt.y + normalizedVectors[i][1];
		  
		    tempPt = Point(x,y);
                    if(normalizedVectors[i][0] == 0 && normalizedVectors[i][1] == 0)
                        validPt = true;
		  }
		else
		  {
		    validPt = true;
		    //exit loop without adding anything to Queue
		  }
	      }  
	  }
      }
    
    newShape = newShape + myCircle.shape;
    for(unsigned i = 0; i < que.size(); i++)
        circle(circlestoadd, que.front(), 10, Scalar(255,255,255), -1, 8);
    circlestoadd = shapes + circlestoadd;

    imshow("circles to add", circlestoadd);
    imshow("shapes",newShape);
    waitKey(0);
  }
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

