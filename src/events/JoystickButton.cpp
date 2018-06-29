#include "events/JoystickButton.h"
#include "events/EventScheduler.h"

JoystickButton::JoystickButton(Joysticks joystick, Buttons button) {
  this->joystickToCheck = (int)joystick;
  switch (button) {
    case Btn5U:
      this->buttonGroup = 5;
      this->button = JOY_UP;
    case Btn5D:
      this->buttonGroup = 5;
      this->button = JOY_DOWN;
    case Btn6U:
      this->buttonGroup = 6;
      this->button = JOY_UP;
    case Btn6D:
      this->buttonGroup = 6;
      this->button = JOY_DOWN;
    case Btn7U:
      this->buttonGroup = 7;
      this->button = JOY_UP;
    case Btn7D:
      this->buttonGroup = 7;
      this->button = JOY_DOWN;
    case Btn7L:
      this->buttonGroup = 7;
      this->button = JOY_LEFT;
    case Btn7R:
      this->buttonGroup = 7;
      this->button = JOY_RIGHT;
    case Btn8U:
      this->buttonGroup = 8;
      this->button = JOY_UP;
    case Btn8D:
      this->buttonGroup = 8;
      this->button = JOY_DOWN;
    case Btn8L:
      this->buttonGroup = 8;
      this->button = JOY_LEFT;
    case Btn8R:
      this->buttonGroup = 8;
      this->button = JOY_RIGHT;
  }
  EventScheduler::getInstance()->addEventListener(this);
}

void JoystickButton::checkConditions() {
  bool currentButtonState = joystickGetDigital(joystickToCheck, buttonGroup, button);
  if (currentButtonState) {
    if (currentButtonState == lastState) {
      printf("HELD\n");
      if (heldCommand != NULL) {
        EventScheduler::getInstance()->addCommand(heldCommand);
      }
    } else {
      if (pressedCommand != NULL) {
        EventScheduler::getInstance()->addCommand(pressedCommand);
      }
    }
  } else {
    if (currentButtonState == lastState) {
      if (releasedCommand != NULL) {
        EventScheduler::getInstance()->addCommand(releasedCommand);
      }
    } else {
      if (depressedCommand != NULL) {
        EventScheduler::getInstance()->addCommand(depressedCommand);
      }
    }
  }
  lastState = currentButtonState;
}

void JoystickButton::whenPressed(Command* pressedCommand) {
  this->pressedCommand = pressedCommand;
}

void JoystickButton::whileHeld(Command* heldCommand) {
  this->heldCommand = heldCommand;
}

void JoystickButton::whenDepressed(Command* depressedCommand) {
  this->depressedCommand = depressedCommand;
}

void JoystickButton::whileReleased(Command* releasedCommand) {
  this->releasedCommand = releasedCommand;
}
