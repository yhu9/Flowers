
#include "MyFeatureDetector.h"

using namespace std;

int main()
{
    MyFeatureDetector* myDetector = new MyFeatureDetector();
    if(myDetector->init("Flowers/Shape3/shape3-000.ppm") != 0)
    {
        cout << "shape 1" << endl;
        myDetector->drawCircle2();
        myDetector->drawSkeleton(2);
        double a = myDetector->extractAreaOfCircles();
        double b = myDetector->extractAreaOfBoundRect();
        double c = myDetector->extractAreaOfBoundHull();
        double d = myDetector->extractHullLength();
        double e = myDetector->extractSkeletonLength();
        double f = myDetector->extractAverageDegree();
        double g = myDetector->extractAverageAngle();
        double h = myDetector->extractAreaOfFirstCircle();
        double i = myDetector->extractDegreeOfFirstCircle();
        double j = myDetector->extractNumberOfSkeletonNodes();
        double k = myDetector->extractNumberOfHullNodes();
        cout << "circles area: " << a << endl;
        cout << "brect area: " << b << endl;
        cout << "bHull area: " << c << endl;
        cout << "hull length: " << d << endl;
        cout << "skeleton length: " << e << endl;
        cout << "average degree: " << f << endl;
        cout << "average angle: " << g << endl;
        cout << "area of first circle: " << h << endl;
        cout << "degree of first node: " << i << endl;
        cout << "number of skeleton nodes: " << j << endl;
        cout << "number of hull nodes: " << k << endl;
        //cout << "final shape" << endl;
        //myDetector->showShape();
        //myDetector->test("FILE NAME");


        myDetector->exit();
    }
    else
        cout << "file could not be initialized" << endl;
    delete myDetector;

    myDetector = new MyFeatureDetector();
    cout << endl;
    if(myDetector->init("Flowers/Shape1/shape1-001.ppm") != 0)
    {
        cout << "shape 1" << endl;
        myDetector->drawCircle2();
        myDetector->drawSkeleton(2);
        double a = myDetector->extractAreaOfCircles();
        double b = myDetector->extractAreaOfBoundRect();
        double c = myDetector->extractAreaOfBoundHull();
        double d = myDetector->extractHullLength();
        double e = myDetector->extractSkeletonLength();
        double f = myDetector->extractAverageDegree();
        double g = myDetector->extractAverageAngle();
        double h = myDetector->extractAreaOfFirstCircle();
        double i = myDetector->extractDegreeOfFirstCircle();
        double j = myDetector->extractNumberOfSkeletonNodes();
        double k = myDetector->extractNumberOfHullNodes();
        
        cout << "circles area: " << a << endl;
        cout << "brect area: " << b << endl;
        cout << "bHull area: " << c << endl;
        cout << "hull length: " << d << endl;
        cout << "skeleton length: " << e << endl;
        cout << "average degree: " << f << endl;
        cout << "average angle: " << g << endl;
        cout << "area of first circle: " << h << endl;
        cout << "degree of first node: " << i << endl;
        cout << "number of skeleton nodes: " << j << endl;
        cout << "number of hull nodes: " << k << endl;
        //cout << "final shape" << endl;
        //myDetector->showShape();
        //myDetector->test("FILE NAME");


        myDetector->exit();
    }
    else
        cout << "file could not be initialized" << endl;
    delete myDetector;

    return 1;
}

