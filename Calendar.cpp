//------------------------------------------------------------------------------
// Calendar.cpp
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#include "Calendar.h"
#include "Event.h"
#include "Show.h"
#include "Quit.h"
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using std::list;
using std::string;
using std::map;
using std::stringstream;
using std::vector;
using std::cout;
using std::endl;
using std::cin;

//--------------------------------------------------------------------------
Calendar::Calendar()
{
}

//--------------------------------------------------------------------------
Calendar::Calendar(std::list<Event*> events)
    : events_(events)
{
}

//--------------------------------------------------------------------------
Calendar::~Calendar() throw()
{
  for(list<Event*>::const_iterator it = events_.begin(); it != events_.end();
        ++it)
      delete (*it);
}

//--------------------------------------------------------------------------
list<Event*> Calendar::getEvents() const
{
  return events_;
}

//--------------------------------------------------------------------------
void Calendar::setEvents(const std::list<Event*> events)
{
  events_ = events;
}

void Calendar::addEvent(Event* event)
{
  events_.push_back(event);
}

//-----------------------------------------------------------------------------
int Calendar::run()
{
  Command* actual;
  map<string, Command*> commands;

  Command* show = new Show("Show");
  Command* quit = new Quit("Quit", true);

  stringstream divide_buffer;
  int error = 0;
  string current_command;
  string input;
  string token;
  vector<string> parameter;

  //initialize map
  commands["show"] = show;
  commands["quit"] = quit;

  // loop stops if quit.loop_ gets 0
  while(dynamic_cast<Quit*>(quit)->getLoop())
  {
    cout << "cnw> ";
    getline(cin, input);
    divide_buffer.str(input);

    // split input into vector
    divide_buffer >> current_command;
    int count_div = 1;

    // get parameters
    while(getline(divide_buffer, token, '"'))
    {
      if(count_div % 2 != 0)
      {
        stringstream s_buffer(token);
        while(getline(s_buffer, input, ' '))
        {
          if(input.empty() != true)
            parameter.push_back(input);
        }
      }
      else
      {
        if(token.empty() != true)
          parameter.push_back(token);
      }
      count_div++;
    }

    // validate Command and execute it
    if(commands.find(current_command) != commands.end())
    {
      actual = commands.find(current_command)->second;
      error = actual->execute(*this, parameter);
    }
    else
    {
      if(current_command.empty() != true)
        cout << "Error: Unknown command!\n";
    }

    if(error == ERROR)
      cout << "Error: Unknown command!\n";

    //Clear Strings for next turn
    parameter.clear();
    current_command.clear();
    input.clear();
    divide_buffer.clear();
    error = SUCCESS;
  }

  //free used Pointers
  delete show;
  delete quit;

  return SUCCESS; // return 0 if success
}

//--------------------------------------------------------------------------
void Calendar::printEvents() const
{
  for(list<Event*>::const_iterator it = events_.begin(); it != events_.end();
      ++it)
    (*it)->printInfo();
}
