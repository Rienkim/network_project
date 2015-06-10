/*
 * Signin.h
 *
 *  Created on: 2015. 6. 3.
 *      Author: suck
 */

#ifndef SIGNIN_H_
#define SIGNIN_H_

#include "Command.h"

//------------------------------------------------------------------------------
// Signin Class inherited from Command-Class
// Representing the "Signin" command called from calendar.run()
//
class Signin : public Command
{
  private:
    //--------------------------------------------------------------------------
    // Copy-Constructor
    // No Copies of Object are possible.
    //
    Signin(const Signin&);

    //--------------------------------------------------------------------------
    // = Operator-Overloading
    // No Copies of Object are possible.
    //
    Signin& operator=(const Signin&);

  public:
    //--------------------------------------------------------------------------
    // Constructor
    //
    Signin(const std::string& name);

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Signin() throw();

    //--------------------------------------------------------------------------
    // Prints the usage of command
    //
    void printUsage();

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

#endif /* SIGNIN_H_ */
