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
  ////printf("Event scheduler update...\n");
  //delay(1000);
  for (EventListener* listener : eventListeners) {
    listener->checkConditions();
  }

  for (Subsystem* subsystem : subsystems) {
    subsystem->initDefaultCommand();
    this->numSubsystems++;
  }
  subsystems.clear();

  ////printf("Default commands initialized\n");
  //delay(1000);

  std::vector<Subsystem*> usedSubsystems;
  CommandGroup* commandGroup;
  bool canRun;

  ////printf("Looping through commandGroupQueue. CommandGroupQueue size is %d\n", commandGroupQueue.size());
  //delay(1000);
  ////printf("CommandGroupQueue size is %d\n", commandGroupQueue.size());

  if (commandGroupQueue.size() != 0) {
    for (int i = commandGroupQueue.size() - 1; i >= 0; i--) {
      commandGroup = commandGroupQueue[i];
      ////printf("Index is %d\n", i);
      //delay(1000);

      //printf("Command group status is %d, interrupted is %d\n", commandGroup->status, Interrupted);
      //delay(1000);
      if (commandGroup->status == Interrupted) {
        //printf("Command group was interrupted\n");
        //delay(1000);
        commandGroup->status = Idle;
        commandGroup->interrupted();
        commandGroupQueue.erase(commandGroupQueue.begin() + i);
        continue;
      }

      //printf("Index is still %d\n", i);
      //delay(1000);
      canRun = commandGroup->canRun();
      //printf("Command group can run is %d\n", canRun);
      //delay(1000);
      std::vector<Subsystem*>& commandGroupRequirements = commandGroup->getRequirements();

      if (canRun) {
        //printf("Looping through command group requirements\n");
        //delay(1000);
        /*for (size_t j = 0; j < commandGroupRequirements.size(); j++) {
          ////printf("Second index is %d, commandGroupRequirements size is %d and usedSubsystems size is %d\n", j, commandGroupRequirements.size(), usedSubsystems.size());
          //delay(1000);
          if (std::find(commandGroupQueue.begin(), commandGroupQueue.end(), commandGroup) != commandGroupQueue.end()) {
            ////printf("Command group cannot run\n");
            //delay(1000);
            canRun = false;
            break;
          }
        }*/
        for (Subsystem* aSubsystem : commandGroupRequirements) {
          if (std::find(usedSubsystems.begin(), usedSubsystems.end(), aSubsystem) != usedSubsystems.end()) {
            //printf("Command group cannot run\n");
            canRun = false;
            break;
          }
        }
      }

      if (canRun) {
        //printf("Command group can run\n");
        //delay(1000);
        usedSubsystems.insert(usedSubsystems.end(), commandGroupRequirements.begin(), commandGroupRequirements.end());
        //printf("Added command requirments to used subsystems\n");
        //delay(1000);

        if (commandGroup->status != Running) {
          commandGroup->initialize();
          commandGroup->status = Running;
        }
        commandGroup->execute();

        if (commandGroup->isFinished()) {
          commandGroup->end();
          commandGroup->status = Finished;
          commandGroupQueue.erase(commandGroupQueue.begin() + i);
        }
      } else {
        if (commandGroup->status == Running) {
          commandGroup->interrupted();
          commandGroup->status = Interrupted;
        }
        commandGroupQueue.erase(commandGroupQueue.begin() + i);
      }
    }
  }

  usedSubsystems.clear();
  Command* command; // Holds the command we're currently checking for run-ability

  ////printf("CommandQueue size is %u\n", commandQueue.size());
  if (commandQueue.size() != 0) {
    for (int i = commandQueue.size() - 1; i >= 0; i--) {
      command = commandQueue[i];

      canRun = command->canRun();

      // First, check requirements, and if we can't run because of them,
      // then pop us off the commandQueue and pretend we don't exist
      std::vector<Subsystem*>& commandRequirements = command->getRequirements();

      if (usedSubsystems.size() == this->numSubsystems || !canRun) {
        // Shortcut to not iterate through the usedSubsystems vector each time
        canRun = false;
      } else {
        for (Subsystem* aSubsystem : commandRequirements) {
          if (std::find(usedSubsystems.begin(), usedSubsystems.end(), aSubsystem) != usedSubsystems.end()) {
            // If the subsystem that we want to use is already in usedSubsystems
            // (Quick sidenote: C++'s way of checking for object existence in
            // an array seems really stupid, but it's surprisingly useful!)
            // then we need to pop it off the queue, since we can't take control
            //
            // Remember: We're already going in order of priority, so we can't
            // take control anyways
            canRun = false;
            break;
          }
        }
      }
      if (canRun) {
        // Keep track of the subsystems we've already used
        usedSubsystems.insert(usedSubsystems.end(), commandRequirements.begin(), commandRequirements.end());

        /*if (!command->`initialized`) {
          command->initialize();
          command->initialized = true;
        }*/
        if (command->status != Running) {
          command->initialize();
          command->status = Running;
        }

        command->execute();

        if (command->isFinished()) {
          command->end();
          //command->initialized = false;
          command->status = Finished;
          if (command->priority > 0) {
            // Not a default command, we can pop it off the commandQueue
            commandQueue.erase(commandQueue.begin() + i);
          }
        }
      } else {
        /*if (command->initialized) {
          command->interrupted();
          command->initialized = false;
        }*/
        if (command->status == Running) {
          command->interrupted();
          command->status = Interrupted;
          //printf("Command interrupted, status is %d, queue size is %d\n", command->status, commandQueue.size());
        }
        if (command->priority > 0) {
          // We're not a default command (defined by having a priority of 0),
          // so there's no danger in discarding us
          commandQueue.erase(commandQueue.begin() + i);
        }
        //continue;
      }

      // We've proven that we can run, and since we're going in order of descending
      // priority, we don't need to worry about other commands using our requirements.
      // Therefore, we can set up, execute, and finish the command like normal
    }
  }

  ////printf("Done!\n");
  //delay(1000);
  delay(5);
}

