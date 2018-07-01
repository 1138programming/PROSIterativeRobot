#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "main.h"
#include "RobotBase.h"

class Robot : public RobotBase {
  private:
    static Robot* instance;
  protected:
    // Let's *not* let anybody randomly call something like robotInit() during
    // a match or anywhere unintended, like in a command
    void robotInit();
    void autonInit();
    void autonPeriodic();
    void teleopInit();
    void teleopPeriodic();
    void disabledInit();
    void disabledPeriodic();
  public:
    Robot();
    static Robot* getInstance();
};


#endif // _ROBOT_H_
