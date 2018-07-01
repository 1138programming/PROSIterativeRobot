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
    template<class RobotMain> static void _privateRunRobot();
  protected:
    virtual void robotInit();
    virtual void autonInit();
    virtual void autonPeriodic();
    virtual void teleopInit();
    virtual void teleopPeriodic();
    virtual void disabledInit();
    virtual void disabledPeriodic();
  public:
    void doOneCycle();
    RobotBase();
    template<class RobotMain> static void runRobot();
};


#endif // _ROBOTBASE_H_
