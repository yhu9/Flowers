#include "MyFeatureDetector.h"
#include <string>
#include <sstream>
using namespace std;

int main()
{
	vector<MyFeatureDetector> shape1Detectors;
	vector<MyFeatureDetector> shape2Detectors;
	vector<MyFeatureDetector> shape3Detectors;
	vector<MyFeatureDetector> shape4Detectors;
	vector<MyFeatureDetector> shape5Detectors;
	vector<vector<MyFeatureDetector> > shapeDetectors;
	shapeDetectors.push_back(shape1Detectors);
	shapeDetectors.push_back(shape2Detectors);
	shapeDetectors.push_back(shape3Detectors);
	shapeDetectors.push_back(shape4Detectors);
	shapeDetectors.push_back(shape5Detectors);

//Currently there are only 18 min 23 max images in any given flower shape. Alter the max of i and j if you want to go through only some of these shapes.
//Features can be taken out and switched in any order.
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 10; j++){
			for(int x = 0; x < (int)shapeDetectors.size(); x++){
				shapeDetectors[x].push_back(MyFeatureDetector());
				stringstream fname;
				fname << "Flowers/Shape"; fname << x + 1; fname << "/shape"; fname << x + 1; fname << "-0";
				fname << i; fname << j; fname << ".ppm";
				string tmp = fname.str();
				const char* filename = tmp.c_str();
				//cout << filename << endl;
				if(shapeDetectors[x].back().init((char*)filename) != 0)
				{
					shapeDetectors[x].back().drawCircle2();
					if(shapeDetectors[x].back().getCircles().size() > 10){
						shapeDetectors[x].back().drawSkeleton(2);
						shapeDetectors[x].back().showShape(1);
						double tmpa = shapeDetectors[x].back().extractAreaOfCircles();
						//double tmpb = shapeDetectors[x].back().extractAreaOfBoundRect();
						double tmpc = shapeDetectors[x].back().extractAreaOfBoundHull();
						//double tmpd = shapeDetectors[x].back().extractHullLength();
						double tmpe = shapeDetectors[x].back().extractSkeletonLength();
						double tmpf = shapeDetectors[x].back().extractAverageDegree();
						//double tmpg = shapeDetectors[x].back().extractAverageAngle();
						//double tmph = shapeDetectors[x].back().extractAreaOfFirstCircle();
						//double tmpi = shapeDetectors[x].back().extractDegreeOfFirstCircle();
						//double tmpj = shapeDetectors[x].back().extractNumberOfSkeletonNodes();
						//double tmpk = shapeDetectors[x].back().extractNumberOfHullNodes();

						shapeDetectors[x].back().addFeature(tmpa);
						//shapeDetectors[x].back().addFeature(tmpb);
						shapeDetectors[x].back().addFeature(tmpc);
						//shapeDetectors[x].back().addFeature(tmpd);
						shapeDetectors[x].back().addFeature(tmpe);
						shapeDetectors[x].back().addFeature(tmpf);
						//shapeDetectors[x].back().addFeature(tmpg);
						//shapeDetectors[x].back().addFeature(tmph);
						//shapeDetectors[x].back().addFeature(tmpi);
						//shapeDetectors[x].back().addFeature(tmpj);
						//shapeDetectors[x].back().addFeature(tmpk);
					}
				}
				else{
					shapeDetectors[x].pop_back();	
					cout << "file could not be initialized" << endl;
				}
			}
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//make features relative to each other.

	vector<double> maximums;
	for(unsigned i = 0; i < shapeDetectors.front().front().features.size(); i++)
		maximums.push_back(0);
	
	for(unsigned a = 0; a < shapeDetectors.size(); a++)
	{
		for(unsigned b = 0; b < shapeDetectors[a].size(); b++)
		{
			if(shapeDetectors[a][b].features.size() < maximums.size())
				cout << "shape " << a + 1 << " image " << b << " was invalid" << endl;
			else{
				for(unsigned c = 0; c < maximums.size(); c++){
					if(maximums[c] < shapeDetectors[a][b].features[c])
						maximums[c] = shapeDetectors[a][b].features[c];
				}
			}
		}
	}

	for(unsigned a = 0; a < shapeDetectors.size(); a++){
		for(unsigned b = 0; b < shapeDetectors[a].size(); b++){
			cout << "+1";
			for(unsigned c = 0; c < shapeDetectors[a][b].features.size(); c++){
				double normFeature;
				if(maximums[c] != 0)
					normFeature = shapeDetectors[a][b].features[c] / maximums[c];
				else
					cout << "divide by zero error" << endl;
					
				//cout << " " << c + 1 << ":" << normFeature;
				cout << " " << c + 1 << ":" << shapeDetectors[a][b].features[c];
			}
			if(b < 10)	
				cout << " # " << "shape" << a + 1 << "-00" << b << endl;
			else
				cout << " # " << "shape" << a + 1 << "-0" << b << endl;
		}
	}

	//cout << "check 3" << endl;	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//exit

	for(unsigned i = shapeDetectors.size(); i > 0; i--)
	{
		for(unsigned j = shapeDetectors[i - 1].size(); j > 0; j--){
			shapeDetectors[i - 1][j - 1].exit();
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 1;
}

