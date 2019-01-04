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
	return true;
}
