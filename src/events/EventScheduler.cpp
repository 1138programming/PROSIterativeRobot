#include "events/EventScheduler.h"

EventScheduler* EventScheduler::instance = NULL;

EventScheduler::EventScheduler() {
}

EventScheduler* EventScheduler::getInstance() {
    if (instance == NULL) {
        instance = new EventScheduler();
    }
    return instance;
}
