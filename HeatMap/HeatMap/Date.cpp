#include <math.h>
#include <ctime>

#include "Date.h"

int Date::getDaysInMonth(Month month, int year)
{
	if (month != Month::February) {
		if (month < Month::August) {
			if (month % 2 == 0) {
				return 30;
			}
			else {
				return 31;
			}
		}
		else {
			if (month % 2 == 0) {
				return 31;
			}
			else {
				return 30;
			}
		}
	}
	else {
		if (year % 4 == 0 && (year % 100 != 0 || (year % 100 == 0 && year % 400 == 0))) { //If it is a leap year
			return 29;
		}
		else {
			return 28;
		}
	}
}

Date::Date()
{
}

Date::Date(int year, Month month, int day, int hour, int minute, int second)
{
	this->year = year;
	this->month = month;
	this->day = day;
	this->hour = hour;
	this->minute = minute;
	this->second = second;
}

string Date::toString()
{
	return string();
}

bool Date::isMoreRecentThan(Date other)
{
	if (year > other.getYear())
		return true;
	if (year < other.getYear())
		return false;
	if (month > other.getMonth())
		return true;
	if (month < other.getMonth())
		return false;
	if (day > other.getDay())
		return true;
	if (day < other.getDay())
		return false;
	if (hour > other.getHour())
		return true;
	if (hour < other.getHour())
		return false;
	if (minute > other.getMinute())
		return true;
	if (minute < other.getMinute())
		return false;
	if (second > other.getSecond())
		return true;
	if (second < other.getSecond())
		return false;

	return false; //They are equal, therefore this is not more recent.
}

bool Date::isOlderThan(Date other)
{
	if (!operator==(other) && !isMoreRecentThan(other))
		return true;

	return false;
}

bool Date::isConcurrentWith(Date other)
{
	return operator==(other);
}

bool Date::operator==(Date other) const
{
	return year == other.getYear() && month == other.getMonth() && day == other.getDay() && hour == other.getHour() && minute == other.getMinute() && second == other.getSecond();
}

int Date::getYear()
{
	return year;
}

Month Date::getMonth()
{
	return month;
}

int Date::getDay()
{
	return day;
}

int Date::getHour()
{
	return hour;
}

int Date::getMinute()
{
	return minute;
}

int Date::getSecond()
{
	return second;
}

Weekday Date::getDayOfWeek()
{
	//Adapted from the algorithm posted on: https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
	return static_cast<Weekday>(((day += month < 3 ? year-- : year - 2, 23 * month / 9 + day + 4 + year / 4 - year / 100 + year / 400) - 1) % 7);
}

Date Date::now()
{
	time_t currentTime = time(NULL);
	tm* ltm = localtime(&currentTime);

	return Date::createDate(ltm->tm_year + 1970, static_cast<Month>(ltm->tm_mon + 1), ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

Date Date::createDate(int year, Month month, int day, int hour, int minute, int second)
{
	return Date(year, month, day, hour, minute, second);
}

Date Date::parseDateString(string dateString)
{
	return Date();
}

bool Date::isValidDate()
{
	if (day < 1 || hour < 0 || minute < 0 || second < 0) return false;
	if (day > getDaysInMonth(month, year) || hour > 23 || minute > 59 || second > 59 /*Should I worry about leap seconds?*/) return false;
	return true;
}

Date Date::offsetByNumOfDays(int numOfDays)
{
	return Date();
}
