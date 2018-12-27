#include "HeatMapCell.h"

HeatMapCell::HeatMapCell()
{
	value = 0.0;
	normalizedValue = 0.0;
}

double HeatMapCell::getValue()
{
	return value;
}

void HeatMapCell::addToValue(double amount)
{
	value += amount;
}

double HeatMapCell::getNormalizedValue()
{
	return normalizedValue;
}

void HeatMapCell::normalizeValue(double min, double max)
{
	normalizedValue = (value - min) / (max - min);
}
