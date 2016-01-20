#include "Circle.h"


Circle::Circle(void)
{
}

Circle::Circle(Point centerIn, double radiusIn)
{
	center = centerIn;
	radius = radiusIn;

	createPoints();
}


Circle::~Circle(void)
{
}

void Circle::createPoints()
{
	for(int i = 0; i <= 360; i++)
	{
		double rad = (double)i/180 * 3.1415926;
		int x = center.x + radius * cos(rad);
		int y = center.y + radius * sin(rad);
		Point newpt = Point(x,y);
		cPoints.push_back(newpt);
	}
}
/*
Point Circle::findPoint(Vec2i ray)
{
	Point nextPt;
	double slope1 = (double)ray[1]/ray[0];
	for(unsigned i = 0; i < cPoints.size(); i++)
	{
		if(cPoints[i].y - center.y < slope1*(cPoints[i].x - center.x) + 5 && cPoints[i].y - center.y > slope1*(cPoints[i].x - center.x) - 5)
		{
			if(ray[0] > 0 && ray[1] < 0)
			{
				
			}
			else if(ray[0] < 0 && ray[1] < 0)
			{
				
			}
			else if(ray[0] < 0 && ray[1] > 0)
			{
				
			}
			else if(ray[0] > 0 && ray[1] > 0)
			{
				
			}
			else
				cout << "unhandled error" << endl;
		}
	}

	return nextPt;
}
*/
