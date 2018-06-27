#ifndef _ROBOTBASE_H_
#define _ROBOTBASE_H_

#include "main.h"

class RobotBase {
  private:
    enum class RobotState {
      None,
      Auton,
      Teleop,
      Disabled,
    };
    RobotState lastState = RobotState::None;
  public:
    virtual void robotInit();
    virtual void autonInit();
    virtual void autonPeriodic();
    virtual void teleopInit();
    virtual void teleopPeriodic();
    virtual void disabledInit();
    virtual void disabledPeriodic();
    void doOneCycle();
    RobotBase();
};


#endif // _ROBOTBASE_H_
