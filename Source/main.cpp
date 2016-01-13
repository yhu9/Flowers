#include <iostream>
#include <opencv2/opencv.hpp>

#include "FeatureDetector.h"

void main()
{
	FeatureDetector* myDetector = new FeatureDetector();
	myDetector->init("Shape1/shape1-001.ppm");
	myDetector->drawCircle2();
	//myDetector->test();
	
	//MyTools tool = MyTools();
	//cv::waitKey();
	//myDetector->drawCircle(0);
	//myDetector->drawCircle_man(0);

	//myDetector->drawMoreCircles(0,0);
	//std::cout << "hello worl0" << std::endl;

	myDetector->exit();
	delete myDetector;
}