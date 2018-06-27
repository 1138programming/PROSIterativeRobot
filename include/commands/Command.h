#ifndef _COMMANDS_COMMAND_H_
#define _COMMANDS_COMMAND_H_

#include "main.h"

class Command {
  private:
  public:
    int priority = 50; // Commands can only be interrupted by commands with a higher priority

    virtual void canRun(); // Whether or not the command can run right now. If false, it is ignored
    virtual void initialize(); // Set up the command for running
    virtual void run(); // Run the command
    virtual void isFinished(); // Whether or not the command is finished. The run() command is run continuously until thie istrue
    virtual void end(); // Run when command is finished
    virtual void interrupted(); // Run when command was interrupted by one with a higher priority
    Command();
};

#endif // _COMMANDS_COMMAND_H_
