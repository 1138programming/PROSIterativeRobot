#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "main.h"
#include "RobotBase.h"

class Robot : public RobotBase {
  private:
    static Robot* instance;
  public:
    Robot();
    static Robot* getInstance();
};


#endif // _ROBOT_H_
