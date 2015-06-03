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
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;
using std::string;

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

  Event* event = new Event();
  int ret = event->stringToEvent(params);

  if(ret == USAGE_ERROR)
  {
    printUsage();
    delete event;
    return ERROR;
  }

  time_t t_now = std::time(NULL);
  tm* now = localtime( &t_now);

  Event* event_now = new Event("now", now, 0);

  // Event is in future.
  if(event_now->compareEvent(event) != EARLIER)
    calendar.addEvent(event, false);

  else
  {
    cout << "Event was in past and thus not created." << endl;
    delete event;
  }

  event_now->setStartTime(NULL);
  delete event_now;

  return SUCCESS;
}
