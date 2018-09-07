#include "events/JoystickChannel.h"
#include "events/EventScheduler.h"

JoystickChannel::JoystickChannel(Joysticks joystick, Channels channel) {
  this->joystickToCheck = (int)joystick;
  this->channel = channel;
  EventScheduler::getInstance()->addEventListener(this);
}

void JoystickChannel::checkConditions() {
  if (pastThresholdCommand == NULL)
    return;

  int currentChannelState = joystickGetAnalog(joystickToCheck, channel);
  if (currentChannelState > threshold || currentChannelState < -threshold) {
    //EventScheduler::getInstance()->addCommand(pastThresholdCommand);
    pastThresholdCommand->addSelf();
  }
  //printf("Joystick channel current value is %d and the threshold is %d\n", currentChannelState, threshold);
  //printf("Checking joystick channel conditions\n");
}

void JoystickChannel::whilePastThreshold(Command* pastThresholdCommand, int threshold) {
  this->pastThresholdCommand = pastThresholdCommand;
  this->threshold = threshold;
}
