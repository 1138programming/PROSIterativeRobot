#ifndef _EVENTS_EVENTSCHEDULER_H_
#define _EVENTS_EVENTSCHEDULER_H_

#include "commands/Command.h"
#include "commands/CommandGroup.h"
#include "main.h"
#include "events/EventListener.h"
#include "subsystems/Subsystem.h"
#include <vector>

class EventScheduler {
  private:
    size_t numSubsystems = 0;
    static EventScheduler* instance;
    EventScheduler();
    std::vector<Subsystem*> subsystems;
    std::vector<EventListener*> eventListeners;
    std::vector<Command*> commandQueue;
    std::vector<CommandGroup*> commandGroupQueue;
  public:
    const static int millisecondTimeout = 25; // Maximum time to run event listeners
    static EventScheduler* getInstance(); // Return the singleton instance of me
    void update();
    void addEventListener(EventListener* eventListener);
    void addCommand(Command* commandToRun);
    void addCommandGroup(CommandGroup* commandGroupToRun);
    void removeCommand(Command* commandToRemove);
    void removeCommandGroup(CommandGroup* commandGroupToRemove);
    void clearCommandQueue();
    void clearCommandGroupQueue();
    void trackSubsystem(Subsystem* aSubsystem);
    bool commandInQueue(Command* aCommand);
    bool commandGroupInQueue(CommandGroup* aCommandGroup);
};

#endif // _EVENTS_EVENTSCHEDULER_H_
