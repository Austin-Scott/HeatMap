#pragma once
#include <vector>
#include <fstream>
#include <string.h>
#include <string>
#include <iostream>

#include "GeographicCoordinate.h"
#include "Activity.h"


#include "rapidxml.hpp"


using namespace std;
using namespace rapidxml;

class TrainingCenterXML : public Activity {
private:
	string filename;

	vector<GeographicCoordinate> track;

	ActivityType activityType;
	Date startDate;
	Speed averageSpeed;

	double maxLatitude;
	double minLatitude;
	double maxLongitude;
	double minLongitude;

public:
	TrainingCenterXML(string filename);
	const vector<GeographicCoordinate>& getTrack();
	ActivityType getActivityType();
	Date getStartDate();
	Speed getAverageSpeed();
	string getFilename();
	GeographicCoordinate upperRight();
	GeographicCoordinate lowerLeft();
};