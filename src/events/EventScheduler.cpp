#include "events/EventScheduler.h"

EventScheduler* EventScheduler::instance = NULL;

EventScheduler::EventScheduler() {
}

void EventScheduler::update() {
  long startTime = millis();
  auto listenLoop = [](void* eventListeners) -> void {
    for (EventListener* listener : *(reinterpret_cast<std::vector<EventListener*>*>(eventListeners))) {
      listener->checkConditions();
    }
  };

  // PROS v2 tasks, since C++'s <thread> module isn't available
  TaskHandle listenerTask = taskCreate(listenLoop, TASK_DEFAULT_STACK_SIZE, &this->eventListeners, TASK_PRIORITY_DEFAULT);
  while ((millis() - startTime) < millisecondTimeout && taskGetState(listenerTask) == TASK_RUNNING) {
    taskDelay(2); // Wait 2 milliseconds
  }
  if (taskGetState(listenerTask) != TASK_RUNNING) {
    taskDelete(listenerTask);
  }

  Command* command;
  for (int i = 0; i < commandQueue.size(); i++) {
    command = commandQueue[i];
    if (!command->canRun()) {
      command->initialized = false;
      commandQueue.erase(commandQueue.begin(), commandQueue.begin() + i);
      i--;
      continue;
    }
    if (!command->initialized) {
      command->initialize();
      command->initialized = true;
    }
    command->run();
    if (command->isFinished()) {
      command->end();
      command->initialized = false;
      commandQueue.erase(commandQueue.begin(), commandQueue.begin() + i);
      i--;
      continue;
    }
  }
}

void EventScheduler::addCommand(Command* commandToRun) {
  // Todo: Command priority
  this->commandQueue.push_back(commandToRun);
}

void EventScheduler::addEventListener(EventListener* eventListener) {
  // Todo: Make some kind of shared pointer
  this->eventListeners.push_back(eventListener);
}

EventScheduler* EventScheduler::getInstance() {
    if (instance == NULL) {
        instance = new EventScheduler();
    }
    return instance;
}
