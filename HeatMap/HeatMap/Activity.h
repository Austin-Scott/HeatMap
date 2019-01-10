#pragma once
#include <vector>
#include <string>

#include "GeographicCoordinate.h"
#include "Date.h"
#include "Speed.h"

struct HeatMapConfiguration;
class HeatMap;

using namespace std;

enum ActivityType { Unknown, Running, Cycling, Walking, Swimming };

class Activity {
public:
	virtual const vector<GeographicCoordinate>& getTrack() = 0;
	virtual GeographicCoordinate upperRight() = 0;
	virtual GeographicCoordinate lowerLeft() = 0;
	virtual ActivityType getActivityType() = 0;
	virtual Date getStartDate() = 0;
	virtual Speed getAverageSpeed() = 0;
	virtual string getFilename() = 0;
};

string getOnlyFilename(string path);
bool includeActivity(Activity &activity, HeatMapConfiguration configuration);
//{lowerLeft, upperRight}
vector<GeographicCoordinate> computeBoundingBoxVertical(GeographicCoordinate bottomCenter, HeatMapConfiguration config, double maxLatitude);
vector<GeographicCoordinate> computeBoundingBoxHorizontal(GeographicCoordinate leftCenter, HeatMapConfiguration config, double rightMostLongitude);
vector<GeographicCoordinate> guessBounds(vector<Activity*> activities, HeatMapConfiguration config, double radius);