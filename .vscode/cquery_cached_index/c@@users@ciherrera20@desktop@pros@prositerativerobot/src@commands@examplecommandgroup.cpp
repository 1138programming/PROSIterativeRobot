#include "commands/ExampleCommandGroup.h"
#include "commands/ExampleCommand.h"

ExampleCommandGroup::ExampleCommandGroup() {
  // Add commands to be run sequentially using addSequentialCommand()
  addSequentialCommand(new ExampleCommand());

  // Add commands to be run in parallel using addParallelCommand();
}
