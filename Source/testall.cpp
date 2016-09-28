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

		bool testExtractAverageAngle(){return false;}
};

int main(){
	MyFeatureDetector detector = MyFeatureDetector();
	if(detector.init("Flowers/Shape1/shape1-001.ppm", 0) != 0)
	{
		detector.drawCircle2();
		detector.getCircles().size();
		detector.drawSkeleton(2);

		double tmpa = detector.extractAreaOfCircles();
		double tmpb = detector.extractAreaOfBoundRect();
		double tmpc = detector.extractAreaOfBoundHull();
		double tmpd = detector.extractHullLength();
		double tmpe = detector.extractSkeletonLength();
		double tmpf = detector.extractAverageDegreeOfSkeleton();
		double tmpg = detector.extractAverageAngleOfSkeleton();
		double tmph = detector.extractAreaOfFirstCircle();
		double tmpi = detector.extractDegreeOfFirstCircle();
		double tmpj = detector.extractNumberOfSkeletonNodes();
		double tmpk = detector.extractNumberOfHullNodes();
		double tmpl = detector.extractNumberOfMSTNodes();
		double tmpm = detector.extractMSTLength();
		double tmpn = detector.extractAverageAreaOfCircles();
		double tmpo = detector.extractAverageAngleOfMST();
		double tmpp = detector.extractAverageAngleOfHull();
		double tmpq = detector.extractRatioSkeletonDegreeLarge();
		double tmpr = detector.extractRatioMSTDegreeLarge();
		double tmps = detector.extractRatioSkeletonAreaLarge();
		double tmpt = detector.extractRatioCircleAreaHullArea();
		double tmpu = detector.extractRatioHullLengthSkeletonLength();
		double tmpv = detector.extractRatioMSTLengthSkeletonLength();
		double tmpw = detector.extractRatioAreaOfFirstCircleAndAverageAreaOfSkeleton();
		double tmpx = detector.extractRatioDegreeOfFirstCircleAndAverageDegreeOfSkeleton();

		
		detector.addFeature(tmpa);
		detector.addFeature(tmpb);
		detector.addFeature(tmpc);
		detector.addFeature(tmpd);
		detector.addFeature(tmpe);
		detector.addFeature(tmpf);
		detector.addFeature(tmpg);
		detector.addFeature(tmph);
		detector.addFeature(tmpi);
		detector.addFeature(tmpj);
		detector.addFeature(tmpk);
		detector.addFeature(tmpl);
		detector.addFeature(tmpm);
		detector.addFeature(tmpn);
		detector.addFeature(tmpo);
		detector.addFeature(tmpp);
		detector.addFeature(tmpq);
		detector.addFeature(tmpr);
		detector.addFeature(tmps);
		detector.addFeature(tmpt);
		detector.addFeature(tmpu);
		detector.addFeature(tmpv);
		detector.addFeature(tmpw);
		detector.addFeature(tmpx);

		for(int a = 0; a < detector.features.size(); a++)
			cout << detector.features[a] << endl;
	
		detector.showShape(0);
	}
}
