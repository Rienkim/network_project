//------------------------------------------------------------------------------
// main.cpp
//
// CSED353
//
// Paul Schreiner
// TODO: Add your name here.
// commit test
//------------------------------------------------------------------------------
//

#include "Event.h"
#include "Calendar.h"
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;
using std::list;

//--------------------------------------------------------------------------
// Main Function
//
int main(int argc, char *argv[])
{
  tm* begin1 = new tm;
  begin1->tm_hour = 12;
  begin1->tm_min = 30;
  begin1->tm_mday = 1;
  begin1->tm_mon = 1;
  begin1->tm_year = 2015;

  tm* begin2 = new tm;
  memcpy(begin2, begin1, sizeof(tm));
  begin2->tm_mday = 2;

  Event* event1 = new Event("Event 1", begin1, 9*60);
  Event* event2 = new Event("Event 2", begin2, 3*60);

  list<Event*> events;
  events.push_back(event1);
  events.push_back(event2);

  Calendar calendar(events);

  calendar.run();

  return SUCCESS;
}
