#include "PrivacyZones.h"

Zone::Zone(GeographicCoordinate lowerLeft, GeographicCoordinate upperRight)
{
	rectangle = true;
	this->lowerLeft = lowerLeft;
	this->upperRight = upperRight;
}

Zone::Zone(GeographicCoordinate center, double radius)
{
	rectangle = false;
	this->center = center;
	this->radius = radius;
}

bool Zone::isPointInZone(GeographicCoordinate point)
{
	if (rectangle) {
		if (point.getLat() < lowerLeft.getLat() || point.getLat() > upperRight.getLat())
			return false;
		if (point.getLon() < lowerLeft.getLon() || point.getLon() > upperRight.getLon())
			return false;

		return true;
	}
	else {
		return point.getKilometersTo(center) < radius;
	}
}

PrivacyZones::PrivacyZones(string filename)
{
	ifstream file(filename);
	if (file) {

		while (!file.eof()) {
			string line = "";
			getline(file, line);
			if (line.size() > 0 && line[0] != '#') {
				vector<string> components;
				stringstream buffer(line);
				while (!buffer.eof()) {
					string component = "";
					getline(buffer, component, ' ');
					components.push_back(component);
				}
				if (components.size() == 4) {
					zones.emplace_back(geoCoord(stod(components[0].c_str()), stod(components[1].c_str())), geoCoord(stod(components[2].c_str()), stod(components[3].c_str())));
				}
				else if (components.size() == 3) {
					zones.emplace_back(geoCoord(stod(components[0].c_str()), stod(components[1].c_str())), stod(components[2].c_str()));
				}
			}
		}

		file.close();
	}
}

PrivacyZones::PrivacyZones()
{
}

bool PrivacyZones::isPointInZones(GeographicCoordinate point)
{
	for (Zone z : zones) {
		if (z.isPointInZone(point))
			return true;
	}
	return false;
}
