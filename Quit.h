//------------------------------------------------------------------------------
// Quit.h
//
// CSED353
// TODO: Add your names here.
// Paul Schreiner
//
//------------------------------------------------------------------------------
//

#ifndef QUIT_H
#define QUIT_H

#include "Command.h"

//------------------------------------------------------------------------------
// Quit Class inherited from Command-Class
// Representing the "quit" command called from calendar.run()
//
class Quit : public Command
{
  private:
    //--------------------------------------------------------------------------
    // Bool attribute for quitting endless loop in calendar.run()
    //
    bool loop_;

    //--------------------------------------------------------------------------
    // Copy-Constructor
    // No Copies of Object are possible.
    //
    Quit(const Quit&);

    //--------------------------------------------------------------------------
    // = Operator-Overloading
    // No Copies of Object are possible.
    //
    Quit& operator=(const Quit&);

  public:
    //--------------------------------------------------------------------------
    // Constructor
    //
    Quit(const std::string& name, bool init);

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Quit() throw();

    //--------------------------------------------------------------------------
    // Getter
    //
    bool getLoop() const;

    //--------------------------------------------------------------------------
    // Setter
    //
    void setLoop(bool new_value);

    //--------------------------------------------------------------------------
    // Virtual Function from Command-Class executing Command
    // Quits the endless loop by setting it's attribute to false
    //
    // @param calendar current Calendar Object
    // @param params string vector of parameter
    // @return Returns ERROR or SUCCESS
    //
    int execute(Calendar& calendar, std::vector<std::string>& params);
};

#endif // QUIT_H
