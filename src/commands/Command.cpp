#include "commands/Command.h"
#include "subsystems/Subsystem.h"
#include "events/EventScheduler.h"

Command::Command() {
  status = CommandStatus::idle;
}

void Command::requires(Subsystem* aSubsystem) {
  printf("adding requirement");
  subsystemRequirements.push_back(aSubsystem);
}

std::vector<Subsystem*>& Command::getRequirements() {
  return this->subsystemRequirements;
}

bool Command::canRun() {
  return true;
}

void Command::initialize() {

}

void Command::execute() {

}

bool Command::isFinished() {
  return true;
}

void Command::end() {

}

void Command::interrupted() {

}

bool Command::canBeInterruptedBy(Command* aCommand) {
  return aCommand->priority > this->priority;
}

void Command::run() {
  EventScheduler::getInstance()->addCommand(this);
}
