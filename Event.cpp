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

using std::string;
using std::cout;
using std::endl;
using std::setfill;
using std::setw;

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
