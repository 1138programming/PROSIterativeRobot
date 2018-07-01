#include "subsystems/Subsystem.h"
#include "events/EventScheduler.h"
#include "commands/Command.h"

Subsystem::Subsystem() {
}

void Subsystem::setDefaultCommand(Command *aCommand) {
  aCommand->priority = 0; // Give it the lowest possible priority
  this->defaultCommand = aCommand;
  EventScheduler::getInstance()->addCommand(aCommand);
}

Command* Subsystem::getDefaultCommand() {
  return this->defaultCommand;
}
