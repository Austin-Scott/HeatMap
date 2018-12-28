#pragma once
#include <vector>

#include "GeographicCoordinate.h"

using namespace std;

class Activity {
public:
	virtual const vector<GeographicCoordinate>& getTrack() = 0;
};