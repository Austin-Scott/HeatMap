#include "HeatMapCell.h"

HeatMapCell::HeatMapCell()
{
	value = 0.0;
	normalizedValue = 0.0;
	activities = 0;
	setThisCycle = false;
	deltaValue = 0.0;
}

double HeatMapCell::getValue()
{
	return value;
}

void HeatMapCell::addToValue(double amount)
{
	if (deltaValue + amount<=1.0) {
		value += amount;
		deltaValue += amount;
	}
	else {
		value += 1.0 - deltaValue;
		deltaValue = 1.0;
	}
	setThisCycle = true;
}

double HeatMapCell::getNormalizedValue()
{
	return normalizedValue;
}

void HeatMapCell::setNormalizedValue(double amount)
{
	normalizedValue = amount;
}

void HeatMapCell::incrementCounter()
{
	deltaValue = 0.0;
	if (setThisCycle) {
		activities++;
		setThisCycle = false;
	}
}

int HeatMapCell::getActivities()
{
	return activities;
}
