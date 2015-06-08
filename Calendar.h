//------------------------------------------------------------------------------
// Calendar.h
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#ifndef CALENDAR_H
#define CALENDAR_H

#include <list>
#include <iostream>
#include <vector>
#include <pthread.h>
using namespace std;


const int SUCCESS = 0;
const int ERROR = 1;

class Event;
// This class represents a calendar day.
//
//
class Calendar
{//suck joon ba bo
  private:
    std::list<Event*> calendar_queue_;
    char ID[40];
    bool isConnected;
    string recvdata;
    string command;
    vector<string> data;
    pthread_mutex_t mutx;

    //--------------------------------------------------------------------------
    // Copy-Constructor
    // No Copies of Object are possible.
    //
    Calendar(const Calendar&);

    //--------------------------------------------------------------------------
    // = Operator-Overloading
    // No Copies of Object are possible.
    //
    Calendar& operator=(const Calendar&);

  public:
    //--------------------------------------------------------------------------
    // Default-Constructor
    //
    Calendar();

    //--------------------------------------------------------------------------
    // Constructor
    //
    Calendar(std::list<Event*> calendar_queue);

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Calendar() throw();

    //--------------------------------------------------------------------------
    // Getter Methods
    //
    std::list<Event*> getQueue() const;
    const bool getConnectionState();
    void getID(char* buffer);
    void getSendData(string& command_input, vector<string>& data_input);
    void getRecvData(string& data_input);
    void lockMutex();

    //--------------------------------------------------------------------------
    // Setter Methods
    //
    void setQueue(const std::list<Event*> calendar_queue);
    void setConnectionState(bool);
    void setID(const char *IDstr);
    void setSendData(string command_input, vector<string> data_input);
    void setRecvData(string data_input);
    void unlockMutex();

    //--------------------------------------------------------------------------
    // Check whether overlap or not
    //
    bool isOverlap(vector<string> event_string);

    //--------------------------------------------------------------------------
    // Add new event
    //
    void addEvent(Event* event, bool wait_server);

    //--------------------------------------------------------------------------
    // Function to start the calendar.
    //
    int run();

    //--------------------------------------------------------------------------
    // Delete old events and set next event pointer.
    //
    void updateCalendar();

    //--------------------------------------------------------------------------
    // Print Events of Calendar
    //
    void printEvents() const;
};

#endif // CALENDAR_H
