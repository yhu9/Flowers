#include "MyFeatureDetector.h"

using namespace cv;
using namespace std;

MyFeatureDetector::MyFeatureDetector(void)
{
}
MyFeatureDetector::~MyFeatureDetector(void)
{
}

//test() tests all the function in the MyFeatureDetector class
void MyFeatureDetector::test(char* imgname)
{

}

bool MyFeatureDetector::init(char* imgname)
{
    MyTools tools;
    //Initialize the image
    ifstream file;
    file.open(imgname);

    if(file.is_open())
    {

        shapes = imread(imgname,CV_8U);
        Mat hull_shape = Mat::zeros(shapes.size(), CV_8U);
        Mat detectedEdges = Mat::zeros(shapes.size(), CV_8U);
        Mat drawing = Mat::zeros(shapes.size(),CV_8U);
        Mat reducedImage = Mat::zeros(shapes.size(),CV_8U);

        //Not sure why but for some reason when I use canny I get a clear image of all the shapes but when I
        //run findContours() on shape1 it seems to change the image.
        //Finds the verticies of the image
        vector<vector<Point> > contours;
        vector<Vec4i> heirarchy;

        //Run Canny which highlights the verticies of the shape
        blur(shapes, detectedEdges, Size(3,3));

        Canny(detectedEdges,detectedEdges,50,150,3);

        //If you want to see the raw, blured, and detected edges
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //namedWindow("raw image", CV_WINDOW_FREERATIO);						//
        //imshow("raw image", shapes);									//
        //namedWindow("blured image", CV_WINDOW_FREERATIO);						//
        //imshow("blured image", detectedEdges);							//
        //namedWindow("detected edges", CV_WINDOW_FREERATIO);						//
        //imshow("detected edges", detectedEdges);							//
        //waitKey(0);											//
        //cout << "canny run successfully" << endl;							//
        //////////////////////////////////////////////////////////////////////////////////////////////////

        //Finds contours of the image
        //Basically verticies that are next to each other.
        //And draws it onto a drawing board with blue coloring
       
    //CV_RETR_EXTERNAL retrieves only the extreme outer contours. It sets hierarchy[i][2]=hierarchy[i][3]=-1 for all the contours.
    //CV_RETR_LIST retrieves all of the contours without establishing any hierarchical relationships.
    //CV_RETR_CCOMP retrieves all of the contours and organizes them into a two-level hierarchy. At the top level, there are external boundaries of the components. At the second level, there are boundaries of the holes. If there is another contour inside a hole of a connected component, it is still put at the top level.
    //CV_RETR_TREE retrieves all of the contours and reconstructs a full hierarchy of nested contours. This full hierarchy is built and shown in the OpenCV contours.c demo.
    //CV_CHAIN_APPROX_NONE stores absolutely all the contour points. That is, any 2 subsequent points (x1,y1) and (x2,y2) of the contour will be either horizontal, vertical or diagonal neighbors, that is, max(abs(x1-x2),abs(y2-y1))==1.
    //CV_CHAIN_APPROX_SIMPLE compresses horizontal, vertical, and diagonal segments and leaves only their end points. For example, an up-right rectangular contour is encoded with 4 points.
    //CV_CHAIN_APPROX_TC89_L1,CV_CHAIN_APPROX_TC89_KCOS applies one of the flavors of the Teh-Chin chain approximation algorithm. See [TehChin89] for details.

//offset – Optional offset by which every contour point is shifted. This is useful if the contours are extracted from the image ROI and then they should be analyzed in the whole  
        findContours(detectedEdges,contours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,Point());
        cout << "contours found" << endl;

        //remove outliers
        contours = tools.removeOutliers(contours);

        //Draw contour on drawing
        for(unsigned i = 0; i < contours.size(); i++)
            drawContours(drawing,contours,i,Scalar(255,255,255),0,8,heirarchy,0,Point());

        //Store contours created in previous step in pSet of class
        vector<Point> tempSet;
        for(unsigned i = 0; i < contours.size(); i++)
        {
            for(unsigned j = 0; j < contours[i].size(); j++)
            {
                tempSet.push_back(Point(contours[i][j]));
                pSet.push_back(new Point(contours[i][j]));
            }
        }

        //Reduce the set size of pSet or else we may end up with too many points in the set
        //we do not use variable contours anymore
        cout << "set size: " << pSet.size() << endl;
        pSet = tools.reduction(pSet,12); //12 = radius of reduction size
        cout << "set reduced to: " << pSet.size() << endl;

        //Create the nodes based on pSet
        //We also intialize the id of each node
        //The nodes are stored in a class variable called maps
        for(unsigned i = 0; i < pSet.size(); i++)
        {
            maps.push_back(new Node(*pSet[i]));
            maps[i]->id = new int(i);
        }
        cout << "Nodes successfully created" << endl;

        //Apply Prim
        //Previously we did apply Dijkstra but there was no need for it
        tools.mapNodesPrim(maps);
        cout << "Nodes mapped using prim" << endl;

        //Draw the shape on reducedImage all we do here is thicken each point in pSet
        for(unsigned i = 0; i < maps.size(); i++)
            circle(reducedImage,*pSet[i],1,Scalar(255,255,255),-1,8);
        for(unsigned i = 0; i < maps.size(); i++)
        {
            for(unsigned j=0; j < maps[i]->edge.size(); j++)
                line(reducedImage,maps[i]->pt,maps[i]->edge[j]->pt,Scalar(255,255,255),2,8);
        }
        cout << "image drawn on reduced set" << endl;

        //Close the shape
        tools.closeImage(maps,reducedImage);

        //create temp vector
        for(int i = 0; i < (int)pSet.size(); i++)
            pointSet.push_back(Point(pSet[i]->x,pSet[i]->y));

        //enclose the shape with a circle.
        Mat bCircle = Mat::zeros(drawing.size(), CV_8U);
        Point2f center = Point2f(0,0);
        float radius = 0;
        minEnclosingCircle(pointSet,center,radius);
        circle(bCircle, center, radius, Scalar(255,255,255), 2, 8);
        bCirc = Circle(center,radius);

        //enclose the shape with a rectangle.
        Mat rec_temp = Mat::zeros(drawing.size(),CV_8U);
        //for(int i = 0; i < (int)contours.size(); i++)
        bRect = boundingRect(pointSet);
        rectangle(rec_temp, bRect.tl(), bRect.br(), Scalar(255,255,255), 2, 8);

        //enclose the shape with a hull.
        convexHull(Mat(pointSet), hull, true);
        Mat img = Mat::zeros(drawing.size(), CV_8U);
        unsigned hullcount = hull.size();
        Point pt0 = pointSet[hull[hullcount-1]];
        for(unsigned i = 0; i < hullcount; i++ )
        {
            Point pt = pointSet[hull[i]];
            line(img, pt0, pt, Scalar(255,255,255), 1,LINE_AA);
            pt0 = pt;
        }

        ///////////////////////

        contourSet = contours;
        shapes = reducedImage;
        boundCircle = bCircle;
        boundRect = rec_temp;
        boundHull = img;

        //if you'd like you can show the basic images we created
        namedWindow("drawing",CV_WINDOW_FREERATIO);
        //namedWindow("hull", CV_WINDOW_FREERATIO);
        imshow("drawing",drawing);
        //imshow("hull",img);
        //waitKey(0);
        //namedWindow("reducedImage",CV_WINDOW_FREERATIO);
        //imshow("reducedImage",reducedImage);

        file.close();
        return 1;
    }
    return 0;
}
void MyFeatureDetector::exit()
{
    for(unsigned i = maps.size(); i > 0; i--)
    {
        delete maps[i-1]->id;
        delete maps[i-1];
        maps.erase(maps.begin() + (i-1));
    }

    for(unsigned i = pSet.size(); i > 0; i--)
    {
        delete pSet[i-1];
        pSet.erase(pSet.begin() + (i-1));
    }
}

