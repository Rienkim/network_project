//------------------------------------------------------------------------------
// Quit.cpp
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#include "Quit.h"
#include "Calendar.h"
#include <string>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

//------------------------------------------------------------------------------
Quit::Quit(const string& name, bool init)
    : Command(name), loop_(init)
{
}

//------------------------------------------------------------------------------
Quit::~Quit() throw()
{
}

//------------------------------------------------------------------------------
bool Quit::getLoop() const
{
  return loop_;
}

//------------------------------------------------------------------------------
void Quit::setLoop(bool new_value)
{
  loop_ = new_value;
}

//------------------------------------------------------------------------------
int Quit::execute(Calendar& calendar, vector<string> &params)
{
  if(params.size() != 0)
    return PARAMETER_ERROR;

  cout << "Good Bye!" << endl;

  loop_ = false;

  return SUCCESS;
}
