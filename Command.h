//------------------------------------------------------------------------------
// Command.h
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#ifndef COMMAND_H
#define COMMAND_H

#include "Calendar.h"
#include <string>
#include <vector>

class Command
{
  private:
    std::string command_name_;

    //--------------------------------------------------------------------------
    // Copy-Constructor
    // No Copies of Object are possible.
    //
    Command(const Command& original);

    //--------------------------------------------------------------------------
    // = Operator-Overloading
    // No Copies of Object are possible.
    //
    Command& operator=(const Command& original);

  public:
    //--------------------------------------------------------------------------
    // Constructor
    //
    Command(std::string name);

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Command() throw();

    //--------------------------------------------------------------------------
    // Getter Methods
    const std::string& getName();

    //--------------------------------------------------------------------------
    // Setter Methods
    void setName(const std::string& name);

    //--------------------------------------------------------------------------
    // Executes the action.
    // @param calendar The calendar on which the action should be performed
    // @param params Possible parameters needed for the execution
    // @return Integer representing the success of the action
    //
    virtual int execute(Calendar& calendar, std::vector<std::string>& params) = 0;
};

#endif //COMMAND_H
