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

      waitKey(0);
      //Not sure why but for some reason when I use canny I get a clear image of all the shapes but when I
      //run findContours() on shape1 it seems to change the image.
      //Finds the verticies of the image
      vector<vector<Point> > contours;
      vector<Vec4i> heirarchy;

      //Run Canny which highlights the verticies of the shape
      blur(shapes, detectedEdges, Size(3,3));
      namedWindow("blured image", CV_WINDOW_FREERATIO);
      imshow("blured image", detectedEdges);
		
      waitKey(0);

      Canny(detectedEdges,detectedEdges,50,150,3);
      namedWindow("detected edges", CV_WINDOW_FREERATIO);
      imshow("detected edges", detectedEdges);

      waitKey(0);
      cout << "canny run successfully" << endl;

      drawing = drawing + detectedEdges;
      namedWindow("drawing U detectedEdges", CV_WINDOW_FREERATIO);
      imshow("drawing U detectedEdges", drawing);

      waitKey(0);

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
      //namedWindow("reducedImage",CV_WINDOW_FREERATIO);
      //imshow("reducedImage",reducedImage);

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
  if(!tools.isInside(seed,shape) || tools.isOn(seed,shape)){
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
	  circle_img = Mat::zeros(shapes.size(),CV_8U);
	  circle(circle_img,seed,radius,Scalar(255,255,255),2,8);

	  radius++;
	  t = tools.doesIntersect(shapes, circle_img);
	}while(!t);

      temp = tools.jitterCircle(seed, radius,shapes);

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
  myCircle.shape = Mat::zeros(shapes.size(), CV_8U);
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
  //namedWindow("circle U shapes", CV_WINDOW_FREERATIO);	       
   
  Point seed = tools.findCentroid(pSet);
  vector<Point> stack;
  stack.push_back(seed);
  while(stack.size() > 0){
    //Pop one from stack
    Point pt = stack.back();
    stack.pop_back();

    //insert circle   
    Circle myCircle = insertCircle(shapes, pt);	
    //find intersectpts and check how many there are
    vector<Point> intersectPoints;
    intersectPoints = tools.findIntersections(myCircle.shape,shapes);

    //temp Mat img to hold the unified image which we later apply to shapes
    Mat temp = myCircle.shape | shapes;
	
    

    if(intersectPoints.size() > 3)
      cout << "found more than 3 intersections" << endl;
    else if(intersectPoints.size() < 3)
      cout << "found less than 3 intersections" << endl;
    else if(intersectPoints.size() == 3)
      {
	//find midpts
	Point midpts[3];
	for(unsigned i = 0; i < intersectPoints.size(); i++)
	  midpts[i] = tools.findMidPoint(intersectPoints[i],intersectPoints[(i + 1) % 3]);
	//find vectors
	Vec2i vectors[3];
	for(unsigned i = 0; i < 3; i++)
	  vectors[i] = tools.makeVector(myCircle.center,midpts[i]);
	//Normalize vectors
	Vec2i normalizedVectors[3];
	for(unsigned i = 0; i < 3; i++)
	  normalizedVectors[i] = tools.normalize(vectors[i]);

	for(unsigned i = 0; i < intersectPoints.size(); i++)
	  {
	    Point tempPt = midpts[i];
	    bool validPt = false;
	    while(!validPt)
	      {
		if(tools.isInsideAndNotOn(tempPt,shapes)
		   && !tools.isInside (tempPt,myCircle.shape))
		  {
		    validPt = true;
		    stack.push_back(Point(tempPt.x,tempPt.y));
		  }
		//I suppose this could potentially fail if Your somehow end up always in the circle and not on the shape.
		//Karnaugh Map representation of this gives us the output.
		else if(!tools.isOn(tempPt,shapes) && tools.isInside(tempPt,myCircle.shape))
		  {
		    double x = tempPt.x + normalizedVectors[i][0];
		    double y = tempPt.y + normalizedVectors[i][1];
		  
		    tempPt = Point(x,y);
		  }
		else
		  {
		    validPt = true;
		    //exit loop without adding anything to Queue
		  }
	      }  
	  }
	temp.copyTo(shapes);
	imshow("shapes",shapes);
	waitKey(0);
      }
  }
  
  
  // for(unsigned i = 0; i < queue.size() && queue.size() > 0; i++)
  //   {
  //     circle(shapes,queue[i],20,Scalar(255,255,255),2,8);
  //   }
 
  //queue holds the points that we are gonna run insertCircle on
  //1.insertCircle
  //2.findIntercepts
  //3.find midpts
  //4.find vectors
  //5.find new pts to add to queue
  //6.repeat for the other points in the queue
    
  //queue.push_back();

  //all you really wanna do is do the insertCircle function on the unifies drawing with the correct seed point
  //if you can do that then this is a piece of cake.
  
  //For testing												//
  // //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // for(unsigned i = 0; i < intersectPoints.size(); i++)							//
  //   {													//
  //     circle(shapes,intersectPoints[i],20,Scalar(255,255,255),2,8);					//
  //   }													//
  //////////////////////////////////////////////////////////////////////////////////////////////////////////

  //For testing	
  //IF you want to see the circle, the shape, and the Union of the two. Otherwise these lines are uneeded really.
	

  // imshow("circle", myCircle.shape);									//
  // //

  // imshow("shape", shapes);										//
  // //
  // Mat circleUshapes = myCircle.shape + shapes;								//

  // imshow("circle U shapes", circleUshapes);								//
  // //
  // waitKey(0);
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

