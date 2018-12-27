#include <iostream>

#include "TrainingCenterXML.h"
#include "HeatMap.h"

using namespace std;

int main(int argc, char* argv[]) {
	/*
	1. Create an nxm array of doubles corresponding to an nxm resulting image. Initialize all with zero.
	2. For each file in a directory, plot the GPS data using Xiaolin Wu's line algorithm onto the 2d array of doubles.
		2.1. Use RapidXML to parse the data TCX data files.
	3. Normalize each cell of the array to a value from [0,1] based on the min and max values.
	4. Render the data into an image and then use LodePNG to encode a png file that can be saved.
	*/
	HeatMap map(800, 600, geoCoord(44.851533, -91.965198), geoCoord(44.913078, -91.864642));
	TrainingCenterXML tcx("test.tcx");
	TrainingCenterXML tcx2("test2.tcx");
	map.addActivity(tcx);
	map.addActivity(tcx2);
	map.debugDrawCells();
	system("pause");
}