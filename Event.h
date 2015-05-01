//------------------------------------------------------------------------------
// Event.h
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <ctime>

const int NOT_INIT = -1;

//------------------------------------------------------------------------------
// Event
// This class represents a calendar event.
//
// name_       = event name
// start_time_ = start time info of the event
// duration_   = duration of event in minutes
//
class Event
{
  private:
    std::string name_;
    tm* start_time_;
    short duration_;

    //--------------------------------------------------------------------------
    // Copy-Constructor
    // No Copies of Object are possible.
    //
    Event(const Event&);

    //--------------------------------------------------------------------------
    // = Operator-Overloading
    // No Copies of Object are possible.
    //
    Event& operator=(const Event&);

  public:
    //--------------------------------------------------------------------------
    // Default-Constructor
    // Sets name_ to NULL and the times to NOT_INIT.
    //
    Event();

    //--------------------------------------------------------------------------
    // Constructor
    //
    Event(const std::string& name, tm* time, short duration);

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Event() throw();

    //--------------------------------------------------------------------------
    // Getter Methods
    //
    std::string getName() const;
    tm* getStartTime() const;
    short getDuration() const;

    //--------------------------------------------------------------------------
    // Setter Methods
    //
    void setName(const std::string& name);
    void setStartTime(tm* time);
    void setDuration(short duration);

    //--------------------------------------------------------------------------
    // Print Event Info
    //
    void printInfo() const;
};

#endif // EVENT_H
