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
    MyTools tools;
    //Initialize the image
    ifstream file;
    file.open(imgname);

    if(file.is_open())
    {

        shapes = imread(imgname,CV_8U);
        Mat detectedEdges = Mat::zeros(shapes.size(), CV_8U);
        Mat drawing = Mat::zeros(shapes.size(),CV_8U);
        Mat reducedImage = Mat::zeros(shapes.size(),CV_8U);


        namedWindow("raw image", CV_WINDOW_FREERATIO);
        imshow("raw image", shapes);

        //Not sure why but for some reason when I use canny I get a clear image of all the shapes but when I
        //run findContours() on shape1 it seems to change the image.
        //Finds the verticies of the image
        vector<vector<Point> > contours;
        vector<Vec4i> heirarchy;

        //Run Canny which highlights the verticies of the shape
        blur(shapes, detectedEdges, Size(3,3));
        namedWindow("blured image", CV_WINDOW_FREERATIO);
        imshow("blured image", detectedEdges);


        Canny(detectedEdges,detectedEdges,50,150,3);
        namedWindow("detected edges", CV_WINDOW_FREERATIO);
        imshow("detected edges", detectedEdges);

        cout << "canny run successfully" << endl;

        drawing = drawing + detectedEdges;
        namedWindow("drawing U detectedEdges", CV_WINDOW_FREERATIO);
        imshow("drawing U detectedEdges", drawing);

        //Finds contours of the image
        //Basically verticies that are next to each other.
        //And draws it onto a drawing board with blue coloring
        findContours(detectedEdges,contours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,Point());
        cout << "contours found" << endl;
        //contours = tools.removeOutliers(contours);

        file.close();
    }
    else
        cout << "failed to open" << imgname << endl;
}

void MyFeatureDetector::init(char* imgname)
{
    MyTools tools;
    //Initialize the image
    ifstream file;
    file.open(imgname);

    if(file.is_open())
    {

        shapes = imread(imgname,CV_8U);
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
        findContours(detectedEdges,contours,heirarchy,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,Point());
        cout << "contours found" << endl;

        contours = tools.removeOutliers(contours);

        contourSet = contours;

        //Draw contour on drawing
        for(unsigned i = 0; i < contours.size(); i++)
            drawContours(drawing,contours,i,Scalar(255,255,255),0,8,heirarchy,0,Point());

        //Store contours created in previous step in pSet of class
        for(unsigned i = 0; i < contours.size(); i++)
        {
            for(unsigned j = 0; j < contours[i].size(); j++)
                pSet.push_back(new Point(contours[i][j]));
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

        //tools contain the functions made by Masa Hu
        ////////////////////////////////////
        //////////////////////////////
        ///////////////////////

        contourSet = contours;
        shapes = reducedImage;

        //if you'd like you can show the basic images we created
        namedWindow("drawing",CV_WINDOW_FREERATIO);
        imshow("drawing",drawing);
        waitKey(0);
        //namedWindow("reducedImage",CV_WINDOW_FREERATIO);
        //imshow("reducedImage",reducedImage);

        file.close();
    }
    else
    {
        cout << "failed to open" << imgname << endl;
    }
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
    bool isInside = tools.isInside(seed,shapes);
    bool isOn = tools.isOn(seed,shape);
    if(isInside == false || isOn == true){
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
    //namedWindow("shape to insert on", CV_WINDOW_FREERATIO);
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
        circle(tempCircle, prev.center, prev.radius, Scalar(255,255,255), 2, 8);
        vector<Point> newSeedPt = tools.findIntersections(tempLine,tempCircle);

        //check if new circle is already at max size
        bool touchingShape = false;
        Mat circle_img = Mat::zeros(shape.size(), CV_8U);
        circle(circle_img, new_center_pt, radius, Scalar(255,255,255), 1, 8);
        touchingShape = tools.doesIntersect(shape, circle_img);

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

            if(!tools.isInside2(new_center_pt, shapes))
            {
                myCircle.radius = -1;
                return myCircle;
            }
            
            //////////////////////////////////////////////////////////////////////
            Mat temp = Mat::zeros(shape.size(), CV_8U);
            temp = circle_img | shape | prev.shape;
            imshow("circle img", temp);
            waitKey(10);
            //////////////////////////////////////////////////////////////////////
        }

        //find new center of new circle
        Point temppt = tools.jitterCircle(prev.center, prev.radius, new_center_pt, radius, shape);
        if(new_center_pt.x == temppt.x && temppt.y == new_center_pt.y)
        {
            isLargestCircle = true;
            radius += 2;
        }
        else
            new_center_pt = temppt;
    }

    myCircle.center = new_center_pt;
    myCircle.radius = radius;
    myCircle.shape = Mat::zeros(shape.size(), CV_8U);
    circle(myCircle.shape, myCircle.center, myCircle.radius, Scalar(255,255,255), -1, 8);

    return myCircle;
}


