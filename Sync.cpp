/*
 * sync.cpp
 *
 *  Created on: 2015. 6. 6.
 *      Author: suck
 */

#include "Sync.h"
#include "Event.h"
#include "Calendar.h"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

//------------------------------------------------------------------------------
Sync::Sync(const string& name)
    : Command(name)
{
}

//------------------------------------------------------------------------------
Sync::~Sync() throw()
{
}

//------------------------------------------------------------------------------
void Sync::printUsage()
{
  cout << "Usage: create <event name> <start time (hour-min 24-hour format)> "
      << "<start date (dd-mm-yyyy)> <duration in minutes (max 24*60)> <IDs(id1,id2,...)>"
      << endl;
}

//------------------------------------------------------------------------------
int Sync::execute(Calendar& calendar, vector<string> &params)
{
  if(params.size() < 5)
  {
    cout << params.size() << endl;
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
  {
    if(calendar.isOverlap(event))
      return ERROR;

    char buffer[7] = { 0 };
    write(calendar.getServerSock(), "sync", 5);

    read(calendar.getServerSock(), buffer, 7);
    if(strcmp(buffer, "ack") != 0)
    {
      cout << "Handshake Error : sync not ack" << endl;
      return ERROR;
    }

    for(unsigned int i = 0; i < params.size(); i++)
    {
      char ack_buffer[4] = { 0 };
      write(calendar.getServerSock(), params[i].c_str(),
          strlen(params[i].c_str()) + 1);

      read(calendar.getServerSock(), ack_buffer, 4);
      if(strcmp(ack_buffer, "ack") != 0)
      {
        cout << "Handshake Error : data write ack" << endl;
        return ERROR;
      }
    }

    write(calendar.getServerSock(), "ended", 6);
    read(calendar.getServerSock(), buffer, 7);

    if((strcmp(buffer, "delete") == 0))
    {
      cout << "Event not created because of conflict with other client!" << endl;
      delete event;
    }

    if((strcmp(buffer, "create") == 0))
    {
      cout << "Event was created and synchronized with other clients!" << endl;
      calendar.addEvent(event);
    }

    if((strcmp(buffer, "delete") != 0) && (strcmp(buffer, "create") != 0))
    {
      cout << "Handshake Error : sync end ack" << endl;
      return ERROR;
    }
  }

  else
  {
    cout << "Event was in past and thus not created." << endl;
    delete event;
  }

  event_now->setStartTime(NULL);
  delete event_now;

  return SUCCESS;
}

