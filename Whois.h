/*
 * Whois.h
 *
 *  Created on: 2015. 6. 2.
 *      Author: JOHNSON
 */

#ifndef WHOIS_H_
#define WHOIS_H_

#include "Command.h"

//------------------------------------------------------------------------------
// Whois Class inherited from Command-Class
// Representing the "whois" command called from calendar.run()
//
class Whois : public Command
{
  private:
    //--------------------------------------------------------------------------
    // Copy-Constructor
    // No Copies of Object are possible.
    //
    Whois(const Whois&);

    //--------------------------------------------------------------------------
    // = Operator-Overloading
    // No Copies of Object are possible.
    //
    Whois& operator=(const Whois&);

  public:
    //--------------------------------------------------------------------------
    // Constructor
    //
    Whois(const std::string& name);

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Whois() throw();

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

#endif /* WHOIS_H_ */
