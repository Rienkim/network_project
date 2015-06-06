/*
 * sync.h
 *
 *  Created on: 2015. 6. 6.
 *      Author: suck
 */

#ifndef SYNC_H_
#define SYNC_H_


#include "Command.h"

//Headers for socket comm
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

//------------------------------------------------------------------------------
// Sync Class inherited from Command-Class
// Representing the "Sync" command called from calendar.run()
//
class Sync : public Command
{
  private:
    //--------------------------------------------------------------------------
    // Copy-Constructor
    // No Copies of Object are possible.
    //
    Sync(const Sync&);

    //--------------------------------------------------------------------------
    // = Operator-Overloading
    // No Copies of Object are possible.
    //
    Sync& operator=(const Sync&);

  public:
    //--------------------------------------------------------------------------
    // Constructor
    //
    Sync(const std::string& name);

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Sync() throw();

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


#endif /* SYNC_H_ */
