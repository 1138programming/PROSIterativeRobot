#include "commands/Command.h"
#include "subsystems/Subsystem.h"

Command::Command() {

}

void Command::requires(Subsystem* aSubsystem) {
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

void Command::run() {

}

bool Command::isFinished() {
  return true;
}

void Command::end() {

}

void Command::interrupted() {

}
