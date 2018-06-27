#ifndef _EVENTS_JOYSTICKBUTTON_H_
#define _EVENTS_JOYSTICKBUTTON_H_

#include "main.h"
#include "events/EventListener.h"
#include "commands/Command.h"

class JoystickButton : public EventListener {
  private:
  protected:
    virtual void checkConditions();
  public:
    enum class Joysticks {
      MainJoystick,
      PartnerJoystick,
    };
    enum class Buttons {
      // Button group 5
      Btn5U,
      Btn5D,

      // Button group 6
      Btn6U,
      Btn6D,

      // Button group 7
      Btn7U,
      Btn7D,
      Btn7L,
      Btn7R,

      // Button group 8
      Btn8U,
      Btn8D,
      Btn8L,
      Btn8R,
    };

    JoystickButton();
    void whenPressed(Command* pressedCommand);
    void whileHeld(Command* heldCommand);
    void whenDepressed(Command* depressedCommand);
    void whileReleased(Command* releasedCommand);
};

#endif // _EVENTS_JOYSTICKBUTTON_H_
