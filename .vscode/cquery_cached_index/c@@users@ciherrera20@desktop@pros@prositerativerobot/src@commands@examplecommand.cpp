#include "commands/ExampleCommand.h"
#include "Robot.h"

ExampleCommand::ExampleCommand() {
  // Add all requirements using requires()
  requires(Robot::exampleSubsystem);

  // Set the command's priority
  this->priority = DefaultCommandPriority;
}

bool ExampleCommand::canRun() {
  return true; // This is the default value anyways, so this method can be removed
}

void ExampleCommand::initialize() {
  // Perform any initialization steps for this command here, not in the constructor
}

void ExampleCommand::execute() {
  // Code that runs when this command is scheduled to run
}

bool ExampleCommand::isFinished() {
  return true; // This is the default value anyways, so this method can be removed
}

void ExampleCommand::end() {
  // Code that runs when isFinished() returns true.
}

void ExampleCommand::interrupted() {
  // Code that runs when this command is interrupted by another one with a higher priority.
}
