#ifndef _EVENTS_EVENTSCHEDULER_H_
#define _EVENTS_EVENTSCHEDULER_H_

#include "main.h"
#include "events/EventListener.h"
#include <vector>

class EventScheduler {
  private:
    static EventScheduler* instance;
    EventScheduler();
    std::vector<EventListener*> eventListeners;
  public:
    static EventScheduler* getInstance(); // Return the singleton instance of me
    void update();
    void addEventListener(EventListener* eventListener);
};

#endif // _EVENTS_EVENTSCHEDULER_H_
