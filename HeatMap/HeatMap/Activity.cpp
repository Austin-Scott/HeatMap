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