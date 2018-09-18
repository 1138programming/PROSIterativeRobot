#include "commands/CommandGroup.h"
#include "events/EventScheduler.h"
#include <algorithm>

CommandGroup::CommandGroup() {
  status = Idle;
}

std::vector<Subsystem*>& CommandGroup::getRequirements() {
  return requirements[sequentialIndex];
}

bool CommandGroup::canRun() {
  printf("Checking if the command group can run\n");
  //delay(1000);
  for (size_t i = 0; i < commands[sequentialIndex].size(); i++) {
    if (!commands[sequentialIndex][i]->canRun())
      return false;
  }
  return true;
}

void CommandGroup::initialize() {
  printf("Command group is initialized\n");
  //delay(1000);
  //printf("Total added size is %d and sequential added size is %d\n", added.size(), added[sequentialIndex].size());
  /*this->sequentialIndex = 0;
  std::vector<Command*> lastCommandList = this->commands[sequentialIndex];
  for (Command* aCommand : lastCommandList) {
    EventScheduler::getInstance()->addCommand(aCommand);
  }*/
  sequentialIndex = 0;

  for (size_t i = 0; i < commands.size(); i++) {
    for (size_t j = 0; j < commands.size(); j++) {
      added[i][j] = 0;
    }
  }
}

void CommandGroup::execute() {
  //printf("Command group is running. Sequential step size is %d\n", commands[sequentialIndex].size());
  //delay(1000);
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
    //printf("Index is %d\n", i);
    //delay(1000);
    if (!added[sequentialIndex][i]) {
      command->run();
      added[sequentialIndex][i] = 1;
      sequentialFinished = false;
    } else {
      if (command->status != Finished) {
        sequentialFinished = false;
      }

      //printf("Index is %d, status is %d\n", i, command->status);
      //delay(100);
      if (command->status == Interrupted || (command->status != Running && command->status != Finished)) {
        //printf("Command in commandGroup was interrupted\n");
        //delay(1000);
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
  printf("Command group is finished\n");
  sequentialIndex = 0;
}

void CommandGroup::interrupted() {
  printf("Command group interrupted\n");
  //delay(1000);
  status = Idle;
  for (size_t i = 0; i < commands[sequentialIndex].size(); i++) {
    commands[sequentialIndex][i]->stop();
  }
  sequentialIndex = 0;
}

void CommandGroup::addSequentialCommand(Command* aCommand) {
  printf("Adding sequential command\n");
  //delay(1000);
  std::vector<Command*> commandList;
  std::vector<Subsystem*> requirementList;
  std::vector<int> addedList;

  commandList.push_back(aCommand);
  requirementList.insert(requirementList.end(), aCommand->getRequirements().begin(), aCommand->getRequirements().end());
  addedList.push_back(0);

  this->commands.push_back(commandList);
  this->requirements.push_back(requirementList);
  this->added.push_back(addedList);
}

void CommandGroup::addParallelCommand(Command *aCommand) {
  //std::vector<Command*> lastCommandList = this->commands.back();
  //std::vector<Subsystem*> lastRequirementList = this->requirements.back();
  //std::vector<int> lastAddedList = this->added.back();

  this->commands.back().push_back(aCommand);
  this->requirements.back().insert(this->requirements.back().end(), aCommand->getRequirements().begin(), aCommand->getRequirements().end());
  this->added.back().push_back(0);
  printf("Added parallel command. Size of last vector is %d\n", commands.back().size());
  //delay(1000);
}


void CommandGroup::run() {
  printf("Adding command group\n");
  //delay(1000);
  EventScheduler::getInstance()->addCommandGroup(this);
}

void CommandGroup::stop() {
  EventScheduler::getInstance()->removeCommandGroup(this);
}

void CommandGroup::printSomething() {
  printf("I am a command group!\n");
  delay(1000);
}
