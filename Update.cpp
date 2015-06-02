//------------------------------------------------------------------------------
// Update.cpp
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#include "Update.h"
#include "Calendar.h"
#include <string>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

//------------------------------------------------------------------------------
Update::Update(const string& name)
    : Command(name)
{
}

//------------------------------------------------------------------------------
Update::~Update() throw()
{
}

//------------------------------------------------------------------------------
int Update::execute(Calendar& calendar, vector<string> &params)
{
  if(params.size() != 0)
    return ERROR;

  if( &calendar == NULL || calendar.getQueue().size() == 0)
  {
    cout << "No Calendar entries yet!" << endl;
    return SUCCESS;
  }

  calendar.updateCalendar();

  return SUCCESS;
}
