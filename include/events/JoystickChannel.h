#ifndef _EVENTS_JOYSTICKCHANNEL_H_
#define _EVENTS_JOYSTICKCHANNEL_H_

#include "main.h"
#include "events/EventListener.h"
#include "commands/Command.h"
#include "events/JoystickButton.h"

enum Channels {
  ChRightY=1,
  ChRightX,
  ChLeftY,
  ChLeftX,
};


class JoystickChannel : public EventListener {
  private:
    char joystickToCheck;
    int channel;
    int threshold = kDefaultThreshold;
    Command* pastThresholdCommand = NULL;
  protected:
    virtual void checkConditions();
  public:
    static const int kDefaultThreshold = 10;
    JoystickChannel(Joysticks joystick, Channels channel);
    void whilePastThreshold(Command* pastThresholdCommand, int threshold = kDefaultThreshold);
};

#endif // _EVENTS_JOYSTICKCHANNEL_H_
