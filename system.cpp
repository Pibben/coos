//
// Created by per on 2017-10-02.
//

#include "system.h"

Eventloop System::mEventloop;
Uart System::mUart;
ArmTimer System::mArmTimer;
SystemTimer System::mSystemTimer1(1);
SystemTimer System::mSystemTimer3(3);
LocalTimer System::mLocalTimer;
