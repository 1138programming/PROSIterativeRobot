#include "commands/CommandGroup.h"
#include "events/EventScheduler.h"
#include <algorithm>

CommandGroup::CommandGroup() {

}

void CommandGroup::initialize() {
  this->commandIndex = 0;
  std::vector<Command*> lastCommandList = this->commands[commandIndex];
  for (Command* aCommand : lastCommandList) {
    EventScheduler::getInstance()->addCommand(aCommand);
  }
}

void CommandGroup::execute() {
  // Somewhat idle-ish loop to just check if we need to move to the next set of commands
  if (commandIndex == this->commands.size()) return;
  std::vector<Command*> lastCommandList = this->commands[commandIndex];
  if (std::all_of(lastCommandList.begin(),
                  lastCommandList.end(),
                  [](Command* aCommand){
                    return !EventScheduler::getInstance()->commandInQueue(aCommand);
                  })) {
    this->commandIndex++;
    if (commandIndex == this->commands.size()) return;
    lastCommandList = this->commands[commandIndex];
    for (Command* aCommand : lastCommandList) {
      EventScheduler::getInstance()->addCommand(aCommand);
    }
  }
}

bool CommandGroup::isFinished() {
  if (this->commandIndex == this->commands.size()) {
    std::vector<Command*> lastCommandList = this->commands.back();
    return std::all_of(lastCommandList.begin(),
                       lastCommandList.end(),
                       [](Command* aCommand){
                         return !EventScheduler::getInstance()->commandInQueue(aCommand);
                       });
  }
  return false;
}

void CommandGroup::addSequentialCommand(Command* aCommand) {
  std::vector<Command*> commandList;
  commandList.push_back(aCommand);

  this->commands.push_back(commandList);
}

void CommandGroup::addParallelCommand(Command *aCommand) {
  std::vector<Command*> lastCommandList = this->commands.back();
  lastCommandList.push_back(aCommand);
}
