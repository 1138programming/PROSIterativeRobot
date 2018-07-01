#ifndef _SUBSYSTEMS_SUBSYSTEM_H_
#define _SUBSYSTEMS_SUBSYSTEM_H_

#include "main.h"

class Command;

class Subsystem {
  private:
    Command* defaultCommand = NULL;
    void setDefaultCommand(Command* aCommand);
  public:
    Command* getDefaultCommand();
    Command* currentCommand = NULL;
    Subsystem();
};

#endif // _SUBSYSTEMS_SUBSYSTEM_H_
