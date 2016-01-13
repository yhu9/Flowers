#include "Circle.h"


Circle::Circle(void)
{
}

Circle::Circle(cv::Point centerIn, double radiusIn)
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
		int x = center.x + radius * std::cos(rad);
		int y = center.y + radius * std::sin(rad);
		cv::Point newpt = cv::Point(x,y);
		cPoints.push_back(newpt);
	}
}
/*
cv::Point Circle::findPoint(cv::Vec2i ray)
{
	cv::Point nextPt;
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
				std::cout << "unhandled error" << std::endl;
		}
	}

	return nextPt;
}
*/