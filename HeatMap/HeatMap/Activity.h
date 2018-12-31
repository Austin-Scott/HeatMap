#pragma once
#include <vector>
#include <string>

#include "GeographicCoordinate.h"
#include "Date.h"
#include "Speed.h"

using namespace std;

enum ActivityType { Unknown, Running, Cycling, Walking, Swimming };

class Activity {
public:
	virtual const vector<GeographicCoordinate>& getTrack() = 0;
	virtual ActivityType getActivityType() = 0;
	virtual Date getStartDate() = 0;
	virtual Speed getAverageSpeed() = 0;
	virtual string getFilename() = 0;
};