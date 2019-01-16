#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>
#include <locale>
#include <Windows.h>

#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/widgets/date_chooser.hpp>

#include "Activity.h"
#include "TrainingCenterXML.h"
#include "GPSExchangeFormat.h"
#include "FITProtocol.h"
#include "HeatMap.h"
#include "HeatMapConfiguration.h"
#include "MainGUI.h"

#include "ActivityDirectoryGUI.h"

#include "MapQuest.h"

using namespace std;
using namespace nana;
using namespace experimental::filesystem;

void executeSystemCommand(string cmd) {
	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	if (CreateProcess(NULL, const_cast<char *>(cmd.c_str()), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &info, &processInfo))
	{
		WaitForSingleObject(processInfo.hProcess, INFINITE);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
}

void writeSharedString(mutex &m, string* strDest, string strSrc) {
	m.lock();
	*strDest = strSrc;
	m.unlock();
}

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

atomic<bool> shouldCancelHM;
atomic<unsigned int> progressAmountHM;
atomic<bool> progressKnownHM;
mutex statusMutexHM;
string statusStringHM;

Image* generateHeatMapImage(HeatMapConfiguration configuration, vector<Activity*> activities)  {
	progressKnownHM = false;
	Image* background = nullptr;
	string mapKey = "";
	fstream keyFile("key.txt");
	if (configuration.downloadMap && keyFile) {
		getline(keyFile, mapKey);

		writeSharedString(statusMutexHM, &statusStringHM, "Downloading map...");

		MapQuestConfig mapConfig = getMapConfig(configuration.lowerLeft, configuration.upperRight, configuration.width, configuration.height);

		configuration.lowerLeft = mapConfig.lowerLeft;
		configuration.upperRight = mapConfig.upperRight;

		wstring tempPathW;
		wchar_t wcharPath[MAX_PATH];
		if (GetTempPathW(MAX_PATH, wcharPath)) {
			tempPathW = wcharPath;
			tempPathW += L"\\tempMapDownload.png";
		}
		else {
			tempPathW = L"tempMapDownload.png";
		}
		wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
		string tempPath = converter.to_bytes(tempPathW);


		string downloadCommand = "curl -k -o \""+tempPath+"\" \"https://open.mapquestapi.com/staticmap/v5/map?key=" + mapKey + "&size=" + to_string(configuration.width) + "," + to_string(configuration.height) + "&center=" + to_string(mapConfig.center.getLat()) + "," + to_string(mapConfig.center.getLon()) + "&zoom=" + to_string(mapConfig.zoom) + "&format=png&type="+configuration.mapType+"&margin=0\"";

		executeSystemCommand(downloadCommand);

		background = new Image(tempPath);

		DeleteFile(const_cast<char *>(tempPath.c_str())); //Clean up temp file

		keyFile.close();
	}
	else {
		background = new Image(configuration.width, configuration.height, configuration.backgroundColor);
	}
	progressKnownHM = true;
	
	cout << "Lat/Lon Bounding Box for Heat Map:" << endl
		<< "\t*Lower left bound: \"" << configuration.lowerLeft.toString()
		<< "\"\n\t*Upper right bound: \"" << configuration.upperRight.toString() << "\"" << endl;

	HeatMap map(configuration);

	writeSharedString(statusMutexHM, &statusStringHM, "Rendering activities on Heat Map...");
	progressKnownHM = true;
	for (int i = 0; i < activities.size(); i++) {
		if (shouldCancelHM) return nullptr;

		map.addActivity(*activities[i]);
		progressAmountHM = ((double)i / (double)activities.size())*(int)100;
	}

	writeSharedString(statusMutexHM, &statusStringHM, "Normalizing map...");
	map.normalizeMap();

	writeSharedString(statusMutexHM, &statusStringHM, "Rendering image...");
	Image* result = map.renderImage();

	Image* resultWithBackground = result->overlayImage(background, configuration.heatLayerTransparency);
	delete result;

	return resultWithBackground;
}

void onActivitiesLoaded(vector<Activity*> activities, MainGUI* mainGUI) {
	cout << "Loading finished." << endl;
	cout << activities.size() << " activities loaded." << endl;
	mainGUI->present(generateHeatMapImage, &progressAmountHM, &shouldCancelHM, &progressKnownHM, &statusMutexHM, &statusStringHM, activities);
}

atomic<bool> shouldCancel;
atomic<unsigned int> progressAmount;
atomic<bool> progressKnown;
string statusString;
mutex statusMutex;

vector<Activity*> loadActivities(string activityDirectory, bool shouldDecompress) {
	progressAmount = 0;

	if (shouldDecompress) {
		progressKnown = false;
		writeSharedString(statusMutex, &statusString, "Decompressing files...");
		cout << "\nDecompressing any compressed activity files..." << endl << endl;
		//7z x "{Directory name ending in /}*.gz" -aos "-o{directory name ending in /}"
		string activityDirectoryWithSlash = activityDirectory;
		if (activityDirectory.size()==0 || activityDirectoryWithSlash.back() != '/' || activityDirectoryWithSlash.back() != '\\')
			activityDirectoryWithSlash.push_back('/');
		string command = "7z x \"" + activityDirectoryWithSlash + "*.gz\" -aos \"-o" + activityDirectoryWithSlash + "\"";
		executeSystemCommand(command);
	}

	if (shouldCancel) {
		return vector<Activity*>();
	}

	writeSharedString(statusMutex, &statusString, "Counting activity files...");
	int totalNumberOfActivities = 0;
	for (auto &p : directory_iterator(activityDirectory)) {
		string filename = p.path().string();
		if (filename.substr(filename.length() - 4) == ".tcx" || filename.substr(filename.length() - 4) == ".gpx" || filename.substr(filename.length() - 4) == ".fit") {
			totalNumberOfActivities++;
		}
	}

	if (shouldCancel) {
		return vector<Activity*>();
	}

	vector<Activity*> result;

	progressKnown = true;
	progressAmount = 0;
	int filesLoaded = 0;
	cout << "\nBegin loading activity files from " << activityDirectory << "..." << endl << endl;
	for (auto &p : directory_iterator(activityDirectory)) {
		string filename = p.path().string();
		if (filename.substr(filename.length() - 4) == ".tcx") {
			writeSharedString(statusMutex, &statusString, "Loading " + getOnlyFilename(filename) + "...");
			result.push_back(new TrainingCenterXML(filename));
			filesLoaded++;
		}
		else if (filename.substr(filename.length() - 4) == ".gpx") {
			writeSharedString(statusMutex, &statusString, "Loading " + getOnlyFilename(filename) + "...");
			result.push_back(new GPSExchangeFormat(filename));
			filesLoaded++;
		}
		else if (filename.substr(filename.length() - 4) == ".fit") {
			writeSharedString(statusMutex, &statusString, "Loading " + getOnlyFilename(filename) + "...");
			result.push_back(new FITProtocol(filename));
			filesLoaded++;
		}
		if (shouldCancel) {
			for (auto p : result) {
				delete p;
			}
			return vector<Activity*>();
		}
		if (totalNumberOfActivities != 0) {
			progressAmount = ((double)filesLoaded / (double)totalNumberOfActivities) * (int)100;
		}
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	ActivityDirectoryGUI activityDirectoryGUI;
	MainGUI mainGUI;

	activityDirectoryGUI.present(loadActivities, onActivitiesLoaded, &progressAmount, &shouldCancel, &progressKnown, &statusString, &statusMutex, &mainGUI);
	

	exec();
}