//We get an error if the inscribed circle somehow ends up waaay off the screen.
Mat MyFeatureDetector::drawSet(vector<int> set)
{
    sort(set.begin(),set.end());
    Mat shape;
    shape = Mat::zeros(shapes.size(),CV_8U);
    for(unsigned i = 0; i < set.size(); i++)
    {
        for(unsigned j = 0; j < maps[set[i]]->edge.size(); j++)
        {
            //Check to see if the adj node is in set.
            if(binary_search(set.begin(),set.end(),*maps[set[i]]->edge[j]->id))
                line(shape,maps[set[i]]->pt, maps[set[i]]->edge[j]->pt,Scalar(255,255,255),2,8);
        }
    }

    return shape;
}

//insertCircle takes  a seed point and a shape and tries to insert a circle in the shape from the seed point.
//The circle will be the best fit upon jiterring a circle inside the shape.
//The function will return a Circle with a radius of -1 if the seed point isn't inside the shape.
//jittering the circle returns a point where the circle does not intersect any part of the shape.
Circle MyFeatureDetector::insertCircle(Mat shape, Point seed)
{
    while(tools.isOn(seed,shape))
        tools.jitterCircle(seed,5, shape);
    //bool isInside = tools.isInside(seed,shapes);
    bool isInside2 = tools.isInside2(seed,shapes);
    if(isInside2 == false){
        Circle falseCircle;
        falseCircle.radius = -1;
        return falseCircle;
    }

    Circle myCircle;

    bool largestCircle = false;
    bool t = false;
    double radius = 1;
    Point temp = seed;
    Mat circle_img;
    while(largestCircle == false)
    {
        do
        {
            circle_img = Mat::zeros(shape.size(),CV_8U);
            circle(circle_img,seed,radius,Scalar(255,255,255),2,8);

            radius++;
            t = tools.doesIntersect(shape, circle_img);
        }while(!t);

        temp = tools.jitterCircle(seed, radius,shape);

        if(temp.x == seed.x && temp.y == seed.y)
        {
            largestCircle = true;
            radius+=2;
        }
        else
            seed = temp;
    }

    myCircle.center = seed;
    myCircle.radius = radius;
    myCircle.shape = Mat::zeros(shape.size(), CV_8U);
    circle(myCircle.shape,seed,radius,Scalar(255,255,255),-1,8);

    return myCircle;
}

