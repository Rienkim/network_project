//------------------------------------------------------------------------------
// Create.h
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#ifndef CREATE_H
#define CREATE_H

#include "Command.h"

//------------------------------------------------------------------------------
// Create Class inherited from Command-Class
// Representing the "create" command called from calendar.run()
//
class Create : public Command
{
  private:
    //--------------------------------------------------------------------------
    // Copy-Constructor
    // No Copies of Object are possible.
    //
    Create(const Create&);

    //--------------------------------------------------------------------------
    // = Operator-Overloading
    // No Copies of Object are possible.
    //
    Create& operator=(const Create&);

  public:
    //--------------------------------------------------------------------------
    // Constructor with Parameter for Command Constructor
    //
    Create(const std::string &name);

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Create() throw();

    //--------------------------------------------------------------------------
    // Print Usage in case of wrong parameters
    //
    void printUsage();

    //--------------------------------------------------------------------------
    // Virtual Function from Command-Class executing Command
    // Creates a new event end pushes it back to the event list
    //
    // @param calendar current Calendar Object
    // @param params string vector of parameter
    // @return Returns ERROR or SUCCESS
    //
    int execute(Calendar& calendar, std::vector<std::string>& params);
};

#endif // CREATE_H
