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
Whois::Whois(const string& name, bool init)
    : Command(name), loop_(init)
{
}

//------------------------------------------------------------------------------
Whois::~Whois() throw()
{
}

//------------------------------------------------------------------------------
bool Whois::getLoop() const
{
  return loop_;
}

//------------------------------------------------------------------------------
void Whois::setLoop(bool new_value)
{
  loop_ = new_value;
}

//------------------------------------------------------------------------------
int Whois::execute(Calendar& calendar, vector<string> &params)
{
  if(params.size() != 0)
    return ERROR;

  cout << "Good Bye!" << endl;

  loop_ = false;

  return SUCCESS;
}



