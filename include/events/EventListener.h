#ifndef _EVENTS_EVENTLISTENER_H_
#define _EVENTS_EVENTLISTENER_H_

#include "main.h"

class EventListener {
  private:
  protected:
    virtual void checkConditions();
  public:
  friend class EventScheduler; // Let the Event Scheduler use the checkConditions() method
};

#endif // _EVENTS_EVENTLISTENER_H_
