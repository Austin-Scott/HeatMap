#include <iostream>
#include <string>
#include <filesystem>

#include "TrainingCenterXML.h"
#include "GPSExchangeFormat.h"
#include "FITProtocol.h"
#include "HeatMap.h"

using namespace std;
using namespace experimental::filesystem;


int main(int argc, char* argv[]) {

	string activityDirectory = "test";


	bool useAntiAliasing = true;

	bool useActivityFiltering = false;
	vector<ActivityType> activityFilters = { ActivityType::Running }; //Activities types that should be used, everything else excluded

	bool useDateFiltering = false;
	bool includeUnknownDates = false;
	Date startDate(2018, Month::January, 1, 0, 0, 0); //Activities after startDate and before endDate should be used, everything else excluded
	Date endDate(2018, Month::December, 31, 23, 59, 59);

	bool useAverageSpeedFiltering = false;
	bool includeUnknownSpeeds = false;
	Speed slowestSpeed(8.5, SpeedUnits::MinutesPerMile); //Activities with average speeds faster than slowestSpeed and slower than fastestSpeed should be used, everything else excluded
	Speed fastestSpeed(5, SpeedUnits::MinutesPerMile);


	string backgroundHexColor = "#000000FF";
	string minimumActivityHexColor = "#FF000080";
	string maximumActivityHexColor = "#FFFFFFFF";

	GeographicCoordinate bottomCenter(44.846595, -91.897108);
	double maxLatitude = 44.938059;

	int width = 1920;
	int height = 1080;

	string renderedImageFilename = "result.png";


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
			TrainingCenterXML tcx(filename);
			map.addActivity(tcx);
		} else if (filename.substr(filename.length() - 4) == ".gpx") {
			cout << filename << endl;
			GPSExchangeFormat gpx(filename);
			map.addActivity(gpx);
		} else if (filename.substr(filename.length() - 4) == ".fit") {
			cout << filename << endl;
			FITProtocol fit(filename);
			map.addActivity(fit);
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