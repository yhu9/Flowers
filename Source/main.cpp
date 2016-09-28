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
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			for(int x = 0; x < (int)shapeDetectors.size(); x++){
				shapeDetectors[x].push_back(MyFeatureDetector());
				stringstream fname;
				fname << "Flowers/Shape"; fname << x + 1; fname << "/shape"; fname << x + 1; fname << "-0";
				fname << i; fname << j; fname << ".ppm";
				string tmp = fname.str();
				const char* filename = tmp.c_str();
				if(shapeDetectors[x].back().init((char*)filename, 0) != 0)
				{
					shapeDetectors[x].back().drawCircle2();
					if(shapeDetectors[x].back().getCircles().size() > 10){
						shapeDetectors[x].back().drawSkeleton(2);
						shapeDetectors[x].back().showShape(1);
						double tmpa = shapeDetectors[x].back().extractAreaOfCircles();
						double tmpb = shapeDetectors[x].back().extractAreaOfBoundRect();
						double tmpc = shapeDetectors[x].back().extractAreaOfBoundHull();
						double tmpd = shapeDetectors[x].back().extractHullLength();
						double tmpe = shapeDetectors[x].back().extractSkeletonLength();
						double tmpf = shapeDetectors[x].back().extractAverageDegreeOfSkeleton();
						double tmpg = shapeDetectors[x].back().extractAverageAngleOfSkeleton();
						double tmph = shapeDetectors[x].back().extractAreaOfFirstCircle();
						double tmpi = shapeDetectors[x].back().extractDegreeOfFirstCircle();
						double tmpj = shapeDetectors[x].back().extractNumberOfSkeletonNodes();
						double tmpk = shapeDetectors[x].back().extractNumberOfHullNodes();
						double tmpl = shapeDetectors[x].back().extractNumberOfMSTNodes();
						double tmpm = shapeDetectors[x].back().extractMSTLength();
						double tmpn = shapeDetectors[x].back().extractAverageAreaOfCircles();
						double tmpo = shapeDetectors[x].back().extractAverageAngleOfMST();
						double tmpp = shapeDetectors[x].back().extractAverageAngleOfHull();
						double tmpq = shapeDetectors[x].back().extractRatioSkeletonDegreeLarge();
						double tmpr = shapeDetectors[x].back().extractRatioMSTDegreeLarge();
						double tmps = shapeDetectors[x].back().extractRatioSkeletonAreaLarge();
						double tmpt = shapeDetectors[x].back().extractRatioCircleAreaHullArea();
						double tmpu = shapeDetectors[x].back().extractRatioHullLengthSkeletonLength();
						double tmpv = shapeDetectors[x].back().extractRatioMSTLengthSkeletonLength();
						double tmpw = shapeDetectors[x].back().extractRatioAreaOfFirstCircleAndAverageAreaOfSkeleton();
						double tmpx = shapeDetectors[x].back().extractRatioDegreeOfFirstCircleAndAverageDegreeOfSkeleton();

						shapeDetectors[x].back().addFeature(tmpa);
						shapeDetectors[x].back().addFeature(tmpb);
						shapeDetectors[x].back().addFeature(tmpc);
						shapeDetectors[x].back().addFeature(tmpd);
						shapeDetectors[x].back().addFeature(tmpe);
						shapeDetectors[x].back().addFeature(tmpf);
						shapeDetectors[x].back().addFeature(tmpg);
						shapeDetectors[x].back().addFeature(tmph);
						shapeDetectors[x].back().addFeature(tmpi);
						shapeDetectors[x].back().addFeature(tmpj);
						shapeDetectors[x].back().addFeature(tmpk);
						shapeDetectors[x].back().addFeature(tmpl);
						shapeDetectors[x].back().addFeature(tmpm);
						shapeDetectors[x].back().addFeature(tmpn);
						shapeDetectors[x].back().addFeature(tmpo);
						shapeDetectors[x].back().addFeature(tmpp);
						shapeDetectors[x].back().addFeature(tmpq);
						shapeDetectors[x].back().addFeature(tmpr);
						shapeDetectors[x].back().addFeature(tmps);
						shapeDetectors[x].back().addFeature(tmpt);
						shapeDetectors[x].back().addFeature(tmpu);
						shapeDetectors[x].back().addFeature(tmpv);
						shapeDetectors[x].back().addFeature(tmpw);
						shapeDetectors[x].back().addFeature(tmpx);

						cout << "+1";
						for(unsigned a = 0; a < shapeDetectors[x].back().features.size(); a++){
							cout << " " << a + 1 << ":" << shapeDetectors[x].back().features[a];
						}

						cout << " # " << filename << endl;
						shapeDetectors[x].back().exit();
						shapeDetectors[x].pop_back();
					}
				}
				else{
					shapeDetectors[x].pop_back();	
					cout << "file could not be initialized" << endl;
				}
			}
		}
	}

	/*
	for(unsigned a = 0; a < shapeDetectors.size(); a++){
		for(unsigned b = 0; b < shapeDetectors[a].size(); b++){
			cout << "+1";
			for(unsigned c = 0; c < shapeDetectors[a][b].features.size(); c++){

				cout << " " << c + 1 << ":" << shapeDetectors[a][b].features[c];
			}
			if(b < 10)	
				cout << " # " << "shape" << a + 1 << "-00" << b << endl;
			else
				cout << " # " << "shape" << a + 1 << "-0" << b << endl;
		}
	}
	*/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//exit

	//for(unsigned i = shapeDetectors.size(); i > 0; i--)
	//{
	//	for(unsigned j = shapeDetectors[i - 1].size(); j > 0; j--){
	//		shapeDetectors[i - 1][j - 1].exit();
	//	}
	//}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 1;
}

