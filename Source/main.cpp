
#include "MyFeatureDetector.h"

using namespace std;

int main()
{
	MyFeatureDetector* myDetector = new MyFeatureDetector();
	myDetector->init("Flowers/Shape1/shape1-001.ppm");
	//myDetector->drawCircle2();
	//myDetector->test();
	
	//MyTools tool = MyTools();
	//waitKey();
	//myDetector->drawCircle(0);
	//myDetector->drawCircle_man(0);

	//myDetector->drawMoreCircles(0,0);
	//cout << "hello worl0" << endl;

	myDetector->exit();
	delete myDetector;

	return 1;
}