Circle MyFeatureDetector::insertCircle2(Mat shape, Circle prev, Point seed)
{
    namedWindow("circle img", CV_WINDOW_FREERATIO);
    Circle myCircle;
    if(tools.isOn(seed, shape))
    {
        myCircle.radius = -1;
        return myCircle;
    }
    bool isLargestCircle = false;
    double radius = 0;
    double count = 0;
    Point new_center_pt = Point(seed.x, seed.y);

    //Mat new_shape = Mat::zeros(shape.size(), CV_8U);
    //circle(new_shape, new_center_pt, 5, Scalar(255,255,255), -1, 8);
    //new_shape = shape | new_shape;
    //imshow("shape to insert on", new_shape);
    //waitKey(0);
    while(isLargestCircle == false)
    {
        Vec2d vec1 = tools.normalize(tools.makeVector(prev.center, new_center_pt));

        //find the new seed pt by where the vector intersects the circle
        Mat tempLine = Mat::zeros(shape.size(), CV_8U);
        Mat tempCircle = Mat::zeros(shape.size(), CV_8U);
        line(tempLine, prev.center, new_center_pt, Scalar(255,255,255), 1, 8);
        circle(tempCircle, prev.center, prev.radius, Scalar(255,255,255),2, 8);
        vector<Point> newSeedPt = tools.findIntersections(tempLine,tempCircle);

        //check if new circle is already at max size
        bool touchingShape = false;
        Mat circle_img = Mat::zeros(shape.size(), CV_8U);
        circle(circle_img, new_center_pt, radius, Scalar(255,255,255), 1, 8);
        touchingShape = tools.doesIntersect(shape, circle_img);

        if(newSeedPt.size() > 0)
        {
            while(touchingShape == false)
            {
                count += 1;
                circle_img = Mat::zeros(shape.size(), CV_8U);

                double x = newSeedPt[0].x + vec1[0] * count;
                double y = newSeedPt[0].y + vec1[1] * count;
                new_center_pt = Point(x,y);

                radius = tools.findDistance(new_center_pt, newSeedPt[0]);
                circle(circle_img, new_center_pt, radius, Scalar(255,255,255),1, 8);
                touchingShape = tools.doesIntersect(shape, circle_img);

                //if it failes both definition of inside a shape, return a -1
                if(!tools.isInside2(new_center_pt, shapes))
                {
                    if(!tools.isInside(new_center_pt,shapes))
                    {
                        myCircle.radius = -1;
                        return myCircle;
                    }
                }

                //////////////////////////////////////////////////////////////////////
                //Mat temp = Mat::zeros(shape.size(), CV_8U);
                //temp = circle_img | shape | prev.shape;
                //imshow("circle img", temp);
                //waitKey(1);
                //////////////////////////////////////////////////////////////////////
            }

            //find new center of new circle
            Point temppt = tools.jitterCircle(prev.center, prev.radius, new_center_pt, radius, shape);
            if(new_center_pt.x == temppt.x && temppt.y == new_center_pt.y)
            {
                isLargestCircle = true;
                radius += 2.5;
            }
            else
                new_center_pt = temppt;
        }
    }

    myCircle.center = new_center_pt;
    myCircle.radius = radius;
    myCircle.shape = Mat::zeros(shape.size(), CV_8U);
    circle(myCircle.shape, myCircle.center, myCircle.radius, Scalar(255,255,255), -1, 8);

    return myCircle;
}

