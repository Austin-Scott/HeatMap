#include <iostream>
#include <string>
#include <filesystem>

#include "TrainingCenterXML.h"
#include "GPSExchangeFormat.h"
#include "FITProtocol.h"
#include "HeatMap.h"
#include "HeatMapConfiguration.h"

using namespace std;
using namespace experimental::filesystem;

void printActivityInfo(Activity &activity, bool includedOnMap) {
	cout << activity.getFilename() << endl;
	Date startDate = activity.getStartDate();
	if (startDate.isDateSet()) {
		cout << "\t*Activity recorded on: " << startDate.toString() << endl;
	}
	ActivityType activityType = activity.getActivityType();
	switch (activityType) {
	case ActivityType::Running:
		cout << "\t*Activity type: Running" << endl;
		break;
	case ActivityType::Cycling:
		cout << "\t*Activity type: Cycling" << endl;
		break;
	case ActivityType::Walking:
		cout << "\t*Activity type: Walking" << endl;
		break;
	case ActivityType::Swimming:
		cout << "\t*Activity type: Swimming" << endl;
		break;
	default:
		cout << "\t*Activity type: Unknown" << endl;
	}
	Speed averageSpeed = activity.getAverageSpeed();
	if (averageSpeed.isSpeedSet()) {
		cout << "\t*Average speed: " << averageSpeed.getSpeed(SpeedUnits::MetersPerSecond) << " m/s" << endl;
	}
	cout << "\t*GPS track: " << activity.getTrack().size() << " points" << endl;

	if (includedOnMap) {
		cout << "\t+Included on Heat Map" << endl;
	}
	else {
		cout << "\t-Excluded from Heat Map" << endl;
	}
}

Image* generateHeatMapImage(string activityDirectory, bool decompressFiles, HeatMapConfiguration configuration)  {
	cout << "Lat/Lon Bounding Box for Heat Map:" << endl
		<< "\t*Lower left bound: \"" << configuration.lowerLeft.toString()
		<< "\"\n\t*Upper right bound: \"" << configuration.upperRight.toString() << "\"" << endl;

	HeatMap map(configuration);
	
	if (decompressFiles) {
		cout << "\nDecompressing any compressed activity files..." << endl << endl;
		//7z x "{Directory name ending in /}*.gz" -aos "-o{directory name ending in /}"
		string activityDirectoryWithSlash = activityDirectory;
		if (activityDirectoryWithSlash.back() != '/' || activityDirectoryWithSlash.back() != '\\')
			activityDirectoryWithSlash.push_back('/');
		string command = "7z x \"" + activityDirectoryWithSlash + "*.gz\" -aos \"-o" + activityDirectoryWithSlash + "\"";
		system(command.c_str());
	}

	cout << "\nBegin adding activity files from " << activityDirectory << "..." << endl << endl;
	for (auto &p : directory_iterator(activityDirectory)) {
		string filename = p.path().string();
		if (filename.substr(filename.length() - 4) == ".tcx") {
			TrainingCenterXML tcx(filename);
			printActivityInfo(tcx, map.checkFilter(tcx));
			map.addActivity(tcx);
		}
		else if (filename.substr(filename.length() - 4) == ".gpx") {
			GPSExchangeFormat gpx(filename);
			printActivityInfo(gpx, map.checkFilter(gpx));
			map.addActivity(gpx);
		}
		else if (filename.substr(filename.length() - 4) == ".fit") {
			FITProtocol fit(filename);
			printActivityInfo(fit, map.checkFilter(fit));
			map.addActivity(fit);
		}
	}

	cout << "\nNormalizing Heat Map..." << endl << endl;
	map.normalizeMap();

	cout << "Rendering Image..." << endl << endl;
	Image* result = map.renderImage();

	cout << "...done!" << endl;

	return result;
}

int main(int argc, char* argv[]) {

	string activityDirectory = "test";
	string renderedImageFilename = "result.png";

	HeatMapConfiguration config(1920, 1080);
	config.computeBoundingBox(geoCoord(44.846595, -91.897108), 44.938059);
	config.setRenderer(true, Color("#000000FF"), Color("#FF000080"), Color("#FFFFFFFF"));

	Image* heatMapImage = generateHeatMapImage(activityDirectory, true, config);

	heatMapImage->saveImage(renderedImageFilename);
	delete heatMapImage;

	system("pause");
}