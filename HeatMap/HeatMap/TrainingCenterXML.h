#pragma once
#include <vector>
#include <fstream>
#include <string.h>
#include <string>
#include <iostream>

#include "GeographicCoordinate.h"

#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

class TrainingCenterXML {
private:
	vector<GeographicCoordinate> track;
public:
	TrainingCenterXML(string filename);
	const vector<GeographicCoordinate>& getTrack();
};