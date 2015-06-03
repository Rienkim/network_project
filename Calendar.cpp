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
#include "Create.h"
#include "Quit.h"
#include "Whois.h"
#include "Update.h"
#include "Signin.h"
#include <iostream>
#include <map>
#include <sstream>

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
Calendar::Calendar(vector<list<Event*>*> calendar_queue)
    : calendar_queue_(calendar_queue), next_event_(NULL), isConnected(false)
{
}

//--------------------------------------------------------------------------
Calendar::~Calendar() throw()
{
  for(int i = 0; i < 4; i++)
  {
    for(list<Event*>::const_iterator it = calendar_queue_[i]->begin();
        it != calendar_queue_[i]->end(); ++it)
      delete ( *it);
    delete calendar_queue_[i];
  }
}

//--------------------------------------------------------------------------
vector<list<Event*>*> Calendar::getQueue() const
{
  return calendar_queue_;
}

//--------------------------------------------------------------------------
const Event* Calendar::getNextEvent() const
{
  return next_event_;
}

//--------------------------------------------------------------------------
const bool Calendar::getConnectionState()
{
	return isConnected;
}

//--------------------------------------------------------------------------
void Calendar::setQueue(const vector<list<Event*>*> calendar_queue)
{
  calendar_queue_ = calendar_queue;
}

//--------------------------------------------------------------------------
void Calendar::setNextEvent(Event* next_event)
{
  next_event_ = next_event;
}

//--------------------------------------------------------------------------
void Calendar::setConnectionState(bool state)
{
	isConnected = state;
}

//--------------------------------------------------------------------------
void Calendar::addEvent(Event* event)
{
  // Get event quarter number.
  int quarter = event->calcQuarter();
  bool inserted = false;

  for(list<Event*>::iterator it = calendar_queue_[quarter]->begin();
      it != calendar_queue_[quarter]->end(); ++it)
  {
    int ret = ( *it)->compareEvent(event);

    if(ret == OVERLAP)
    {
      cout << "Event could not be added to calendar due to overlapping with: "
          << endl;
      ( *it)->printInfo();
      delete event;
      return;
    }

    if(ret == LATER)
      continue;

    // Else insert event here.
    calendar_queue_[quarter]->insert(it, event);
    inserted = true;
    break;
  }

  if( !inserted)
  {
    calendar_queue_[quarter]->push_back(event);
  }

  if(next_event_ == NULL || next_event_->compareEvent(event) == EARLIER)
    next_event_ = event;
}

//-----------------------------------------------------------------------------
int Calendar::run()
{
  Command* actual;
  map<string, Command*> commands;

  Command* show = new Show("Show");
  Command* create = new Create("Create");
  Command* quit = new Quit("Quit", true);
  Command* update = new Update("Update");
  Command* whois = new Whois("Whois");
  Command* signin = new Signin("Signin");

  stringstream divide_buffer;
  int error = 0;
  string current_command;
  string input;
  string token;
  vector<string> parameter;

//initialize map
  commands["show"] = show;
  commands["create"] = create;
  commands["quit"] = quit;
  commands["update"] = update;
  commands["whois"] = whois;
  commands["signin"] = signin;

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
      error = actual->execute( *this, parameter);
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
  delete create;
  delete quit;
  delete update;
  delete whois;
  delete signin;

  return SUCCESS; // return 0 if success
}

//--------------------------------------------------------------------------
void Calendar::updateCalendar()
{
  time_t t_now = std::time(NULL);
  tm* now = localtime( &t_now);
  Event* event_now = new Event("now", now, 0);
  double diff = event_now->compareEvent(next_event_);

  // Next Event in list occurs in future or is scheduled at the moment.
  if(diff == LATER || diff == OVERLAP)
    return;

  // Else delete old events.
  for(int i = next_event_->calcQuarter(); i <= event_now->calcQuarter(); i++)
  {
    for(list<Event*>::iterator it = calendar_queue_[i]->begin();
        it != calendar_queue_[i]->end(); ++it)
    {
      if( *it != next_event_)
        continue;
      // TODO: Implement here!!!
    }
  }
}

//--------------------------------------------------------------------------
void Calendar::printEvents() const
{
  cout << "----- Next Event: " << endl;
  next_event_->printInfo();
  cout << "-----" << endl;
  for(int i = 0; i < 4; i++)
  {
    for(list<Event*>::const_iterator it = calendar_queue_[i]->begin();
        it != calendar_queue_[i]->end(); ++it)
      ( *it)->printInfo();
  }
}
