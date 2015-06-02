//------------------------------------------------------------------------------
// Show.cpp
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#include "Show.h"
#include "Calendar.h"
#include <string>
#include <iostream>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::vector;

//------------------------------------------------------------------------------
Show::Show(const string& name)
    : Command(name)
{
}

//------------------------------------------------------------------------------
Show::~Show() throw()
{
}

//------------------------------------------------------------------------------
int Show::execute(Calendar& calendar, vector<string>& params)
{
  if(params.size() != 0)
    return ERROR;

  if(&calendar == NULL || calendar.getQueue().size() == 0)
  {
    cout << "No Calendar entries yet!" << endl;
    return SUCCESS;
  }

  calendar.printEvents();

  return SUCCESS;
}
