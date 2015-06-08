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
	      << "<start date (dd-mm-yyyy)> <duration in minutes (max 24*60)> <IDs(id1,id2,...)>" << endl;
}


//------------------------------------------------------------------------------
int Sync::execute(Calendar& calendar, vector<string> &params)
{
  if(params.size() != 5)
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

  // Event is in future.===========================================
  if(event_now->compareEvent(event) != EARLIER)
  {
	  calendar.setSendData(string("sync"), params);
	  string recvdata_out;
	  while(1)
	  {
		  calendar.getRecvData(recvdata_out);
		  if(recvdata_out=="yes")
		  {
			  calendar.addEvent(event, false);
			  cout<<"event added"<<endl;
			  break;
		  }
		  else if(recvdata_out=="no")
		  {
			  cout<<"event addition failed"<<endl;
			  break;
		  }
		  else if(recvdata_out=="lost")
		  {
			  cout<<"connection lost"<<endl;
			  break;
		  }
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



