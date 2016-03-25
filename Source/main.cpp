
#include "MyFeatureDetector.h"

using namespace std;

int main()
{
	MyFeatureDetector* myDetector = new MyFeatureDetector();
	if(myDetector->init("Flowers/Shape1/shape1-000.ppm") != 0)
        {
       	myDetector->drawCircle2();
        myDetector->drawSkeleton(2);
        cout << "final shape" << endl;
        myDetector->showShape();
	//myDetector->test("FILE NAME");


	myDetector->exit();
        }
        else
            cout << "file could not be initialized" << endl;

	delete myDetector;

	return 1;
}

