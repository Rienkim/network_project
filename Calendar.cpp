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
#include "Save.h"
#include "Signin.h"
#include "Sync.h"
#include <iostream>
#include <map>
#include <sstream>
#include <string.h>

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
Calendar::Calendar(list<Event*> calendar_queue)
    : calendar_queue_(calendar_queue)
{
}

//--------------------------------------------------------------------------
Calendar::~Calendar() throw()
{
  for(list<Event*>::const_iterator it = calendar_queue_.begin();
      it != calendar_queue_.end(); ++it)
    delete ( *it);
}

//--------------------------------------------------------------------------
list<Event*> Calendar::getQueue() const
{
  return calendar_queue_;
}

//--------------------------------------------------------------------------
const bool Calendar::getConnectionState()
{
  return isConnected;
}

//--------------------------------------------------------------------------
void Calendar::getID(char* buffer)
{
	strcpy(buffer, ID);
}

//--------------------------------------------------------------------------
void Calendar::getSendData(string& command_input, vector<string>& data_input)
{
	pthread_mutex_unlock(&mutx);
	command_input = command;
	data_input = data;
	pthread_mutex_unlock(&mutx);
}

//--------------------------------------------------------------------------
void Calendar::getRecvData(string& data_input)
{
	data_input = recvdata;
}

//--------------------------------------------------------------------------
void Calendar::setQueue(const list<Event*> calendar_queue)
{
  calendar_queue_ = calendar_queue;
}

//--------------------------------------------------------------------------
void Calendar::setConnectionState(bool state)
{
  isConnected = state;
}

//--------------------------------------------------------------------------
void Calendar::setID(const char *IDstr)
{
	strcpy(ID, IDstr);
}

//--------------------------------------------------------------------------
void Calendar::setSendData(string command_input, vector<string> data_input)
{
	pthread_mutex_lock(&mutx);
	command = command_input;
	data = data_input;
	pthread_mutex_unlock(&mutx);
}

//--------------------------------------------------------------------------
void Calendar::setRecvData(string data_input)
{
	recvdata = data_input;
}

void Calendar::lockMutex()
{
	pthread_mutex_lock(&mutx);
}

void Calendar::unlockMutex()
{
	pthread_mutex_unlock(&mutx);
}

bool Calendar::isOverlap(vector<string> event_string)
{
	  bool inserted = false;
	  Event event;
	  event.stringToEvent(event_string);

	  for(list<Event*>::iterator it = calendar_queue_.begin();
	      it != calendar_queue_.end(); ++it)
	  {
	    int ret = ( *it)->compareEvent(&event);

	    if(ret == OVERLAP)
	    {
	      cout << "Event could not be added to calendar due to overlapping with: "
	          << endl;
	      ( *it)->printInfo();
	      return true;
	    }

	    if(ret == LATER)
	      continue;

	    // TODO: WAIT FOR REPLY OF SERVER. IF YES INSERT, IF NO DELETE EVENT.
	    return false;
	  }

	return false;
}

void Calendar::addEvent(Event* event, bool wait_server)
{
  bool inserted = false;

  for(list<Event*>::iterator it = calendar_queue_.begin();
      it != calendar_queue_.end(); ++it)
  {
    int ret = ( *it)->compareEvent(event);

    if(ret == OVERLAP)
    {
      cout << "Event could not be added to calendar due to overlapping with: "
          << endl;
      ( *it)->printInfo();
      delete event;

      // TODO: TELL SERVER THAT EVENT CANNOT BE SCHEDULED!

      return;
    }

    if(ret == LATER)
      continue;

    // Else insert event here.
    // TODO: TELL SEVER THAT INSERT IS POSSIBLE.
    while(wait_server);

    // TODO: WAIT FOR REPLY OF SERVER. IF YES INSERT, IF NO DELETE EVENT.
    calendar_queue_.insert(it, event);
    inserted = true;
    break;
  }

  if( !inserted)
  {
    // TODO: TELL SEVER THAT INSERT IS POSSIBLE.
    while(wait_server);

    // TODO: WAIT FOR REPLY OF SERVER. IF YES INSERT, IF NO DELETE.
    calendar_queue_.push_back(event);
  }
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
  Command* save = new Save("Save");
  Command* whois = new Whois("Whois");
  Command* signin = new Signin("Signin");
  Command* sync = new Sync("Sync");

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
  commands["save"] = save;
  commands["whois"] = whois;
  commands["signin"] = signin;
  commands["sync"] = sync;

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
  delete save;
  delete whois;
  delete signin;
  delete sync;

  return SUCCESS; // return 0 if success
}

//--------------------------------------------------------------------------
void Calendar::updateCalendar()
{
  time_t t_now = std::time(NULL);
  tm* now = localtime( &t_now);
  Event* event_now = new Event("now", now, 0);

  while(calendar_queue_.begin() != calendar_queue_.end()
      && event_now->compareEvent(calendar_queue_.front()) == EARLIER)
  {
    delete calendar_queue_.front();
    calendar_queue_.pop_front();
  }

  event_now->setStartTime(NULL);
  delete event_now;
}

//--------------------------------------------------------------------------
void Calendar::printEvents() const
{
  for(list<Event*>::const_iterator it = calendar_queue_.begin();
      it != calendar_queue_.end(); ++it)
    ( *it)->printInfo();
}
