//------------------------------------------------------------------------------
// Save.h
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//


#ifndef SAVE_H_
#define SAVE_H_

#include "Command.h"

//------------------------------------------------------------------------------
// Save Class inherited from Command-Class
// Representing the "save" command called from calendar.run()
//
class Save : public Command
{
  private:
    //--------------------------------------------------------------------------
    // Copy-Constructor
    // No Copies of Object are possible.
    //
    Save(const Save&);

    //--------------------------------------------------------------------------
    // = Operator-Overloading
    // No Copies of Object are possible.
    //
    Save& operator=(const Save&);

  public:
    //--------------------------------------------------------------------------
    // Constructor with Parameter for Command Constructor
    //
    Save(const std::string &name);

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Save() throw();

    //--------------------------------------------------------------------------
    // Virtual Function from Command-Class executing Command
    // saves a .cnw configfile for the calendar
    //
    // @param board current Calendar Object
    // @param params string vector of parameter
    // @return Returns ERROR or SUCCESS
    //
    int execute(Calendar& calendar, std::vector<std::string>& params);
};

#endif // SAVE_H