//1. insert a circle in the centroid of the shape
//2. push this circle into the que
//3. while the que isn't empty
//4. pop one circle from the que
//3. find the intersections between the circle and the shape
//4. cut the circle into multiple arcs
//5. pick a random spot on these arcs make sure the random spot is not on the shape
//6. grow a circle from these spots
//7. add these circles to the que
//8. repeat from step 3
//
//reject bad circles on step 6. The circles get rejected if the follwing happens:
//1. the center of the circle is outside of the shape
//2. if the circle is both inside and outside the shape
//3. if the circle grows into another circle
struct fooSortCircle
{
    inline bool operator() (const Circle& s1, const Circle& s2)
    {
        return (s1.radius > s2.radius);
    }
};

void MyFeatureDetector::drawCircle2()
{
    //namedWindow("my circle", CV_WINDOW_FREERATIO);
    Mat newShape = Mat::zeros(shapes.size(),CV_8U);
    newShape = newShape | shapes;
    Point seed = tools.findCentroid(pSet);

    Circle firstCircle = insertCircle(newShape, seed);
    deque<Circle> que;
    que.push_back(firstCircle);
    while(que.size() > 0)
    {
        sort(que.begin(), que.end(), fooSortCircle());
        Circle myCircle = que.front();
        que.pop_front();
        Mat myCircle_shape = Mat::zeros(newShape.size(), CV_8U);
        circle(myCircle_shape, myCircle.center, myCircle.radius, Scalar(255,255,255), 1,8);
        vector<Point> intersections = tools.findIntersections(myCircle_shape, newShape);

        ////////////////////////////////////////////////////////////////////////////////////////////////
        //cout << "intersection.size(): " << intersections.size() << endl;
        //Mat my_circle = Mat::zeros(newShape.size(), CV_8U);
        //my_circle = myCircle.shape | newShape;
        //imshow("my circle", my_circle);
        //waitKey(0);
        ////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //double check to be sure next circle is not on the newShape at all
        if(intersections.size() > 1 && !tools.isOn(myCircle.center, newShape))
        {
            circles.push_back(myCircle);
            vector<double> radii;
            for(unsigned i = 0; i < intersections.size(); i++)
            {
                radii.push_back(tools.findAngleOfRay(myCircle.center,intersections[i]));
                //http://stackoverflow.com/questions/1311049/how-to-map-atan2-to-degrees-0-360
                radii[i] = ((int)radii[i] + 360) % 360;
            }

            //sort radii after converting everything to positive values
            sort(radii.begin(),radii.end());

            //find the averageDegree of radii
            double averageDegree = 0;
            for(int i = 0; i < (int)radii.size(); i++)
            {
                int j = (i + 1) % (int)radii.size();
                if(j < i)
                    averageDegree += abs(radii[j] + 360 - radii[i]);
                else
                    averageDegree += abs(radii[j] - radii[i]);
            }
            averageDegree = averageDegree / radii.size();

            //Find the midpoint of the arcs. if there are less than 3 arcs or the arcs are greater than the average degree
            vector<Point> seeds;
            for(int i = 0; i < (int)radii.size(); i++)
            {
                int j = (i + 1) % (int)radii.size();
                double theta = 0;
                double diff = 0;
                //if radii[j] < radii[i]
                if(j < i)
                    diff = radii[j] + 360 - radii[i];
                else
                    diff = radii[j] - radii[i];
                if(!(radii.size() > 3 && diff < averageDegree))
                {
                    theta = radii[i] + diff / 2;
                    //Point to start circle growth from
                    theta = theta /180 * 3.141592653;
                    double x = myCircle.center.x + myCircle.radius * cos(theta);
                    double y = myCircle.center.y + myCircle.radius * sin(theta);
                    Point pt = Point(x,y);

                    seeds.push_back(pt);
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////
            //cout << "seeds.size(): " << seeds.size() << endl;
            //Mat my_circle = Mat::zeros(newShape.size(), CV_8U);
            //my_circle = newShape | my_circle | myCircle.shape;
            //for(unsigned i = 0; i < seeds.size(); i++)
            //    circle(my_circle, seeds[i], 5, Scalar(255,255,255), -1, 8); 
            //imshow("my circle", my_circle);
            //waitKey(1);
            ////////////////////////////////////////////////////////////////////////////////////////////////

            //while seeds > 0 pop a seed from the set and grow a circle from there.
            //If the circle is valid push it into the que.
            while(seeds.size() > 0)
            {
                Point seed = seeds.back();
                seeds.pop_back();

                Circle c = insertCircle2(newShape, myCircle, seed);
                ////////////////////////////////////////////////////////////////////////////////////////////////
                //my_circle = Mat::zeros(my_circle.size(), CV_8U);    
                //my_circle = newShape | my_circle;
                //circle(my_circle, c.center, c.radius, Scalar(255,255,255), 1, 8);
                //imshow("my circle", my_circle);
                //    waitKey(0);
                ////////////////////////////////////////////////////////////////////////////////////////////////                   
                if(c.radius != -1 && c.radius < newShape.cols)
                    if(!tools.isOn(c.center, newShape) && !tools.isOn(c.center, myCircle.shape))
                        que.push_back(c);
            }//end while

            //update the newShape so we don't draw into the prev circles (Not sure if we might still draw into prev circles);
            //turns out we do...
            //NOTE: since we divide the growth of the circle by arcs we assume we won't pass into other circle's territories.
            newShape = newShape | myCircle.shape;

            /////////////////////////////////////////////////////////////////////////////////////////    
            //namedWindow("new shape", CV_WINDOW_FREERATIO);
            //imshow("new shape", newShape);

            //Mat circlesInQue = Mat::zeros(newShape.size(), CV_8U);
            //circlesInQue = newShape | circlesInQue;
            //for(unsigned i = 0; i < que.size(); i++)
            //    circlesInQue = circlesInQue | que[i].shape;
            //namedWindow("circles in que", CV_WINDOW_FREERATIO);
            //imshow("circles in que", circlesInQue);
            //waitKey(1);
            /////////////////////////////////////////////////////////////////////////////////////////
        }//end if
    }//end while
}//end function drawCircle2

void MyFeatureDetector::drawSkeleton(int fudge)
{
    skeleton = Mat::zeros(shapes.size(), CV_8U);
    for(unsigned i = 0; i < circles.size(); i++)
        skeletonMap.push_back(new Node(Point(circles[i].center)));
    for(unsigned i = 0; i < circles.size(); i++)
    {
        for(unsigned j = 0; j < circles.size(); j++)
        {
            if(i != j)
            {
                Mat c1 = Mat::zeros(shapes.size(), CV_8U);
                Mat c2 = Mat::zeros(shapes.size(), CV_8U);
                circle(c1, circles[i].center, circles[i].radius, Scalar(255,255,255), fudge, 8);
                circle(c2, circles[j].center, circles[j].radius, Scalar(255,255,255), fudge, 8);
                Mat tempLine = Mat::zeros(shapes.size(), CV_8U);
            
                if(tools.doesIntersect(c1, c2))
                {
                        line(tempLine, circles[i].center, circles[j].center, Scalar(255,255,255), 2, 8);
                        if(!tools.doesIntersect(tempLine,shapes))
                        {
                            skeletonMap[i]->edge.push_back(skeletonMap[j]);
                            skeleton = skeleton | tempLine;
                        }
                }
            }
        }
        circle(skeleton, circles[i].center, 5, Scalar::all(255), -1, 8);
        //circle(skeleton, circles[i].center, circles[i].radius, Scalar(255,255,255), 2, 8);
    }
}

double MyFeatureDetector::extractAreaOfCircles(){
    double total = 0.0;
    for(unsigned i = 0; i < circles.size(); i++){
        total += circles[i].calcArea();
    }

    features.push_back(total);
    return total;
}

double MyFeatureDetector::extractAreaOfBoundRect(){
    double total = bRect.width * bRect.height;
    features.push_back(total);
    return (double)total;
}

double MyFeatureDetector::extractAreaOfBoundHull(){
    double total = 0.0;
    vector<Point> tmp;
    for(unsigned i = 0; i < hull.size(); i++){
        tmp.push_back(*pSet[hull[i]]);
    }
    total = contourArea(tmp);
    features.push_back(total);
    return total;
}

double MyFeatureDetector::extractNumberOfSkeletonNodes(){
    features.push_back((double)circles.size());
    return (double)circles.size();
}

double MyFeatureDetector::extractNumberOfHullNodes(){
    features.push_back((double)circles.size());
    return (double)hull.size();
}

double MyFeatureDetector::extractHullLength(){
    double total = 0.0;
    for(int i = 0; i < (int)hull.size(); i++){
        int a = hull[i];
        int b = hull[(i + 1) % (int)hull.size()];
        total += tools.findDistance(pointSet[a],pointSet[b]);
    }
    features.push_back(total);
    return total;
}

double MyFeatureDetector::extractSkeletonLength(){
    double total = 0.0;
    for(unsigned i = 0; i < skeletonMap.size(); i++){
        double d = 0.0;
        for(unsigned j = 0; j < skeletonMap[i]->edge.size(); j++)
        {
            d += tools.findDistance(skeletonMap[i]->pt,skeletonMap[i]->edge[j]->pt);

        }
        total += d / 2;
    }
    features.push_back(total);
    return total;
}

double MyFeatureDetector::extractAverageDegree(){
    double total = 0.0;
    for(unsigned i = 0; i < skeletonMap.size(); i++){
        total += (double)skeletonMap[i]->edge.size();
    }
    total = total / (double) skeletonMap.size();
    features.push_back(total);
    return total;
}

double MyFeatureDetector::extractAverageAngle(){
    double total = 0.0;
    double n = 0;
    for(unsigned i = 0; i < skeletonMap.size(); i++){    
        double a = skeletonMap[i]->edge.size();
        if(a != 0)
        {
            total += 360 / a;
            n++;
        }
    }
    cout << total << n << endl;
    total = total / n;
    features.push_back(total);
    return total;
}

double MyFeatureDetector::extractAreaOfFirstCircle(){
    double total = circles[0].calcArea();
    features.push_back(total);
    return total;
}

double MyFeatureDetector::extractDegreeOfFirstCircle(){
    double total = (double)skeletonMap[0]->edge.size();
    features.push_back(total);
    return total;
}

void MyFeatureDetector::showShape()
{
    Mat finalShape = shapes | boundCircle | boundRect | boundHull | skeleton;
    namedWindow("final shape", CV_WINDOW_FREERATIO);
    namedWindow("boundHull",CV_WINDOW_FREERATIO);
    namedWindow("skeleton", CV_WINDOW_FREERATIO);
    imshow("final shape", finalShape);
    imshow("boundHull", boundHull);
    imshow("skeleton", skeleton);
    waitKey(0);
}
