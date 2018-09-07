#include "commands/CommandGroup.h"
#include "events/EventScheduler.h"
#include <algorithm>

CommandGroup::CommandGroup() {
  status = Idle;
}

std::vector<Subsystem*>& CommandGroup::getRequirements() {
  return requirements[sequentialIndex];
}

void CommandGroup::addSelf() {
  EventScheduler::getInstance()->addCommandGroup(this);
}

void CommandGroup::removeSelf() {
  EventScheduler::getInstance()->removeCommandGroup(this);
}

bool CommandGroup::canRun() {
  for (size_t i = 0; i < commands[sequentialIndex].size(); i++) {
    if (!commands[sequentialIndex][i]->canRun())
      return false;
  }
  return true;
}

void CommandGroup::initialize() {
  /*this->sequentialIndex = 0;
  std::vector<Command*> lastCommandList = this->commands[sequentialIndex];
  for (Command* aCommand : lastCommandList) {
    EventScheduler::getInstance()->addCommand(aCommand);
  }*/
  sequentialIndex = 0;

  for (size_t i = 0; i < commands.size(); i++) {
    for (size_t j = 0; j < commands.size(); j++) {
      added[i][j] = false;
    }
  }
}

void CommandGroup::execute() {
  // Somewhat idle-ish loop to just check if we need to move to the next set of commands
  /*if (sequentialIndex == this->commands.size()) return;
  std::vector<Command*> lastCommandList = this->commands[sequentialIndex];
  if (std::all_of(lastCommandList.begin(),
                  lastCommandList.end(),
                  [](Command* aCommand){
                    return !EventScheduler::getInstance()->commandInQueue(aCommand);
                  })) {
    this->sequentialIndex++;
    if (sequentialIndex == this->commands.size()) return;
    lastCommandList = this->commands[sequentialIndex];
    for (Command* aCommand : lastCommandList) {
      EventScheduler::getInstance()->addCommand(aCommand);
    }
  }*/
  bool sequentialFinished = true;
  bool sequentialInterrupted = false;
  Command* command;

  for (size_t i = 0; i < commands[sequentialIndex].size(); i++) {
    command = commands[sequentialIndex][i];
    if (!added[sequentialIndex][i]) {
      //EventScheduler::getInstance()->addCommand(command);
      command->addSelf();
      added[sequentialIndex][i] = true;
      sequentialFinished = false;
    } else {
      if (command->status != Finished) {
        sequentialFinished = false;
      }

      if (command->status == Interrupted || (command->status != Running && command->status != Finished)) {
        sequentialInterrupted = true;
      }
    }
  }

  if (sequentialInterrupted)
    status = Interrupted;

  if (sequentialFinished)
    sequentialIndex++;
}

bool CommandGroup::isFinished() {
  /*if (this->sequentialIndex == this->commands.size()) {
    std::vector<Command*> lastCommandList = this->commands.back();
    return std::all_of(lastCommandList.begin(),
                       lastCommandList.end(),
                       [](Command* aCommand){
                         return !EventScheduler::getInstance()->commandInQueue(aCommand);
                       });
  }
  return false;*/
  return !(sequentialIndex < commands.size());
}

void CommandGroup::end() {

}

void CommandGroup::interrupted() {
  status = Interrupted;
  for (size_t i = 0; i < commands[sequentialIndex].size(); i++) {
    commands[sequentialIndex][i]->removeSelf();
  }
}

void CommandGroup::addSequentialCommand(Command* aCommand) {
  std::vector<Command*> commandList;
  std::vector<Subsystem*> requirementList;
  std::vector<bool> addedList;

  commandList.push_back(aCommand);
  requirementList.insert(requirementList.end(), aCommand->getRequirements().begin(), aCommand->getRequirements().end());
  addedList.push_back(false);

  this->commands.push_back(commandList);
  this->requirements.push_back(requirementList);
  this->added.push_back(addedList);
}

void CommandGroup::addParallelCommand(Command *aCommand) {
  std::vector<Command*> lastCommandList = this->commands.back();
  std::vector<Subsystem*> lastRequirementList = this->requirements.back();
  std::vector<bool> lastAddedList = this->added.back();

  lastCommandList.push_back(aCommand);
  lastRequirementList.insert(lastRequirementList.end(), aCommand->getRequirements().begin(), aCommand->getRequirements().end());
  lastAddedList.push_back(false);
}
