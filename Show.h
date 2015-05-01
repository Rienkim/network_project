//------------------------------------------------------------------------------
// Show.h
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#ifndef SHOW_H
#define SHOW_H

#include "Command.h"

//------------------------------------------------------------------------------
// Show Class inherited from Command-Class
// Representing the "show" command called from calendar.run()
//
class Show : public Command
{
  private:
    //--------------------------------------------------------------------------
    // Copy-Constructor
    // No Copies of Object are possible.
    //
    Show(const Show&);

    //--------------------------------------------------------------------------
    // = Operator-Overloading
    // No Copies of Object are possible.
    //
    Show& operator=(const Show&);

  public:
    //--------------------------------------------------------------------------
    // Constructor
    //
    Show(const std::string& name);

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Show() throw();

    //--------------------------------------------------------------------------
    // Virtual Function from Command-Class executing Command
    // Prints events of the calendar
    //
    // @param calendar current Calendar Object
    // @param params string vector of parameter
    // @return Returns ERROR or SUCCESS
    //
    int execute(Calendar& calendar, std::vector<std::string>& params);
};

#endif // SHOW_H