void EventScheduler::addCommand(Command* command) {
  if (!commandInQueue(command)) {
    if (commandQueue.size() == 0) {
      commandQueue.push_back(command);
      return;
    }

    // 0, 5, 10
    // trying to insert 7
    //

    for (size_t i = 0; i < commandQueue.size(); i++) {
      if (command->priority < commandQueue[i]->priority) {
        commandQueue.insert(commandQueue.begin() + i, command);
        return;
      }
    }
    commandQueue.push_back(command);
  }
}

void EventScheduler::addCommandGroup(CommandGroup* commandGroupToRun) {
  /*if (commandGroupInQueue(commandGroupToRun)) {
    commandGroupToRun->initialize();
  } else {
    commandGroupQueue.push_back(commandGroupToRun);
  }*/
  //printf("Attempting to add command group\n");
  //delay(1000);
  if (!commandGroupInQueue(commandGroupToRun)) {
    //printf("Command group queue size is %d\n", commandGroupQueue.size());
    //delay(1000);
    commandGroupQueue.push_back(commandGroupToRun);
    //printf("Command group queue size is %d\n", commandGroupQueue.size());
    //delay(1000);
  }
}

void EventScheduler::removeCommand(Command* commandToRemove) {
  ////printf("Removing command...");
  size_t index = std::find(commandQueue.begin(), commandQueue.end(), commandToRemove) - commandQueue.begin();
  if (index >= commandQueue.size()) {
    ////printf("Done!\n");
    return;
  }
  commandQueue.erase(commandQueue.begin() + index);
  ////printf("Done!\n");
}

void EventScheduler::removeCommandGroup(CommandGroup* commandGroupToRemove) {
  size_t index = std::find(commandGroupQueue.begin(), commandGroupQueue.end(), commandGroupToRemove) - commandGroupQueue.begin();
  if (index >= commandGroupQueue.size())
    return;
  commandGroupQueue.erase(commandGroupQueue.begin() + index);
}

void EventScheduler::clearCommandQueue() {
  commandQueue.clear();
}

void EventScheduler::clearCommandGroupQueue() {
  commandGroupQueue.clear();
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

bool EventScheduler::commandGroupInQueue(CommandGroup* aCommandGroup) {
  //printf("Checking if command group is in the queue\n");
  //delay(1000);
  return std::find(commandGroupQueue.begin(), commandGroupQueue.end(), aCommandGroup) != commandGroupQueue.end();
}

EventScheduler* EventScheduler::getInstance() {
    if (instance == NULL) {
        instance = new EventScheduler();
    }
    return instance;
}
