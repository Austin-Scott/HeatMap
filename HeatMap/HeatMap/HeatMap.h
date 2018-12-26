#pragma once
#include "HeatMapCell.h"
#include "GeographicCoordinate.h"

class HeatMap {
private:
	int width;
	int height;

	GeographicCoordinate lowerLeft;
	GeographicCoordinate upperRight;

	HeatMapCell** cells;
public:

};