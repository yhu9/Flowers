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

bool MyFeatureDetector::init(char* imgname, bool flag)
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

		if(flag == 1)
		{
			namedWindow("blurred shape", CV_WINDOW_FREERATIO);
			imshow("blurred shape",detectedEdges);
			waitKey(0);
		}
		Canny(detectedEdges,detectedEdges,50,150,3);

		if(flag == 1)
		{
			namedWindow("detected edges", CV_WINDOW_FREERATIO);
			imshow("detected edges", detectedEdges);
			waitKey(0);
		}

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

		if(flag ==1)
		{
			Mat foundContours = Mat::zeros(shapes.size(), CV_8U);
			for(unsigned a = 0; a < contours.size(); a++)
				for(unsigned b = 0; b < contours[a].size(); b++)
					circle(foundContours,contours[a][b],1,Scalar(255,255,255), -1, 8);
			namedWindow("findContour", CV_WINDOW_FREERATIO);
			imshow("findContour",foundContours);
			waitKey(0);
		}

        //remove outliers
        contours = tools.removeOutliers(contours);

		if(flag == 1)
		{
			Mat outliersRemoved = Mat::zeros(shapes.size(), CV_8U);
			for(unsigned a = 0; a < contours.size(); a++)
				for(unsigned b = 0; b < contours[a].size(); b++)
					circle(outliersRemoved,contours[a][b],1,Scalar(255,255,255),-1,8);
			namedWindow("outliers removed", CV_WINDOW_FREERATIO);
			imshow("outliers removed", outliersRemoved);
			waitKey(0);
		}

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
        //cout << "set size: " << pSet.size() << endl;
        pSet = tools.reduction(pSet,12); //12 = radius of reduction size
		if(flag == 1)
		{
			Mat simplified = Mat::zeros(shapes.size(), CV_8U);
			for(unsigned a = 0; a < pSet.size(); a ++)
				circle(simplified, *pSet[a], 1, Scalar(255,255,255), -1, 8);
			namedWindow("simplified", CV_WINDOW_FREERATIO);
			imshow("simplified", simplified);
			waitKey(0);
		}

        //Create the nodes based on pSet
        //We also intialize the id of each node
        //The nodes are stored in a class variable called maps
        for(unsigned i = 0; i < pSet.size(); i++)
        {
            maps.push_back(new Node(*pSet[i]));
            maps[i]->id = new int(i);
        }
        //cout << "Nodes successfully created" << endl;

        //Apply Prim
        //Previously we did apply Dijkstra but there was no need for it
        tools.mapNodesPrim(maps);
        //cout << "Nodes mapped using prim" << endl;

        //Draw the shape on reducedImage all we do here is thicken each point in pSet
        for(unsigned i = 0; i < maps.size(); i++)
            circle(reducedImage,*pSet[i],1,Scalar(255,255,255),-1,8);
        for(unsigned i = 0; i < maps.size(); i++)
        {
            for(unsigned j=0; j < maps[i]->edge.size(); j++)
                line(reducedImage,maps[i]->pt,maps[i]->edge[j]->pt,Scalar(255,255,255),2,8);
        }

		if(flag == 1)
		{
			namedWindow("mapped mst", CV_WINDOW_FREERATIO);
			imshow("mapped mst", reducedImage);
			waitKey(0);
		}

		//attempt to close the shape
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
        //namedWindow("drawing",CV_WINDOW_FREERATIO);
        //namedWindow("hull", CV_WINDOW_FREERATIO);
        //imshow("drawing",drawing);
        //imshow("hull",img);
        //waitKey(0);
        namedWindow("reducedImage",CV_WINDOW_FREERATIO);
        imshow("reducedImage",reducedImage);
		waitKey(1);
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
	double jitterAmount = 3;
    while(tools.isOn(seed,shape))
	{
        seed = tools.jitterCircle(seed,2,jitterAmount,shape);
		jitterAmount +=2;
	}
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

        temp = tools.jitterCircle(seed, radius,3,shape);
		
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

