//------------------------------------------------------------------------------
// main.cpp
//
// CSED353
//
// Paul Schreiner
// TODO: Add your name here.
//------------------------------------------------------------------------------
//

#include "Event.h"
#include "Calendar.h"
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;
using std::vector;
using std::list;

//--------------------------------------------------------------------------
// Main Function
//
int main(int argc, char *argv[])
{
  tm* begin1 = new tm;
  begin1->tm_hour = 21;
  begin1->tm_min = 00;
  begin1->tm_mday = 30;
  begin1->tm_mon = 4;
  begin1->tm_year = 2015 - 1900;

  tm* begin2 = new tm;
  memcpy(begin2, begin1, sizeof(tm));
  begin2->tm_mday = 18;

  Event* event1 = new Event("Event 1", begin1, 5*60);
  Event* event2 = new Event("Event 2", begin2, 3 * 60);

  vector<list<Event*>*> calendar_queue;
  list<Event*>* quarter_1 = new list<Event*>;
  list<Event*>* quarter_2 = new list<Event*>;
  list<Event*>* quarter_3 = new list<Event*>;
  list<Event*>* quarter_4 = new list<Event*>;

  calendar_queue.push_back(quarter_1);
  calendar_queue.push_back(quarter_2);
  calendar_queue.push_back(quarter_3);
  calendar_queue.push_back(quarter_4);

  Calendar calendar(calendar_queue);

  calendar.addEvent(event1);
  calendar.addEvent(event2);

  calendar.run();

  return SUCCESS;
}
