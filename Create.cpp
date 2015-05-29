//------------------------------------------------------------------------------
// Create.cpp
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#include "Create.h"
#include "Event.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;
using std::string;

const int MAX_FUTURE_YEAR = 15;
const int MAX_DURATION = 24 * 60;
const int YEAR_BEGIN_CNT = 1900;

//------------------------------------------------------------------------------
Create::Create(const std::string &name)
    : Command(name)
{
}

//------------------------------------------------------------------------------
Create::~Create() throw()
{
}

//------------------------------------------------------------------------------
void Create::printUsage()
{
  cout << "Usage: create <event name> <start time (hour-min 24-hour format)> "
      << "<start date (dd-mm-yyyy)> <duration in minutes (max 24*60)>" << endl;
}

//------------------------------------------------------------------------------
int Create::execute(Calendar& calendar, std::vector<std::string>& params)
{
  //check for Parametersize
  if(params.size() != 4)
  {
    printUsage();
    return ERROR;
  }

  string name = params[0];
  string time = params[1];
  string date = params[2];
  string duration = params[3];
  int hour = NOT_INIT;
  int min = NOT_INIT;
  int day = NOT_INIT;
  int month = NOT_INIT;
  int year = NOT_INIT;

  if(atoi(duration.c_str()) > MAX_DURATION)
  {
    cout << "Wrong Duration: " << duration << endl;
    printUsage();
    return ERROR;
  }

  string delim = "-";
  size_t pos = 0;

  // get and check hour
  string token = time.substr(pos, time.find(delim));
  pos = token.length() + delim.length();
  hour = atoi(token.c_str());
  if(hour < 0 || hour >= 24)
  {
    cout << "Wrong Hour: " << hour << endl;
    printUsage();
    return ERROR;
  }

  // get and check minute
  token = time.substr(pos);
  min = atoi(token.c_str());
  if(min < 0 || min > 59)
  {
    cout << "Wrong Minute: " << min << endl;
    printUsage();
    return ERROR;
  }

  // get day, month, year
  pos = 0;
  token = date.substr(pos, date.find(delim));
  pos = token.length() + delim.length();
  day = atoi(token.c_str());
  token = date.substr(pos, date.find(delim));
  pos += token.length() + delim.length();
  month = atoi(token.c_str());
  token = date.substr(pos);
  year = atoi(token.c_str());

  time_t t_now = std::time(NULL);
  tm* now = localtime( &t_now);

  // check year and month
  if(year < now->tm_year + YEAR_BEGIN_CNT
      || year > now->tm_year + YEAR_BEGIN_CNT + MAX_FUTURE_YEAR || month < 1
      || month > 12)
  {
    cout << "Wrong Year/Month: " << year << '/' << month << endl;
    cout << "Now:              " << now->tm_year + YEAR_BEGIN_CNT << endl;
    printUsage();
    return ERROR;
  }

  // find out if leap year
  int leap_year = false;
  if(((year % 4 == 0) && !(year % 100 == 0)) || (year % 400 == 0))
    leap_year = true;

  // check day
  if(day < 1 || (month == 2 && leap_year && day > 29)
      || (month == 2 && !leap_year && day > 28)
      || (month % 2 == 1 && month < 8 && day > 31)
      || (month % 2 == 1 && month > 8 && day > 30)
      || (month % 2 == 0 && month != 2 && month < 7 && day > 30)
      || (month % 2 == 0 && month > 7 && day > 31))
  {
    cout << "Wrong Day: " << day << endl;
    printUsage();
    return ERROR;
  }

  tm* start_time = new tm;
  start_time->tm_hour = hour;
  start_time->tm_min = min;
  start_time->tm_mday = day;
  start_time->tm_mon = month - 1;
  start_time->tm_year = year - YEAR_BEGIN_CNT;
  start_time->tm_sec = 0;

  Event* event = new Event(name, start_time, atoi(duration.c_str()));

  calendar.addEvent(event);

  return SUCCESS;
}