//Truth Table for acceptable points
//a = isOn Shape; b = isOn Circle; c = isInside Shape; d = isInside Circle
//------------------------------------------------------------------------
//a|b|c|d|result
//--------------
//0|0|0|0|exit
//0|0|0|1|invalid
//0|0|1|0|Q.add
//0|0|1|1|increment
//0|1|0|0|exit
//0|1|0|1|invalid
//0|1|1|0|invalid
//0|1|1|1|increment
//exit on all other states

//1. create 8 different directions
//2. push centroid of the shape into the que
//3. push all 8 directions into a vector
//4. loop while que isn't empty
//5. pop one point from que and try to insert a circle
//6. if the circle is acceptable then draw the circle on the window
//7. then check to see if new acceptable points can be found in the 8 directions
//8. Push all acceptable points onto the que.
//9. repeat loop from step 4.

void MyFeatureDetector::drawCircle()
{
    //namedWindow("circle", CV_WINDOW_FREERATIO);			
    namedWindow("shapes",CV_WINDOW_FREERATIO);			     
    namedWindow("circles to add", CV_WINDOW_FREERATIO);     
    Mat circlestoadd = Mat::zeros(shapes.size(), CV_8U);
    Mat newShape = Mat::zeros(shapes.size(),CV_8U);
    newShape = shapes | newShape;

    Point seed = tools.findCentroid(pSet);
    queue<Point> que;
    que.push(seed);

    Vec2d vec1 = Vec2d(1,0);
    Vec2d vec2 = Vec2d(0,1);
    Vec2d vec3 = Vec2d(-1,0);
    Vec2d vec4 = Vec2d(0,-1);
    Vec2d vec5 = Vec2d(1,1);
    Vec2d vec6 = Vec2d(-1,-1);
    Vec2d vec7 = Vec2d(-1,1);
    Vec2d vec8 = Vec2d(1,-1);

    vector<Vec2d> normalizedVectors;
    normalizedVectors.push_back(vec1);
    normalizedVectors.push_back(vec2);
    normalizedVectors.push_back(vec3);
    normalizedVectors.push_back(vec4);
    normalizedVectors.push_back(vec5);
    normalizedVectors.push_back(vec6);
    normalizedVectors.push_back(vec7);
    normalizedVectors.push_back(vec8);
    while(que.size() > 0){
        //Pop one from que
        Point pt = que.front();
        que.pop();

        //insert circle  
        //check to see if you can insert the circle correctly. 
        vector<Point> intersectPoints;
        Circle myCircle = insertCircle(newShape, pt);
        if(myCircle.radius != -1)
            intersectPoints = tools.findIntersections(myCircle.shape,newShape);

        //Make sure your circle isn't inside any of the old circles you found already.
        bool isInsideCircles = false;
        for(unsigned j = 0; j < circles.size(); j++)
        {
            if(tools.isInside(myCircle.center,circles[j].shape))
                isInsideCircles = true;
        }

        bool isOutsideShape = false;
        if(myCircle.center.x > shapes.cols || myCircle.center.y > shapes.rows)
            isOutsideShape = true;
        //Finding a circle that can be inserted with 3 or more intersections is the criteria of acceptance for the insertion
        //of a circle into the shape. The circle will be inside the shape && notOn newShape
        //new circles to be accepted cannot be inside any of the accepted circles.
        if(intersectPoints.size() < 3 || isInsideCircles == true || isOutsideShape == true)
            cout << "found less than 3 intersections or is inside one of the other circles or is Outside shape." << endl;
        else if(intersectPoints.size() >= 3)
        {
            cout << "circle accepted" << endl;
            Circle copy = Circle(myCircle.center, myCircle.radius, myCircle.shape);
            circles.push_back(copy);
            //If everything goes well we update newShape
            newShape = newShape + myCircle.shape;

            unsigned m = normalizedVectors.size();

            for(unsigned i = 0; i < m; i++)
            {
                Point tempPt = myCircle.center;
                bool exitLoop = false;
                while(!exitLoop)
                {
                    //isInside(shapes && !isOn(newShape) && !isInside(circles)
                    if(tools.isInside(tempPt,shapes)
                            && !tools.isOn(tempPt,newShape)
                            && !tools.isInside(tempPt,myCircle.shape))
                    {
                        exitLoop = true;
                        que.push(Point(tempPt.x,tempPt.y));
                    }
                    //I suppose this could potentially fail if Your somehow end up always in the circle and not on the shape.
                    //Karnaugh Map representation of this gives us the output.
                    //We make sure normalized vectors does not have a vector of {0,0} otherwise we skip over it.
                    else if((tools.isOn(tempPt,newShape) && tools.isInside(tempPt,shapes)) || (tools.isInside(tempPt,shapes) && tools.isInside(tempPt,myCircle.shape)))
                    {

                        double x = tempPt.x + normalizedVectors[i][0];
                        double y = tempPt.y + normalizedVectors[i][1];

                        tempPt = Point(x,y);
                        if(normalizedVectors[i][0] == 0 && normalizedVectors[i][1] == 0)
                            exitLoop = true;
                    }
                    else
                    {
                        que.push(Point(tempPt.x,tempPt.y));
                        exitLoop = true;
                        //exit loop without adding anything to Queue
                    }
                }  
            }
        }

        for(unsigned i = 0; i < que.size(); i++)
        {
            circle(circlestoadd, que.front(), 10, Scalar(255,255,255), -1, 8);
        }
        circlestoadd = newShape + circlestoadd;

        imshow("circles to add", circlestoadd);
        imshow("shapes",newShape);
        waitKey(0);
    }
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
        Circle myCircle = que.front();
        que.pop_front();
        vector<Point> intersections = tools.findIntersections(myCircle.shape, newShape);

        ////////////////////////////////////////////////////////////////////////////////////////////////
        //cout << "intersection.size(): " << intersections.size() << endl;
        //Mat my_circle = Mat::zeros(newShape.size(), CV_8U);
        //my_circle = myCircle.shape | newShape;
        //imshow("my circle", my_circle);
        //waitKey(0);
        ////////////////////////////////////////////////////////////////////////////////////////////////

        if(intersections.size() > 1)
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

            //Find the midpoint of the arcs.
            vector<Point> seeds;
            for(int i = 0; i < (int)radii.size(); i++)
            {
                int j = (i + 1) % (int)radii.size();
                double theta = 0; 
                //if radii[j] < radii[i]
                if(j < i)
                    theta = radii[i] + (radii[j] + 360 - radii[i]) / 2;
                else
                    theta = radii[i] + (radii[j] - radii[i]) / 2;
                //Point to start circle growth from
                theta = theta /180 * 3.141592653;
                double x = myCircle.center.x + myCircle.radius * cos(theta);
                double y = myCircle.center.y + myCircle.radius * sin(theta);
                Point pt = Point(x,y);

                seeds.push_back(pt);
            }

        ////////////////////////////////////////////////////////////////////////////////////////////////
            //for(unsigned i = 0; i < seeds.size(); i++)
            //   circle(my_circle, seeds[i], 5, Scalar(255,255,255), -1, 8); 
            //imshow("my circle", my_circle);
            //waitKey(0);
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
                    if(c.radius != -1 && c.radius > 5 && c.radius < newShape.cols)
                    {
                        if(!tools.isOn(c.center, newShape) && !tools.isOn(c.center, myCircle.shape))
                            que.push_back(c);
                    } 
            }//end while

            cout << "que.size(): " << que.size() << endl;
            //update the newShape so we don't draw into the prev circles (Not sure if we might still draw into prev circles);
            //turns out we do...
            //NOTE: since we divide the growth of the circle by arcs we assume we won't pass into other circle's territories.
            newShape = newShape | myCircle.shape;

            /////////////////////////////////////////////////////////////////////////////////////////    
            namedWindow("new shape", CV_WINDOW_FREERATIO);
            imshow("new shape", newShape);

            Mat circlesInQue = Mat::zeros(newShape.size(), CV_8U);
            circlesInQue = newShape | circlesInQue;
            for(unsigned i = 0; i < que.size(); i++)
                circlesInQue = circlesInQue | que[i].shape;
            namedWindow("circles in que", CV_WINDOW_FREERATIO);
            imshow("circles in que", circlesInQue);
            waitKey(20);
            /////////////////////////////////////////////////////////////////////////////////////////

            cout << "circles.size(): " << circles.size() << endl;
        }//end if
    }//end while
}//end function drawCircle2


void MyFeatureDetector::drawSkeleton(int fudge)
{
    for(unsigned i = 0; i < circles.size(); i++)
    {
        for(unsigned j = 0; j < circles.size(); j++)
        {
            Mat c1 = Mat::zeros(shapes.size(), CV_8U);
            Mat c2 = Mat::zeros(shapes.size(), CV_8U);
            circle(c1, circles[i].center, circles[i].radius, Scalar(255,255,255), fudge, 8);
            circle(c2, circles[j].center, circles[j].radius, Scalar(255,255,255), fudge, 8);
            if(tools.doesIntersect(c1, c2) && i != j)
                line(shapes, circles[i].center, circles[j].center, Scalar(255,255,255), 2, 8);
        }
        Mat circle_img = Mat::zeros(shapes.size(), CV_8U);
        circle(circle_img, circles[i].center, circles[i].radius, Scalar(255,255,255), 2, 8);
        shapes = shapes | circle_img;
    }
}

void MyFeatureDetector::showShape()
{
    namedWindow("final shape", CV_WINDOW_FREERATIO);
    imshow("final shape", shapes);
    waitKey(0);
}
