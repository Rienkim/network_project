//------------------------------------------------------------------------------
// Save.cpp
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#include "Save.h"
#include "Event.h"
#include <list>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using std::list;
using std::string;
using std::ofstream;
using std::ios_base;
using std::vector;
using std::cout;
using std::endl;

//------------------------------------------------------------------------------
Save::Save(const string &name)
    : Command(name)
{
}

//------------------------------------------------------------------------------
int Save::execute(Calendar& calendar, std::vector<std::string>& params)
{
  //check if parametercount is correct
  if(params.size() != 1)
  {
    cout << "Wrong Parameter count!" << endl;
    return ERROR;
  }

  std::stringstream name(params[0]);
  string out_name;
  getline(name, out_name, '.');
  out_name = out_name + ".cnw";
  ofstream out;
  out.open(out_name.c_str(), ios_base::out | ios_base::binary);

  if( !out.is_open())
  {
    cout << "Could not open file!" << endl;
    return ERROR;
  }

  out << "CNW" << '\0';

  int size = calendar.getQueue().size();

  out.write((char*) &size, sizeof(size));

  list<Event*> events = calendar.getQueue();
  short duration;
  int tm_data;

  for(list<Event*>::const_iterator it = events.begin(); it != events.end();
      ++it)
  {
    out << ( *it)->getName().c_str() << '\0';
    tm_data = ( *it)->getStartTime()->tm_mday;
    out.write((char*) &tm_data, sizeof(int));
    tm_data = ( *it)->getStartTime()->tm_mon;
    out.write((char*) &tm_data, sizeof(int));
    tm_data = ( *it)->getStartTime()->tm_year;
    out.write((char*) &tm_data, sizeof(int));
    tm_data = ( *it)->getStartTime()->tm_hour;
    out.write((char*) &tm_data, sizeof(int));
    tm_data = ( *it)->getStartTime()->tm_min;
    out.write((char*) &tm_data, sizeof(int));
    tm_data = ( *it)->getStartTime()->tm_sec;
    out.write((char*) &tm_data, sizeof(int));
    duration = ( *it)->getDuration();
    out.write((char*) &duration, sizeof(short));
  }

  out.close();

  return SUCCESS;
}

//------------------------------------------------------------------------------
Save::~Save() throw()
{

}
