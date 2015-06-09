//------------------------------------------------------------------------------
// Event.cpp
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#include "Event.h"
#include "Create.h"
#include <iostream>
#include <iomanip>
#include <time.h>
#include <sstream>
#include <stdlib.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::setfill;
using std::setw;
using std::ostringstream;

//--------------------------------------------------------------------------
Event::Event()
    : name_(""), start_time_(NULL), duration_(NOT_INIT)
{
}

//--------------------------------------------------------------------------
Event::Event(const string& name, tm* time, short duration)
    : name_(name), start_time_(time), duration_(duration)
{
}

//--------------------------------------------------------------------------
Event::~Event() throw()
{
  if(start_time_ != NULL)
  {
    delete start_time_;
    start_time_ = NULL;
  }
}

//--------------------------------------------------------------------------
string Event::getName() const
{
  return name_;
}

//--------------------------------------------------------------------------
tm* Event::getStartTime() const
{
  return start_time_;
}

//--------------------------------------------------------------------------
short Event::getDuration() const
{
  return duration_;
}

//--------------------------------------------------------------------------
void Event::setName(const string& name)
{
  name_ = name;
}

//--------------------------------------------------------------------------
void Event::setStartTime(tm* start)
{
  start_time_ = start;
}

//--------------------------------------------------------------------------
void Event::setDuration(short duration)
{
  duration_ = duration;
}

//--------------------------------------------------------------------------
int Event::compareEvent(const Event* other) const
{
  if(this->start_time_->tm_year < other->start_time_->tm_year)
    return LATER;
  if(this->start_time_->tm_year > other->start_time_->tm_year)
    return EARLIER;

  // In same year.
  if(this->start_time_->tm_mon < other->start_time_->tm_mon)
    return LATER;
  if(this->start_time_->tm_mon > other->start_time_->tm_mon)
    return EARLIER;

  // In same month.
  double diff = difftime(timegm(this->start_time_), timegm(other->start_time_));

  // One day difference overlapping (other event one day later)
  if(this->start_time_->tm_mday - other->start_time_->tm_mday == -1)
    if((diff * -1) / 60 < this->duration_)
      return OVERLAP;

  // One day difference overlapping (other event one day earlier)
  if(this->start_time_->tm_mday - other->start_time_->tm_mday == 1)
    if(diff / 60 < other->duration_)
      return OVERLAP;

  if(this->start_time_->tm_mday < other->start_time_->tm_mday)
    return LATER;
  if(this->start_time_->tm_mday > other->start_time_->tm_mday)
    return EARLIER;

  // On same day.
  // Other event occurs diff seconds later than current event.
  if(diff < 0 && this->duration_ < (diff / 60))
    return LATER;

  if(diff > 0 && other->duration_ < (diff / 60))
    return EARLIER;

  return OVERLAP;
}

//--------------------------------------------------------------------------
vector<string> Event::eventToString() const
{
  vector<string> result;
  result.push_back(name_);

  ostringstream convert;
  string time_data;
  string date_data;

  convert << start_time_->tm_hour;
  time_data = convert.str();
  time_data += '-';
  convert.clear();
  convert << start_time_->tm_min;
  time_data += convert.str();
  result.push_back(time_data);

  convert.clear();
  convert << start_time_->tm_wday;
  date_data = convert.str();
  date_data += '-';
  convert.clear();
  convert << start_time_->tm_mon;
  date_data += convert.str();
  convert.clear();
  convert << start_time_->tm_year;
  date_data += convert.str();
  result.push_back(date_data);

  convert.clear();
  convert << duration_;
  result.push_back(convert.str());

  return result;
}

//--------------------------------------------------------------------------
int Event::stringToEvent(std::vector<std::string>& event_data)
{
  string name = event_data[0];
  string time = event_data[1];
  string date = event_data[2];
  string duration = event_data[3];
  int hour = NOT_INIT;
  int min = NOT_INIT;
  int day = NOT_INIT;
  int month = NOT_INIT;
  int year = NOT_INIT;

  if(atoi(duration.c_str()) > MAX_DURATION)
  {
    cout << "Wrong Duration: " << duration << endl;
    return USAGE_ERROR;
  }

  string delim = "-";
  size_t pos = 0;

  // get and check hour
  string token = time.substr(pos, time.find(delim));
  pos = token.length() + delim.length();
  hour = atoi(token.c_str());
  if(hour < 0 || hour > 23)
  {
    cout << "Wrong Hour: " << hour << endl;
    return USAGE_ERROR;
  }

  // get and check minute
  token = time.substr(pos);
  min = atoi(token.c_str());
  if(min < 0 || min > 59)
  {
    cout << "Wrong Minute: " << min << endl;
    return USAGE_ERROR;
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
    return USAGE_ERROR;
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
    return USAGE_ERROR;
  }

  name_ = name;

  start_time_ = new tm;
  start_time_->tm_hour = hour;
  start_time_->tm_min = min;
  start_time_->tm_mday = day;
  start_time_->tm_mon = month - MONTH_OFS;      // Month in range 0 - 11
  start_time_->tm_year = year - YEAR_BEGIN_CNT; // Years since 1900
  start_time_->tm_sec = 0;

  duration_ = atoi(duration.c_str());

  return SUCCESS;
}

//--------------------------------------------------------------------------
void Event::printInfo() const
{
  cout << "Event Name : " << name_ << endl << "Event Start: " << setfill('0')
      << setw(2) << start_time_->tm_mday << '-' << setfill('0') << setw(2)
      << start_time_->tm_mon + MONTH_OFS << '-' << setfill('0') << setw(2)
      << start_time_->tm_year + YEAR_BEGIN_CNT << " at " << setfill('0')
      << setw(2) << start_time_->tm_hour << ':' << setfill('0') << setw(2)
      << start_time_->tm_min << endl << "Event Dur. : " << setfill(' ')
      << setw(4) << int(duration_) << " min." << endl;
}
