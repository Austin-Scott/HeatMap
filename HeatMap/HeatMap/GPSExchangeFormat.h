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
	vector<GeographicCoordinate> track;
public:
	GPSExchangeFormat(string filename);
	const vector<GeographicCoordinate>& getTrack();
};