#include "Robot.h"
#include "events/EventScheduler.h"
#include "events/JoystickButton.h"

#include "commands/ExampleCommand.h"
#include "commands/ExampleCommandGroup.h"

Robot* Robot::instance = 0;
ExampleSubsystem* Robot::exampleSubsystem = 0;

Robot::Robot() {
  printf("Overridden robot constructor!\n");
  // Initialize any subsystems
  exampleSubsystem = new ExampleSubsystem();

  // Initialize any actions/commands needed
  JoystickButton* test = new JoystickButton(MainJoystick, Btn5U);
  test->whenPressed(new ExampleCommand());

  //auto test2 = new ExampleCommandGroup();
  //test2->run();
}

void Robot::robotInit() {
  printf("Robot created.\n");
}

void Robot::autonInit() {
  //printf("Default autonInit() function\n");
}

void Robot::autonPeriodic() {
  //printf("Default autonPeriodic() function\n");
  EventScheduler::getInstance()->update();
}

void Robot::teleopInit() {
  //printf("Default teleopInit() function\n");
}

void Robot::teleopPeriodic() {
  //printf("Default teleopPeriodic() function\n");
    EventScheduler::getInstance()->update();
}

void Robot::disabledInit() {
  //printf("Default disabledInit() function\n");
}

void Robot::disabledPeriodic() {
  //printf("Default disabledPeriodic() function");
}

Robot* Robot::getInstance() {
    if (instance == NULL) {
        instance = new Robot();
    }
    return instance;
}
