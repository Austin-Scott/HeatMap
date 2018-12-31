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

class GPSExchangeFormat : public Activity {
private:
	string filename;

	vector<GeographicCoordinate> track;

	Date startDate;
public:
	GPSExchangeFormat(string filename);
	const vector<GeographicCoordinate>& getTrack();
	ActivityType getActivityType();
	Date getStartDate();
	Speed getAverageSpeed();
	string getFilename();
};