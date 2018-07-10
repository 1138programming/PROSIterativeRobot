#include "events/EventScheduler.h"
#include <algorithm>

EventScheduler* EventScheduler::instance = NULL;

namespace std {
    // Fix odd linker errors - please fix
    void __throw_bad_alloc() {while (1);}
    void __throw_length_error(char const* ignore) {while (1);}
}

EventScheduler::EventScheduler() {
}

void EventScheduler::update() {
  for (EventListener* listener : eventListeners) {
    listener->checkConditions();
  }

  for (Subsystem* subsystem : subsystems) {
    subsystem->initDefaultCommand();
  }
  subsystems.clear();

  std::vector<Command*> commandsToAdd;
  Command* command;
  for (size_t i = 0; i < commandQueue.size(); i++) { // Should increment to zero if needed
    command = commandQueue[i];
    if (!command->canRun()) {
      command->initialized = false;
      commandQueue.erase(commandQueue.begin() + i);
      i--;
      continue;
    }
    if (!command->initialized) {
      // First, we need to make sure that none of the required subsystems are in
      // use, and if they are, see if we have a larger priority (e.g. can take
      // over that subsystem anyways)
      std::vector<Subsystem*> commandRequirements = command->getRequirements();
      for (size_t j = 0; j < commandRequirements.size(); j++) {
        if (commandRequirements[j]->currentCommand != NULL) {
          if (!commandRequirements[j]->currentCommand->canBeInterruptedBy(command)) {
            // Re-null all of the commands, send something to stdout, and
            // then remove this from the command queue.
            for (size_t k = 0; k <= j; k++) {
              commandRequirements[k]->currentCommand = NULL;
              printf("Warning: Command tried using a subsystem that was already in use! Skipping...\n");
            }
            commandQueue.erase(commandQueue.begin() + i);
            i--;
            goto skipCommand; // This is a valid use for a "goto"
          } else {
            commandRequirements[j]->currentCommand->interrupted(); // Hopefully this won't take long
          }
        }
        commandRequirements[j]->currentCommand = command;
      }
      command->initialize();
      command->initialized = true;
    }
    command->execute();
    if (command->isFinished()) {
      command->end();
      command->initialized = false;
      commandQueue.erase(commandQueue.begin() + i);
      i--;
      // Now we need to free all of the subsystem's requirements and
      // if needed, create the default command again
      std::vector<Subsystem*> commandRequirements = command->getRequirements();
      for (size_t j = 0; j < commandRequirements.size(); j++) {
        commandRequirements[j]->currentCommand = NULL;
        if (commandRequirements[j]->getDefaultCommand() != NULL) {
          commandsToAdd.push_back(commandRequirements[j]->getDefaultCommand());
        }
      }
    }
    skipCommand:; // Exists as a label due to nexted for loops
  }
  commandQueue.insert(commandQueue.end(), commandsToAdd.begin(), commandsToAdd.end());
  delay(5);
}

void EventScheduler::addCommand(Command* commandToRun) {
  this->commandQueue.push_back(commandToRun);
}

void EventScheduler::addEventListener(EventListener* eventListener) {
  this->eventListeners.push_back(eventListener);
}

void EventScheduler::trackSubsystem(Subsystem *aSubsystem) {
  this->subsystems.push_back(aSubsystem);
}

bool EventScheduler::commandInQueue(Command* aCommand) {
  return std::find(commandQueue.begin(), commandQueue.end(), aCommand) != commandQueue.end();
}

EventScheduler* EventScheduler::getInstance() {
    if (instance == NULL) {
        instance = new EventScheduler();
    }
    return instance;
}
