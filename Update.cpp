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

using std::string;
using std::vector;

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

  calendar.updateCalendar();

  return SUCCESS;
}
