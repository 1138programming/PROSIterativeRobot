#include "events/JoystickChannel.h"
#include "events/EventScheduler.h"

JoystickChannel::JoystickChannel(Joysticks joystick, Channels channel) {
  this->joystickToCheck = (int)joystick;
  this->channel = channel;
  EventScheduler::getInstance()->addEventListener(this);
}

void JoystickChannel::checkConditions() {
  int currentChannelState = joystickGetAnalog(joystickToCheck, channel);
  if (currentChannelState > threshold || currentChannelState < -threshold) {
    EventScheduler::getInstance()->addCommand(pastThresholdCommand);
  }
}

void JoystickChannel::whilePastThreshold(Command* changeCommand, int threshold) {
  this->pastThresholdCommand = pastThresholdCommand;
  this->threshold = threshold;
}
