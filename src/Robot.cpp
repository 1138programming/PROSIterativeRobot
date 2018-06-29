#include "Robot.h"
#include "events/EventScheduler.h"
#include "events/JoystickButton.h"

Robot* Robot::instance = 0;

Robot::Robot() {
  printf("Overridden robot initializer!\n");
  JoystickButton* test = new JoystickButton(MainJoystick, Btn5U);
  //test->whenPressed(new TestCommand);
}

Robot* Robot::getInstance() {
    if (instance == NULL) {
        instance = new Robot();
    }
    return instance;
}
