#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

void _CYCLIC ProgramCyclic(void)
{
    cycleCounter++;

    rtInfo.enable = 1;
    RTInfo(&rtInfo);
    timingOK = (rtInfo.status == 0) && (rtInfo.cycle_time > 0);
    cycleTime = timingOK ? (float)rtInfo.cycle_time * 0.000001f : 0.0f;

    if (simReset)
    {
        enable = 0;
        doorSM.doorState = ST_INIT;
        ledSM.timer = 0.0f;
        ledSM.blink = 0;
    }

    plant.reset = simReset;
    plant.initialPosition = startPosition;
    plant.command = driveSM.command;
    plant.speedSet = driveSM.speed;
    plant.cycleTime = cycleTime;
    SimDoorPlant(&plant);
    simReset = 0;

    inverterState = plant.state;
    doorPosition = plant.position;
    motorSpeed = plant.actualSpeed;
    s0 = plant.s0;
    s1 = plant.s1;
    s2 = plant.s2;
    s3 = plant.s3;

    doorSM.enable = enable && timingOK && plant.valid;
    doorSM.state = inverterState;
    doorSM.direction = direction;
    doorSM.s0 = s0;
    doorSM.s1 = s1;
    doorSM.s2 = s2;
    doorSM.s3 = s3;
    DoorStateMashine(&doorSM);

    driveSM.enable = enable && timingOK && plant.valid;
    driveSM.state = inverterState;
    driveSM.speedSet = doorSM.speed;
    DriveStateMashine(&driveSM);

    ledSM.state = doorSM.doorState;
    ledSM.cycleTime = cycleTime;
    LedStateMashine(&ledSM);
}
