#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
    cycleCounter = 0;
    enable = 0;
    direction = 0;
    startPosition = 50.0f;
    simReset = 1;
    doorPosition = startPosition;
    motorSpeed = 0.0f;
    inverterState = DRV_SWITCH_ON_DISABLED;
    s0 = 0;
    s1 = 0;
    s2 = 0;
    s3 = 0;

    rtInfo.enable = 1;
    cycleTime = 0.0f;
    timingOK = 0;

    plant.initialized = 0;
    plant.actualSpeed = 0.0f;
    plant.state = DRV_SWITCH_ON_DISABLED;
    plant.commandTimer = 0.0f;
    plant.previousCommand = 0;
    plant.acceleration = 800.0f;
    plant.motionGain = 0.01f;
    plant.switchDelay = 0.25f;

    driveSM.step = 0;
    driveSM.command = DRV_CMD_SHUTDOWN;
    driveSM.speed = 0;
    driveSM.ready = 0;

    doorSM.doorState = ST_INIT;
    doorSM.speed = 0;
    doorSM.speedSlow = 200;
    doorSM.speedFast = 800;

    ledSM.timer = 0.0f;
    ledSM.blink = 0;
    ledSM.led1 = 0;
    ledSM.led2 = 0;
    ledSM.led3 = 0;
    ledSM.led4 = 0;
}
