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
	vector<GeographicCoordinate> track;
public:
	TrainingCenterXML(string filename, vector<string> activityFilters);
	const vector<GeographicCoordinate>& getTrack();
};