#include "Activity.h"

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
	if (configuration.useDateFilteringOne || configuration.useDateFilteringTwo) {
		Date date = activity.getStartDate();
		if (date.isDateSet()) {
			if (configuration.useDateFilteringOne && configuration.startDate.isDateSet() && date.isOlderThan(configuration.startDate)) {
				return false;
			}
			if (configuration.useDateFilteringTwo && configuration.endDate.isDateSet() && date.isMoreRecentThan(configuration.endDate)) {
				return false;
			}
		}
		else if(!configuration.includeUnknownDates) {
			return false;
		}
	}
	if (configuration.useAverageSpeedFiltering) {
		Speed activitySpeed = activity.getAverageSpeed();
		if (activitySpeed.isSpeedSet()) {
			if (activitySpeed.getSpeed() < configuration.slowestSpeed.getSpeed() || activitySpeed.getSpeed() > configuration.fastestSpeed.getSpeed()) {
				return false;
			}
		}
		else if (!configuration.includeUnknownSpeeds) {
			return false;
		}
	}
	return true;
}
