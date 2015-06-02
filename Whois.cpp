/*
 * Whois.cpp
 *
 *  Created on: 2015. 6. 2.
 *      Author: JOHNSON
 */

#include "Whois.h"
#include "Calendar.h"
#include <string>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

//------------------------------------------------------------------------------
Whois::Whois(const string& name)
    : Command(name)
{
}

//------------------------------------------------------------------------------
Whois::~Whois() throw()
{
}


//------------------------------------------------------------------------------
int Whois::execute(Calendar& calendar, vector<string> &params)
{
  if(params.size() != 0)
    return ERROR;

  cout << "Good Bye!" << endl;


  return SUCCESS;
}



