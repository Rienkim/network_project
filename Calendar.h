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

#include <vector>
#include <list>

const int SUCCESS = 0;
const int ERROR = 1;

class Event;

//------------------------------------------------------------------------------
// Day
// This class represents a calendar day.
//
//
class Calendar
{
  private:
    std::vector<std::list<Event*>*> calendar_queue_;
    Event* next_event_;
    bool isConnected;

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
    Calendar(std::vector<std::list<Event*>*> calendar_queue_);

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Calendar() throw();

    //--------------------------------------------------------------------------
    // Getter Methods
    //
    std::vector<std::list<Event*>*> getQueue() const;
    const Event* getNextEvent() const;
    const bool getConnectionState();

    //--------------------------------------------------------------------------
    // Setter Methods
    //
    void setQueue(const std::vector<std::list<Event*>*> calendar_queue);
    void setNextEvent(Event* next_event);
    void setConnectionState(bool);

    //--------------------------------------------------------------------------
    // Add new event
    //
    void addEvent(Event* event);

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
