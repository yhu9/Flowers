
#include "MyFeatureDetector.h"

using namespace std;

int main()
{
	MyFeatureDetector* myDetector = new MyFeatureDetector();
	myDetector->init("Flowers/Shape1/shape1-000.ppm");
	myDetector->drawCircle();
	//myDetector->test("FILE NAME");


	myDetector->exit();
	delete myDetector;

	return 1;
}

