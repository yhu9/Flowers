#include "MyFeatureDetector.h"
#include "Node.h"
#include "Circle.h"
#include "MyTools.h"

public class testingClass
{
	private:
		MyFeatureDetector* testObjectMyFeatureDetector;
		Node* testObjectNode;
		Circle* testObjectCircle;
		MyTools* testObjectMyTools;

	public:
		void testingClass(){
			testObjectMyFeatureDetector = new MyFeatureDetector();
			testObjectNode = new Node();
			testObjectCircle = new Circle();
			testObjectMyTools = new MyTools();
		}
		void ~testingClass(){
			delete testObjectMyFeatureDetector;
			delete testObjectNode;
			delete testObjectCircle;
			delete testObjectMyTools;
		}
		
		//Test 

		//Test MyFeatureDetector class
		bool testInit(){};
		bool testAddCircle(){};
		bool testDrawCircle2(){};
		bool testTraverseMap(){};
		bool testInsertCircle(){};
		bool testDrawSet(){};
		bool testSeparateShape(){};


		//Test Node class
		//only variables in the Node class

		//Test Circle class
		bool testCalcArea(){};
		bool testCreatePoints(){};
		bool testFindPoints(){};

		//Test MyTools class
		bool testOutlineContour(){};
		bool testFindAngleOfRay(){};
		bool testFindDirectionOfRay(){};
		bool testFindDistance(){};
		bool testFindDistance(){};
		bool testFindMidPoint(){};
		bool testFindClosestPoint(){};
		bool testFindSlope(){};
		bool testFindPointID(){};
		bool testFindCentroid(){};
		bool testFindInterSection(){};
		bool testFindPtInShape(){};
		bool testDoesIntersect(){};
		bool testCloseOpenContours(){};
		bool testRemoveOutliers(){};
		bool testReduction(){};
		bool testCloseImageDijkstra(){};
		bool testCloseImage(){};
		bool testFindNodeId(){};
		bool testMapNodesDijkstra(){};
		bool testMapNodesPrim(){};
		bool testFindClosestPath(){};
		bool testFindAPath(){};
		bool testTraversePath(){};
		bool testFindAllPaths(){};
		bool testUnionOfPoints(){};
		bool testIntersectionOfPoints(){};
		bool testSubractionOfPoints(){};
		bool testRandomize(){};
		bool testJitterCircle(){};
}


int main()
{
	MyFeatureDetector* testMyFeatureDetectorClass = new MyFeatureDetector();
	Node* testNodeClass = new Node();
	Circle* testCircleClass = new Circle();
	MyTools* testMyToolsClass = new MyTools();

	//test MyFeatureDetector Class
	testMyFeatureDetectorClass->init("Flowers/testForMasa.png");




	delete testMyFeatureDetectorClass;
	delete testNodeClass;
	delete testCircleClass;
	delete testMyToolsClass;
	return 1;
}