//InsertCircle2 does things much differently.
//1. You can only insert circles with a prev circle
//2. A seed point from the previous circle must be specified to grow from
//3. To grow a normalized vector from the previous circle to the seed point is drawn
//4. The center of the circle is moved in the direction of the vector and the radius of the circle is grown
// at the same and at the same rate to ensure that the growing circle is in contact with the prev circle.
//5. Once the growing circle meets another edge, the circle is jittered along the edges of the previous circle
// until it does not intersect with it.
//6. The circle is then grown again.
//7. This process repeates until jittering the circle does not stop an intersection between the circle and an edge
//8. That circle is returned.
Circle MyFeatureDetector::insertCircle2(Mat shape, Circle prev, Point seed)
{
    //namedWindow("circle img", CV_WINDOW_FREERATIO);
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
		else
		{
			myCircle.radius = -1;
			return myCircle;
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
    Mat newShape = Mat::zeros(shapes.size(),CV_8U);
    newShape = newShape | shapes;
    Point seed = tools.findCentroid(pSet);
	deque<Circle> que;

	if(tools.isInside2(seed, newShape) || tools.isInside(seed,newShape)){
		Circle firstCircle = insertCircle(newShape, seed);
		if(firstCircle.radius > 0)
			que.push_back(firstCircle);
	}

	//we start by sorting the solution every single time. But, an alternative solution is possible
	//when we push new circle into the que,
	//pick a random spot on the que
	//from there, move the circle forward if it is greater than the spot
	//move the circle backwards if it is less than the spot
	//repeat until you hit a backward than forward or vise versa.
	//nlogn average running time. compared to n^2 which it is right now. I think? gotta check to be sure
	//c++ sort function
    while(que.size() > 0)
    {
        sort(que.begin(), que.end(), fooSortCircle());
        Circle myCircle = que.front();
        que.pop_front();
        Mat myCircle_shape = Mat::zeros(newShape.size(), CV_8U);
        circle(myCircle_shape, myCircle.center, myCircle.radius, Scalar(255,255,255), 1,8);
        vector<Point> intersections = tools.findIntersections(myCircle_shape, newShape);

		//test circle
		Mat tempCircle = Mat::zeros(newShape.size(),CV_8U);

		//prolly better if I make a variable called estimation for 0.60 to decrease the circle size.
		circle(tempCircle,myCircle.center, myCircle.radius * 0.60, Scalar(255,255,255),1,8);

        ////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //double check to be sure next circle is not on the newShape at all
		//intersections.size() > 1 checks to make sure the circle does touch the shape somewhere
		//intersections.size() < 30 checks to make sure the circle does not touch the shape or other circles too many times
		//!tools.isOn() checks to make sure the center of the circle is not directly on top of the newShape.
        if(intersections.size() > 1 && !tools.isOn(myCircle.center, newShape) && !tools.doesIntersect(tempCircle,newShape))
        {
			
            circles.push_back(myCircle);
            
            vector<Point> seeds;
			for(double i = 1; i <= 7; i++)
			{
				double x = myCircle.center.x + myCircle.radius * cos((3.14159 * 2) * (i/7));
				double y = myCircle.center.y + myCircle.radius * sin((3.14159 * 2) * (i/7));
				Point pt = Point(x,y);
				seeds.push_back(pt);
			}

            //while seeds > 0 pop a seed from the set and grow a circle from there.
            //If the circle is valid push it into the que.
			//int count = 0;
            while(seeds.size() > 0)
            {
                Point seed = seeds.back();
                seeds.pop_back();

                Circle c = insertCircle2(newShape, myCircle, seed);
                
				////////////////////////////////////////////////////////////////////////////////////////////////  
				//alternative sorting solution possible for speed of program
                if(c.radius != -1 && c.radius < newShape.cols)
                    if(!tools.isOn(c.center, newShape) && !tools.isOn(c.center, myCircle.shape))
					{
							que.push_back(c);
					}
            }//end while

            //update the newShape so we don't draw into the prev circles (Not sure if we might still draw into prev circles);
            //turns out we do...
            //NOTE: since we divide the growth of the circle by arcs we assume we won't pass into other circle's territories.
            newShape = newShape | myCircle.shape;

            /////////////////////////////////////////////////////////////////////////////////////////    
            namedWindow("new shape", CV_WINDOW_FREERATIO);
            imshow("new shape", newShape);

            //Mat circlesInQue = Mat::zeros(newShape.size(), CV_8U);
            //circlesInQue = newShape | circlesInQue;
            //for(unsigned i = 0; i < que.size(); i++)
            //    circlesInQue = circlesInQue | que[i].shape;
            //namedWindow("circles in que", CV_WINDOW_FREERATIO);
            //imshow("circles in que", circlesInQue);
            waitKey(1);
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

void MyFeatureDetector::addFeature(double f){
	features.push_back(f);
}

vector<Circle> MyFeatureDetector::getCircles(){
	return circles;
}

double MyFeatureDetector::extractAreaOfCircles(){
    double total = 0.0;
	double max = 0;
    for(unsigned i = 0; i < circles.size(); i++){
		double area = circles[i].calcArea();
        total += area;
		if(max < area)
			max = area;
    }
	
    return total;
}

double MyFeatureDetector::extractAreaOfBoundRect(){
    double total = bRect.width * bRect.height;
    return (double)total;
}

double MyFeatureDetector::extractAreaOfBoundHull(){
    double total = 0.0;
    vector<Point> tmp;
    for(unsigned i = 0; i < hull.size(); i++){
        tmp.push_back(*pSet[hull[i]]);
    }
    total = contourArea(tmp);
    return total;
}

double MyFeatureDetector::extractNumberOfSkeletonNodes(){
    return (double)circles.size();
}

double MyFeatureDetector::extractNumberOfHullNodes(){
    return (double)hull.size();
}

double MyFeatureDetector::extractNumberOfMSTNodes(){
	return (double)maps.size();
}

double MyFeatureDetector::extractHullLength(){
    double total = 0.0;
    for(int i = 0; i < (int)hull.size(); i++){
        int a = hull[i];
        int b = hull[(i + 1) % (int)hull.size()];
        total += tools.findDistance(pointSet[a],pointSet[b]);
    }
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
    return total;
}

double MyFeatureDetector::extractAverageDegreeOfSkeleton(){
    double total = 0.0;
    for(unsigned i = 0; i < skeletonMap.size(); i++){
        total += (double)skeletonMap[i]->edge.size();
    }
    total = total / (double) skeletonMap.size();
    return total;
}

double MyFeatureDetector::extractAverageDegreeOfMST(){
	double total = 0;
	for(unsigned a = 0; a < maps.size(); a++)
		total  += (double)maps[a]->edge.size();

	return total / (double)maps.size();
}

double MyFeatureDetector::extractAverageAreaOfCircles(){
	double total = 0;
	for(unsigned a = 0; a < circles.size(); a++)
		total += circles[a].calcArea();

	return total / (double)circles.size();
}

double MyFeatureDetector::extractAreaOfFirstCircle(){
    double total = circles[0].calcArea();
    return total;
}

double MyFeatureDetector::extractDegreeOfFirstCircle(){
    double total = (double)skeletonMap[0]->edge.size();
    return total;
}

double MyFeatureDetector::extractRatioSkeletonDegreeLarge(){
	double total = 0;
	double ratio = 0;
	double average = 0;
	for(unsigned a = 0; a < skeletonMap.size(); a++)
		total+= skeletonMap[a]->edge.size();

	average = total / (double)skeletonMap.size();

	double numerator; double denominator;
	for(unsigned a = 0; a < skeletonMap.size(); a++)
	{
		if((double)skeletonMap[a]->edge.size() >= average)
			numerator += 1;
		else
			denominator += 1;
	}

	ratio = numerator / denominator;
	return ratio;
}

double MyFeatureDetector::extractRatioMSTDegreeLarge(){
	double total = 0;
	double average = 0;
	for(unsigned a = 0; a < maps.size(); a++)
		total += maps[a]->edge.size();
	
	average = total / (double)maps.size();
	
	double numerator; double denominator;
	for(unsigned a = 0; a < maps.size(); a++)
	{
		if((double)maps[a]->edge.size() >= average)
			numerator += 1;
		else
			denominator += 1;
	}

	return numerator / denominator;
}

double MyFeatureDetector::extractRatioSkeletonAreaLarge(){
	double total = 0;
	double average = 0;

	for(unsigned a = 0; a < circles.size(); a++)
		total += circles[a].calcArea();
	average = total / (double)circles.size();
	
	double numerator; double denominator;
	for(unsigned a = 0; a < circles.size(); a++)
		if(circles[a].calcArea() >= average)
			numerator++;
		else
			denominator++;

	return numerator / denominator;
}

double MyFeatureDetector::extractRatioCircleAreaHullArea(){
	return extractAreaOfCircles() / extractAreaOfBoundHull();
}

double MyFeatureDetector::extractRatioHullLengthSkeletonLength(){

	return extractHullLength() / extractSkeletonLength();
}

double MyFeatureDetector::extractMSTLength(){
	double total = 0;
	for(unsigned a = 0; a < pSet.size(); a++)
	{
		for(unsigned b = 0; b < maps[a]->edge.size(); b++)
			total += tools.findDistance(maps[a]->pt, maps[a]->edge[b]->pt); 
	}

	return total;
}

double MyFeatureDetector::extractRatioMSTLengthSkeletonLength(){
	return extractMSTLength() / extractSkeletonLength();
}

double MyFeatureDetector::extractRatioHullLengthMSTLength(){
	return extractHullLength() / extractMSTLength();
}

double MyFeatureDetector::extractRatioAreaOfFirstCircleAndAverageAreaOfSkeleton(){
	return extractAreaOfFirstCircle() / extractAverageAreaOfCircles();
}

double MyFeatureDetector::extractRatioDegreeOfFirstCircleAndAverageDegreeOfSkeleton(){
	return extractDegreeOfFirstCircle() / extractAverageDegreeOfSkeleton();
}


void MyFeatureDetector::printSkeletonMap()
{
	for(unsigned a = 0; a < skeletonMap.size(); a++)
	{
		for(unsigned b = 0; b < skeletonMap[a]->edge.size(); b++)
		{
			Point p1 = skeletonMap[a]->pt;
			Point p2 = skeletonMap[a]->edge[b]->pt;
			cout << p1.x << "_" << p1.y << "," << p2.x << "_" << p2.y << "," << tools.findDistance(p1,p2) << endl;
		}
	}
}

//Feature extracts the average angle between three points less than 180 degrees inclusive.
//1. look for any node in the map with a degree of 2 or higher
//2. go through every possible combination of 3 points between the center node and its edges
//3. find the angle of the ray between pt1,pt2 and pt1,pt3 with pt1 as the center
//4. normalize the angles between 0-360
//5. find the angular distance of both rays
//6. find the average? or maybe some other statistical value could be better? only way to know for sure is to graph it all.
double MyFeatureDetector::extractAverageAngleOfSkeleton(){
    double total = 0.0;
    double n = 0;
    for(unsigned i = 0; i < skeletonMap.size(); i++){    
		for(unsigned j = 0; j < skeletonMap[i]->edge.size() - 1 && skeletonMap[i]->edge.size() >= 2; j++)
		{
			Point pt1 = skeletonMap[i]->pt;
			Point pt2 = skeletonMap[i]->edge[j]->pt;
			Point pt3 = skeletonMap[i]->edge[j+1]->pt;
			int degree12 = ((int)tools.findAngleOfRay(pt1,pt2) + 360) % 360;
			int degree23 = ((int)tools.findAngleOfRay(pt1,pt3) + 360) % 360;

			int angle = abs(degree12 - degree23);
			if(angle > 180)
				angle = abs(360 - angle);
			total += angle;
			n++;
		}
    }
    total = total / n;
    return total;
}

double MyFeatureDetector::extractAverageAngleOfMST(){
	double total = 0;
	double n = 0;
	for(unsigned a = 0; a < maps.size(); a++)
		for(unsigned b = 0; b < maps[a]->edge.size() - 1 && maps[a]->edge.size() >= 2; b++)
		{
			Point pt1 = maps[a]->pt;
			Point pt2 = maps[a]->edge[b]->pt;
			Point pt3 = maps[a]->edge[b+1]->pt;
			int degree12 = ((int)tools.findAngleOfRay(pt1,pt2) + 360) % 360;
			int degree23 = ((int)tools.findAngleOfRay(pt1,pt3) + 360) % 360;

			int angle = abs(degree12 - degree23);
			if(angle > 180)
				angle = abs(360 - angle);
			total += angle;
			n++;
		}
	if(n == 0)
		return -1;
	return (double)total / n;
}

double MyFeatureDetector::extractAverageAngleOfHull(){
	double total = 0;
	double n = 0;
	for(unsigned a = 0; a < hull.size(); a++)
	{	
		Point pt1 = pointSet[hull[(a-1+hull.size()) % hull.size()]];
		Point pt2 = pointSet[hull[a]];
		Point pt3 = pointSet[hull[(a+1) % hull.size()]];

		int degree12 = ((int)tools.findAngleOfRay(pt2,pt1) + 360) % 360;
		int degree23 = ((int)tools.findAngleOfRay(pt2,pt3) + 360) % 360;

		int angle = abs(degree12 - degree23);
		if(angle > 180)
			angle = abs(360-angle);
		total += angle;
		n++;
	}

	if(n==0)
		return -1;
	return (double)total / n;
}


void MyFeatureDetector::showShape(int time)
{
    Mat finalShape = shapes | skeleton;
	Mat boundingHulls = shapes | boundCircle | boundHull | boundRect;
    namedWindow("final shape", CV_WINDOW_FREERATIO);
    namedWindow("boundHull",CV_WINDOW_FREERATIO);
    imshow("final shape", finalShape);
    imshow("boundHull", boundHull);
    waitKey(time);
}
