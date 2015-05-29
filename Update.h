//------------------------------------------------------------------------------
// Update.h
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#ifndef UPDATE_H
#define UPDATE_H

#include "Command.h"

//------------------------------------------------------------------------------
// Update Class inherited from Command-Class
// Representing the "update" command called from calendar.run()
//
class Update : public Command
{
  private:
    //--------------------------------------------------------------------------
    // Copy-Constructor
    // No Copies of Object are possible.
    //
    Update(const Update&);

    //--------------------------------------------------------------------------
    // = Operator-Overloading
    // No Copies of Object are possible.
    //
    Update& operator=(const Update&);

  public:
    //--------------------------------------------------------------------------
    // Constructor
    //
    Update(const std::string& name);

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Update() throw();

    //--------------------------------------------------------------------------
    // Virtual Function from Command-Class executing Command
    // Updates the calendar
    //
    // @param calendar current Calendar Object
    // @param params string vector of parameter
    // @return Returns ERROR or SUCCESS
    //
    int execute(Calendar& calendar, std::vector<std::string>& params);
};

#endif // UPDATE_H
