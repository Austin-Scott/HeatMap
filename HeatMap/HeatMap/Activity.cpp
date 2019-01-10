#include "Activity.h"
#include "HeatMapConfiguration.h"
#include "HeatMap.h"

string getOnlyFilename(string path)
{
	if (path.size() == 0)
		return "";

	size_t backslash = path.find_last_of('\\');
	size_t forwardslash = path.find_last_of('/');
	size_t accepted;
	if (backslash != string::npos && forwardslash != string::npos) {
		accepted = backslash > forwardslash ? backslash : forwardslash;
	}
	else if (backslash == string::npos && forwardslash == string::npos) {
		return path;
	}
	else {
		if (backslash == string::npos) {
			accepted = forwardslash;
		}
		else {
			accepted = backslash;
		}
	}
	string result = path.substr(accepted);
	if (result.front() == '\\' || result.front() == '/') {
		result.erase(result.begin(), result.begin() + 1);
	}
	return result;
}

bool includeActivity(Activity & activity, HeatMapConfiguration configuration)
{
	Date date = activity.getStartDate();
	if (!configuration.includeUnknownDates && !date.isDateSet()) {
		return false;
	}
	if (configuration.useDateFilteringOne && configuration.startDate.isDateSet() && date.isDateSet() && date.isOlderThan(configuration.startDate)) {
		return false;
	}
	if (configuration.useDateFilteringTwo && configuration.endDate.isDateSet() && date.isDateSet() && date.isMoreRecentThan(configuration.endDate)) {
		return false;
	}
	Speed speed = activity.getAverageSpeed();
	if (!configuration.includeUnknownSpeeds && !speed.isSpeedSet()) {
		return false;
	}
	if (configuration.useAverageSpeedFilteringOne && configuration.slowestSpeed.isSpeedSet() && speed.isSpeedSet() && speed.getSpeed() < configuration.slowestSpeed.getSpeed()) {
		return false;
	}
	if (configuration.useAverageSpeedFilteringTwo && configuration.fastestSpeed.isSpeedSet() && speed.isSpeedSet() && speed.getSpeed() > configuration.fastestSpeed.getSpeed()) {
		return false;
	}
	for (ActivityType t : configuration.activityFilters) {
		if (t == activity.getActivityType()) {
			return false;
		}
	}
	return true;
}

vector<GeographicCoordinate> computeBoundingBoxVertical(GeographicCoordinate bottomCenter, HeatMapConfiguration config, double maxLatitude)
{
	double latitudeDifference = HeatMap::latToWorldY(maxLatitude) - HeatMap::latToWorldY(bottomCenter.getLat());
	double aspectRatio = (double)config.width / (double)config.height;
	double viewportWidth = abs(latitudeDifference) * aspectRatio;
	return vector<GeographicCoordinate>({ geoCoord(bottomCenter.getLat(), HeatMap::worldXToLon(HeatMap::lonToWorldX(bottomCenter.getLon()) - (viewportWidth / 2.0))), geoCoord(maxLatitude, HeatMap::worldXToLon(HeatMap::lonToWorldX(bottomCenter.getLon()) + (viewportWidth / 2.0))) });
}

vector<GeographicCoordinate> computeBoundingBoxHorizontal(GeographicCoordinate leftCenter, HeatMapConfiguration config, double rightMostLongitude)
{
	double longituteDifference = HeatMap::lonToWorldX(rightMostLongitude) - HeatMap::lonToWorldX(leftCenter.getLon());
	double aspectRatio = (double)config.height / (double)config.width;
	double viewportHeight = abs(longituteDifference) * aspectRatio;
	return vector<GeographicCoordinate>({ geoCoord(HeatMap::worldYToLat(HeatMap::latToWorldY(leftCenter.getLat()) - (viewportHeight / 2.0)), leftCenter.getLon()), geoCoord(HeatMap::worldYToLat(HeatMap::latToWorldY(leftCenter.getLat())+(viewportHeight/2.0)), rightMostLongitude) });
}

vector<GeographicCoordinate> guessBounds(vector<Activity*> activities, HeatMapConfiguration config, double radius)
{
	vector<Activity*> filteredActivities;
	vector<double> startLats;
	vector<double> startLons;
	for (Activity* a : activities) {
		if (includeActivity(*a, config)) {
			if (a->getTrack().size() > 0) {
				startLats.push_back(a->getTrack()[0].getLat());
				startLons.push_back(a->getTrack()[0].getLon());
				filteredActivities.push_back(a);
			}
		}
	}
	if (filteredActivities.size() == 0)
		return vector<GeographicCoordinate>();

	sort(startLats.begin(), startLats.end());
	sort(startLons.begin(), startLons.end());

	GeographicCoordinate medianStartPoint = geoCoord(startLats[startLats.size() / 2], startLons[startLons.size() / 2]);

	double maxLatitude;
	double minLatitude;
	double maxLongitude;
	double minLongitude;
	bool initialized = false;

	for (Activity* a : filteredActivities) {
		if (medianStartPoint.getKilometersTo(a->getTrack()[0]) < radius) {
			if(!initialized || (a->upperRight().getLat()>maxLatitude))
				maxLatitude=a->upperRight().getLat();
			if (!initialized || (a->lowerLeft().getLat()<minLatitude))
				minLatitude=a->lowerLeft().getLat();
			if (!initialized || (a->upperRight().getLon()>maxLongitude))
				maxLongitude=a->upperRight().getLon();
			if (!initialized || (a->lowerLeft().getLon()<minLongitude))
				minLongitude=a->lowerLeft().getLon();

			initialized = true;
		}
	}

	const double padding = 0.005;
	maxLatitude += padding;
	minLatitude -= padding;
	maxLongitude += padding;
	minLongitude -= padding;

	double dLat = maxLatitude - minLatitude;
	double dLon = maxLongitude - minLongitude;

	if(dLat==0.0)
		return vector<GeographicCoordinate>();

	double aspectRatio = (double)config.width / (double)config.height;

	if ((dLon / dLat) > aspectRatio) {
		return computeBoundingBoxHorizontal(geoCoord(minLatitude + (abs(dLat) / 2.0), minLongitude), config, maxLongitude);
	}
	else {
		return computeBoundingBoxVertical(geoCoord(minLatitude, minLongitude + (abs(dLon) / 2.0)), config, maxLatitude);
	}
}
