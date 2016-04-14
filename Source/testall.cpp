#include "MyFeatureDetector.h"
#include "Node.h"
#include "Circle.h"

using namespace std;
using namespace cv;

class TestingClass
{
private:
  MyFeatureDetector* testObjectMyFeatureDetector;
  Node* testObjectNode;
  Circle* testObjectCircle;
  MyTools* testObjectMyTools;

public:
  TestingClass(){
    testObjectMyFeatureDetector = new MyFeatureDetector();
    testObjectNode = new Node();
    testObjectCircle = new Circle();
    testObjectMyTools = new MyTools();
  }
  ~TestingClass(){
    delete testObjectMyFeatureDetector;
    delete testObjectNode;
    delete testObjectCircle;
    delete testObjectMyTools;
  }
		
  //Test 

  //Test MyFeatureDetector class
  bool testInit(){return false;}
  bool testAddCircle(){return false;}
  bool testDrawCircle2(){return false;}
  bool testTraverseMap(){return false;}
  bool testInsertCircle(){
    Mat img1 = Mat::zeros(400,400,CV_8U);
    Mat img2 = Mat::zeros(400,400,CV_8U); 
    Mat img3 = Mat::zeros(400,400,CV_8U); 
    Mat img4 = Mat::zeros(400,400,CV_8U);
    Mat img5 = Mat::zeros(400,400,CV_8U);

    rectangle(img1,Point(0,0),Point(400,200),Scalar(255,255,255),2,8);
    rectangle(img2,Point(0,200),Point(400,400),Scalar(255,255,255),2,8);
    rectangle(img3,Point(0,0),Point(200,400),Scalar(255,255,255),2,8);
    rectangle(img4,Point(200,0),Point(400,400),Scalar(255,255,255),2,8);
    rectangle(img5,Point(100,100),Point(300,300),Scalar(255,255,255),2,8);

    Point center(200,200);

    Circle circle1 = testObjectMyFeatureDetector->insertCircle(img1,center);
    Circle circle2 = testObjectMyFeatureDetector->insertCircle(img2,center);
    Circle circle3 = testObjectMyFeatureDetector->insertCircle(img3,center);
    Circle circle4 = testObjectMyFeatureDetector->insertCircle(img4,center);
    Circle circle5 = testObjectMyFeatureDetector->insertCircle(img5,center);

    if(circle1.radius == -1 &&
       circle2.radius == -1 &&
       circle3.radius == -1 &&
       circle4.radius == -1 &&
       circle5.radius > 0 &&
       testObjectMyTools->isInside(circle5.center,img5))
      return true;

    return false;}
  bool testDrawSet(){return false;}
  bool testSeparateShape(){return false;}
  //Test Node class
  //only variables in the Node class

  //Test Circle class
  bool testCalcArea(){return false;}
  bool testCreatePoints(){return false;}
  bool testFindPoints(){return false;}

  //Test MyTools class
  bool testOutlineContour(){return false;}
  bool testFindAngleOfRay(){return false;}
  bool testFindDirectionOfRay(){return false;}
  bool testFindDistance(){return false;}
  bool testFindMidPoint(){return false;}
  bool testFindClosestPoint(){return false;}
  bool testFindSlope(){return false;}
  bool testFindPointID(){return false;}
  bool testFindCentroid(){return false;}
  bool testFindIntersections(){
    Mat circle1 = Mat::zeros(500,500,CV_8U);
    Mat circle2 = Mat::zeros(500,500,CV_8U);

    circle(circle1, Point(200,200),100, Scalar(255,255,255), 1, 8);
    circle(circle2, Point(300,300),100, Scalar(255,255,255), 1,8);

    if(testObjectMyTools->findIntersections(circle1,circle2).size() == 2)
      return true;
    return false;}
  bool testFindPtInShape(){return false;}
  bool testDoesIntersect(){
    Mat circle1 = Mat::zeros(500,500,CV_8U);
    Mat circle2 = Mat::zeros(500,500,CV_8U);

    circle(circle1, Point(200,200),100, Scalar(255,255,255), 1, 8);
    circle(circle2, Point(300,300),100, Scalar(255,255,255), 1,8);
   
    if(testObjectMyTools->doesIntersect(circle1,circle2))
      return true;
    return false;}
  bool testIsInside(){
    Mat circle1 = Mat::zeros(500,500,CV_8U);
    circle(circle1,Point(250,250),100,Scalar(255,255,255),1,8);
    bool test1 = testObjectMyTools->isInside(Point(250,250),circle1);
    bool test2 = testObjectMyTools->isInside(Point(500,500),circle1);
    if(test1 && !test2)
      return true;
    return false;
  }
  bool testCloseOpenContours(){return false;}
  bool testRemoveOutliers(){return false;}
  bool testReduction(){return false;}
  bool testCloseImageDijkstra(){return false;}
  bool testCloseImage(){return false;}
  bool testFindNodeId(){return false;}
  bool testMapNodesDijkstra(){return false;}
  bool testMapNodesPrim(){return false;}
  bool testFindClosestPath(){return false;}
  bool testFindAPath(){return false;}
  bool testTraversePath(){return false;}
  bool testFindAllPaths(){return false;}
  bool testUnionOfPoints(){return false;}
  bool testIntersectionOfPoints(){return false;}
  bool testSubractionOfPoints(){return false;}
  bool testRandomize(){return false;}
  bool testJitterCircle(){return false;}
};


int main()
{
  MyFeatureDetector* testMyFeatureDetectorClass = new MyFeatureDetector();
  Node* testNodeClass = new Node();
  Circle* testCircleClass = new Circle();
  MyTools* testMyToolsClass = new MyTools();
  TestingClass tester = TestingClass();
  
  //test MyFeatureDetector Class
  testMyFeatureDetectorClass->init("Flowers/testForMasa.png");


  testMyFeatureDetectorClass->exit();
  delete testMyFeatureDetectorClass;
  delete testNodeClass;
  delete testCircleClass;
  delete testMyToolsClass;
  return 1;
}
