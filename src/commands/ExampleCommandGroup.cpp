#include "commands/ExampleCommandGroup.h"
#include "commands/ExampleCommand.h"
#include "commands/ExampleWaitCommand.h"

ExampleCommandGroup::ExampleCommandGroup() {
  addSequentialCommand(new ExampleWaitCommand());
  addSequentialCommand(new ExampleCommand());
}
