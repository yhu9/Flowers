
#include "MyFeatureDetector.h"

using namespace std;

int main()
{
	MyFeatureDetector* myDetector = new MyFeatureDetector();
	myDetector->init("Flowers/Shape1/shape1-003.ppm");
       	myDetector->drawCircle2();
        myDetector->drawSkeleton(2);
        cout << "final shape" << endl;
        myDetector->showShape();
	//myDetector->test("FILE NAME");


	myDetector->exit();
	delete myDetector;

	return 1;
}

