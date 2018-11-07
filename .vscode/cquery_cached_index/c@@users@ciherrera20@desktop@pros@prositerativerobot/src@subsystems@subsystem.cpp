#include "subsystems/Subsystem.h"
#include "events/EventScheduler.h"
#include "commands/Command.h"

size_t Subsystem::instances = 0;

Subsystem::Subsystem() {
  EventScheduler::getInstance()->trackSubsystem(this);
  instances++;
}

void Subsystem::setDefaultCommand(Command *aCommand) {
  aCommand->priority = Command::DefaultCommandPriority; // Give the default command the lowest possible priority
  this->defaultCommand = aCommand;
  EventScheduler::getInstance()->addCommand(aCommand);
}

void Subsystem::initDefaultCommand() {
  // Setup up a default command here
}

Command* Subsystem::getDefaultCommand() {
  return this->defaultCommand;
}
