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
  
  //bool test1 = tester.testFindIntersections();
  //cout << "findIntersections(): " << test1 << endl;

  Mat triangle = Mat::zeros(500,500,CV_8U);
  Mat image = Mat::zeros(500,500,CV_8U);

  Point pt1 = Point(0,20); Point pt2 = Point(500,20); Point pt3 = Point(250,500);
  line(triangle, pt1, pt2, Scalar(255,255,255), 1, 8);
  line(triangle, pt2,pt3,Scalar(255,255,255), 1, 8);
  line(triangle,pt3,pt1,Scalar(255,255,255),1,8);

  Circle testCircle = testMyFeatureDetectorClass->insertCircle(triangle, Point(250,250));
  //push intersections found between shape and circle into a set  
  vector<Point> intersections = testMyToolsClass->findIntersections(triangle, testCircle.shape);
  
  for(unsigned i = 0; i < intersections.size(); i++)
      circle(image, intersections[i], 10, Scalar(255,255,255), 1, 8);

  //push angles of the ray created between the center of the circle and the intersections into set radii
  vector<double> radii;
  for(unsigned i = 0; i < intersections.size(); i++)
  {
      radii.push_back(testMyToolsClass->findAngleOfRay(testCircle.center, intersections[i]));
      radii[i] = ((int)radii[i] + 360) % 360;
  }

  cout << intersections.size() << endl;
  //sort set radii
  sort(radii.begin(), radii.end());

  image = triangle | image | testCircle.shape;
  imshow("image", image);
  waitKey(0);
  vector<Point> seeds;
  for(unsigned i = 0; i < radii.size(); i++)
  {
      int j = (i + 1) % (int)radii.size();

      double theta = (radii[i] / 180) * 3.141592653;
      double x = testCircle.radius * cos(theta) + testCircle.center.x;
      double y = testCircle.radius * sin(theta) + testCircle.center.y;
      Point pt = Point(x,y);

      double theta2 = (radii[j]/180) * 3.141592653;
      x = testCircle.radius * cos(theta2) + testCircle.center.x;
      y = testCircle.radius * sin(theta2) + testCircle.center.y;
      Point pt2 = Point(x,y);

      Mat img = Mat::zeros(triangle.size(), CV_8U);
      line(img, testCircle.center, pt, Scalar(255,255,255), 1,8);
      line(img, testCircle.center, pt2, Scalar(255,255,255), 1, 8);
      img = img | triangle | testCircle.shape;
      imshow("rand seeds", img);
      waitKey(0);

      line(image,testCircle.center,pt,Scalar(255,255,255),2,8);
        cout << "radii[i]: " << radii[i] << endl;

      theta = radii[i] + 1 + (rand() % (int)(radii[j] - radii[i] - 1));
      theta = theta * 3.141592653 / 180;
      x = testCircle.radius * cos(theta) + testCircle.center.x;
      y = testCircle.radius * sin(theta) + testCircle.center.y;
      pt = Point(x,y);

      circle(img, pt, 30, Scalar(255,255,255), -1, 8);
      imshow("rand seeds", img);
      waitKey(0);

      seeds.push_back(pt);
  }
  
  for(unsigned i = 0; i < seeds.size(); i++)
    {
      circle(image, seeds[i], 10, Scalar(255,255,255), -1, 8);
    }
  cout << "seeds.size(): " << seeds.size() << endl;

  Circle c = testMyFeatureDetectorClass->insertCircle2(triangle, testCircle, seeds[0]);

  //have a direction to grow and a seed
  //turns out a normalized vector always has a length of 1! Lucky lucky
  //we take advantage of that and the fact that the growth of the circle will always be in the direction of the intersection from 
  //the first circle.
  
  

  image = (triangle | testCircle.shape) | image;
  namedWindow("image", CV_WINDOW_FREERATIO);
  imshow("image", image);
  waitKey(0);

  testMyFeatureDetectorClass->exit();
  delete testMyFeatureDetectorClass;
  delete testNodeClass;
  delete testCircleClass;
  delete testMyToolsClass;
  return 1;
}
