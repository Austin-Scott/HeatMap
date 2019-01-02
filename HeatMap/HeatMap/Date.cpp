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
	isSet = false;
}

Date::Date(int year, Month month, int day, int hour, int minute, int second)
{
	isSet = true;
	this->year = year;
	this->month = month;
	this->day = day;
	this->hour = hour;
	this->minute = minute;
	this->second = second;
}

string Date::toString()
{
	string monthStr = "";
	switch (month) {
	case Month::January:
		monthStr = "January";
		break;
	case Month::February:
		monthStr = "February";
		break;
	case Month::March:
		monthStr = "March";
		break;
	case Month::April:
		monthStr = "April";
		break;
	case Month::May:
		monthStr = "May";
		break;
	case Month::June:
		monthStr = "June";
		break;
	case Month::July:
		monthStr = "July";
		break;
	case Month::August:
		monthStr = "August";
		break;
	case Month::September:
		monthStr = "September";
		break;
	case Month::October:
		monthStr = "October";
		break;
	case Month::November:
		monthStr = "November";
		break;
	case Month::December:
		monthStr = "December";
		break;
	}
	return monthStr + " " + to_string(day) + ", " + to_string(year) + " " + to_string(hour) + ":" + to_string(minute) + ":" + to_string(second);
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

Date Date::createDate(time_t seconds)
{
	seconds -= 86400;
	tm* ltm = localtime(&seconds);

	return Date::createDate(ltm->tm_year + 1920, static_cast<Month>(ltm->tm_mon + 1), ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

Date Date::parseDateString(string dateString)
{
	if (dateString.size() == 24) {
		Date result(atoi(dateString.substr(0, 4).c_str()), static_cast<Month>(atoi(dateString.substr(5, 2).c_str())), atoi(dateString.substr(8, 2).c_str()), atoi(dateString.substr(11, 2).c_str()), atoi(dateString.substr(14, 2).c_str()), atoi(dateString.substr(17, 2).c_str()));
		//That was in Zulu time. Convert to local time.
		struct tm zuluTime;
		zuluTime.tm_year = result.getYear() - 1900;
		zuluTime.tm_mon = result.getMonth() - 1;
		zuluTime.tm_mday = result.getDay();
		zuluTime.tm_hour = result.getHour();
		zuluTime.tm_min = result.getMinute();
		zuluTime.tm_sec = result.getSecond();

		time_t unixTime = mktime(&zuluTime) - _timezone;
		tm* ltm = localtime(&unixTime); //Convert to the current time zone

		return Date::createDate(ltm->tm_year + 1900, static_cast<Month>(ltm->tm_mon + 1), ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
		
	}
	return Date();
}

Date Date::fromFormalString(string dateString)
{
	if (dateString.length() == 10) {
		regex e("\\d{4}-\\d{2}-\\d{2}");
		if (regex_match(dateString, e)) {
			int year = atoi(dateString.substr(0, 4).c_str());
			int monthInt = atoi(dateString.substr(5, 2).c_str());
			Month month = Month::January;
			if (monthInt >= 1 && monthInt <= 12) {
				month = static_cast<Month>(monthInt);
			}
			int day = atoi(dateString.substr(8, 2).c_str());
			return Date(year, month, day, 0, 0, 0);
		}
	}
	return Date();
}

bool Date::isValidDate()
{
	if (day < 1 || hour < 0 || minute < 0 || second < 0) return false;
	if (day > getDaysInMonth(month, year) || hour > 23 || minute > 59 || second > 59 /*Should I worry about leap seconds?*/) return false;
	return true;
}

bool Date::isDateSet()
{
	return isSet;
}

string Date::toFormalString()
{
	if (isSet) {
		return padZeros(4, year) + "-" + padZeros(2, (int)month) + "-" + padZeros(2, day);
	}
}
