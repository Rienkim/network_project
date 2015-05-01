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
  delete start_time_;
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
void Event::printInfo() const
{
  cout << "Event Name : " << name_ << endl << "Event Start: " << setfill('0')
      << setw(2) << start_time_->tm_mday << '-' << setfill('0') << setw(2)
      << start_time_->tm_mon << '-' << setfill('0') << setw(2)
      << start_time_->tm_year << " at " << setfill('0') << setw(2)
      << start_time_->tm_hour << ':' << setfill('0') << setw(2)
      << start_time_->tm_min << endl << "Event Dur. : " << setfill(' ')
      << setw(4) << int(duration_) << " min." << endl;
}
