#include "commands/ExampleWaitCommand.h"
#include "Robot.h"

ExampleWaitCommand::ExampleWaitCommand() {
  requires(Robot::exampleSubsystem);
}

bool ExampleWaitCommand::canRun() {
  return true; // This is the default value anyways, so this method can be removed
}

void ExampleWaitCommand::initialize() {
  // Perform any initialization steps for this command here, not in the
  // constructor
  startTime = millis();
}

void ExampleWaitCommand::execute() {
  // Code that runs when this command is scheduled to run

  printf("Example wait command is running.\n");
}

bool ExampleWaitCommand::isFinished() {
  return (startTime + 5000) < millis(); // This is the default value anyways, so this method can be removed
}

void ExampleWaitCommand::end() {
  // Code that runs when isFinished() returns true.
}

void ExampleWaitCommand::interrupted() {
  // Code that runs when this command is interrupted by another one
  // with a higher priority.
}
