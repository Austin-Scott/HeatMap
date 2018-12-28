#include <iostream>
#include <string>
#include <filesystem>

#include "TrainingCenterXML.h"
#include "GPSExchangeFormat.h"
#include "HeatMap.h"

using namespace std;
using namespace experimental::filesystem;


int main(int argc, char* argv[]) {
	/*
	1. Create an nxm array of doubles corresponding to an nxm resulting image. Initialize all with zero.
	2. For each file in a directory, plot the GPS data using Xiaolin Wu's line algorithm onto the 2d array of doubles.
		2.1. Use RapidXML to parse the data TCX data files.
	3. Normalize each cell of the array to a value from [0,1] based on the min and max values.
	4. Render the data into an image and then use LodePNG to encode a png file that can be saved.
	*/
	string activityDirectory = "test";
	vector<string> activityFilters = { "Running" };
	string backgroundHexColor = "#00000000";
	string minimumActivityHexColor = "#FF000080";
	string maximumActivityHexColor = "#FFFFFFFF";
	GeographicCoordinate bottomCenter(44.846595, -91.897108);
	double maxLatitude = 44.938059;
	int width = 1920;
	int height = 1080;
	string renderedImageFilename = "result.png";
	bool useAntiAliasing = true;

	if (argc == 12) {
		//TODO process cmd-line arguments
	}
	else if (argc == 1) {
		//TODO add interface for entering arguments
	}
	else {
		cerr << "Error: Incorrect number of arguments passed. Please pass either 1 or 12." << endl
			<< "Syntax: \"" << argv[0] << " activityDirectory activityFilter backgroundHexColor minimumActivityHexColor maximumActivityHexColor bottomCenterLatitude bottomCenterLongitude maxLatitude width height renderedImageFilename" << endl;
		return 1;
	}

	GeographicCoordinate* boundingBox = computeBoundingBox(bottomCenter, maxLatitude, width, height);
	cout << "Lat/Lon Bounding Box for Heat Map computed:" << endl
		<< "\t*Lower left bound: \"";
	boundingBox[0].printCoordinate();
	cout << "\"\n\t*Upper right bound: \"";
	boundingBox[1].printCoordinate();
	cout << "\"" << endl;

	HeatMap map(width, height, boundingBox[0], boundingBox[1]);
	delete[] boundingBox;

	cout << "\nDecompressing any compressed activity files..." << endl << endl;
	//7z x "{Directory name ending in /}*.gz" -aos "-o{directory name ending in /}"
	string activityDirectoryWithSlash = activityDirectory;
	if (activityDirectoryWithSlash.back() != '/' || activityDirectoryWithSlash.back() != '\\')
		activityDirectoryWithSlash.push_back('/');
	string command = "7z x \"" + activityDirectoryWithSlash + "*.gz\" -aos \"-o" + activityDirectoryWithSlash + "\"";
	system(command.c_str());

	cout << "\nBegin adding activity files from " << activityDirectory << "..." << endl << endl;
	for (auto &p : directory_iterator(activityDirectory)) {
		string filename = p.path().string();
		if (filename.substr(filename.length() - 4) == ".tcx") {
			cout << filename << endl;
			TrainingCenterXML tcx(filename, activityFilters);
			if(tcx.getTrack().size()>0)
				map.addActivity(tcx, useAntiAliasing);
		} else if (filename.substr(filename.length() - 4) == ".gpx") {
			cout << filename << endl;
			GPSExchangeFormat gpx(filename);
			if (gpx.getTrack().size()>0)
				map.addActivity(gpx, useAntiAliasing);
		}
	}

	cout << "\nNormalizing Heat Map..." << endl << endl;
	map.normalizeMap();

	cout << "Rendering Image..." << endl << endl;
	Image* result = map.renderImage(Color(backgroundHexColor), Color(minimumActivityHexColor), Color(maximumActivityHexColor));

	cout << "Saving Image as \"" << renderedImageFilename << "\"..." << endl << endl;
	result->saveImage(renderedImageFilename);

	delete result;

	cout << "...done!" << endl;
	system("pause");
}