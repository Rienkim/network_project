/*
 * sync.cpp
 *
 *  Created on: 2015. 6. 6.
 *      Author: suck
 */

#include "Sync.h"
#include "Calendar.h"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>

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


  return SUCCESS;
}



