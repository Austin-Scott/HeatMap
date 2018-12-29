#pragma once

enum SpeedUnits { MetersPerSecond, MilesPerHour, KilometersPerHour, MinutesPerMile, MinutesPerKilometer };

class Speed {
private:
	double speedInMetersPerSecond;

	bool isSet;
public:
	Speed(double speed, SpeedUnits units = SpeedUnits::MetersPerSecond);
	Speed();
	void setSpeed(double speed, SpeedUnits units = SpeedUnits::MetersPerSecond);
	double getSpeed(SpeedUnits units = SpeedUnits::MetersPerSecond);

	bool isSpeedSet();
};