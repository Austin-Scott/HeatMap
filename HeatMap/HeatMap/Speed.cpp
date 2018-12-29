#include "Speed.h"

Speed::Speed(double speed, SpeedUnits units)
{
	setSpeed(speed, units);
}

Speed::Speed()
{
	isSet = false;
}

void Speed::setSpeed(double speed, SpeedUnits units)
{
	isSet = true;
	if (speed == 0.0) {
		speedInMetersPerSecond = 0.0; //Prevent possible division by zero
		return;
	}
	switch (units) {
	case SpeedUnits::MilesPerHour:
		speedInMetersPerSecond = speed * 1609.344 * (1.0 / 3600.0);
		break;
	case SpeedUnits::KilometersPerHour:
		speedInMetersPerSecond = speed * 1000.0 *(1.0 / 3600.0);
		break;
	case SpeedUnits::MinutesPerKilometer:
		speedInMetersPerSecond = (1.0 / speed)*(1.0 / 60.0)*1000.0;
		break;
	case SpeedUnits::MinutesPerMile:
		speedInMetersPerSecond = (1.0 / speed)*1609.344*(1.0 / 60.0);
		break;
	default:
		speedInMetersPerSecond = speed;
	}
}

double Speed::getSpeed(SpeedUnits units)
{
	if (speedInMetersPerSecond == 0.0) {
		return 0.0; //Prevent division by zero possibility
	}
	switch (units) {
	case SpeedUnits::MilesPerHour:
		return speedInMetersPerSecond * (1.0 / 1609.344)*3600.0;
	case SpeedUnits::KilometersPerHour:
		return speedInMetersPerSecond * (1.0 / 1000.0)*3600.0;
	case SpeedUnits::MinutesPerKilometer:
		return (1.0 / speedInMetersPerSecond) * 60.0 * (1.0 / 1000.0);
	case SpeedUnits::MinutesPerMile:
		return (1.0 / speedInMetersPerSecond) * (1.0 / 1609.344)*60.0;
	default:
		return speedInMetersPerSecond;
	}
}

bool Speed::isSpeedSet()
{
	return isSet;
}
