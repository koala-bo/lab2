#include <bur/plctypes.h>
#include "DriveLib.h"

void DriveStateMashine(struct DriveStateMashine* inst)
{
    unsigned short status;

    inst->command = DRV_CMD_SHUTDOWN;
    inst->speed = 0;
    inst->ready = 0;
    inst->step = 0;

    if (!inst->enable)
        return;

    status = inst->state & DRV_STATUS_MASK;

    switch (status)
    {
        case DRV_READY_TO_SWITCH_ON:
            inst->command = DRV_CMD_SWITCH_ON;
            inst->step = 1;
            break;

        case DRV_SWITCHED_ON:
            inst->command = DRV_CMD_ENABLE;
            inst->step = 2;
            break;

        case DRV_OPERATION_ENABLED:
            inst->command = DRV_CMD_ENABLE;
            inst->speed = inst->speedSet;
            inst->ready = 1;
            inst->step = 3;
            break;

        default:
            break;
    }
}
