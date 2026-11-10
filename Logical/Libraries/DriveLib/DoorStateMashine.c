#include <bur/plctypes.h>
#include "DriveLib.h"

void DoorStateMashine(struct DoorStateMashine* inst)
{
    unsigned char activeSensors;

    inst->speed = 0;

    if (!inst->enable ||
        ((inst->state & DRV_STATUS_MASK) != DRV_OPERATION_ENABLED))
    {
        inst->doorState = ST_INIT;
        return;
    }

    if ((inst->speedSlow <= 0) || (inst->speedFast < inst->speedSlow))
    {
        inst->doorState = ST_INIT;
        return;
    }

    activeSensors = (inst->s0 != 0) + (inst->s1 != 0) +
                    (inst->s2 != 0) + (inst->s3 != 0);
    if (activeSensors > 1)
    {
        inst->doorState = ST_UNKNOWN;
        return;
    }

    if (inst->doorState == ST_INIT)
        inst->doorState = ST_UNKNOWN;

    if (!inst->direction && inst->s0)
        inst->doorState = ST_OPEN;
    else if (inst->direction && inst->s3)
        inst->doorState = ST_CLOSE;
    else
    {
        switch (inst->doorState)
        {
            case ST_UNKNOWN:
                break;

            case ST_OPEN:
                if (inst->direction)
                    inst->doorState = ST_ACC_NEG;
                break;

            case ST_CLOSE:
                if (!inst->direction)
                    inst->doorState = ST_ACC_POS;
                break;

            case ST_ACC_POS:
                if (inst->direction)
                    inst->doorState = ST_DEC_NEG;
                else if (inst->s1)
                    inst->doorState = ST_DEC_POS;
                else if (inst->s2)
                    inst->doorState = ST_POS;
                break;

            case ST_POS:
                if (inst->direction)
                    inst->doorState = ST_NEG;
                else if (inst->s1)
                    inst->doorState = ST_DEC_POS;
                break;

            case ST_DEC_POS:
                if (inst->direction)
                    inst->doorState = ST_ACC_NEG;
                break;

            case ST_ACC_NEG:
                if (!inst->direction)
                    inst->doorState = ST_DEC_POS;
                else if (inst->s2)
                    inst->doorState = ST_DEC_NEG;
                else if (inst->s1)
                    inst->doorState = ST_NEG;
                break;

            case ST_NEG:
                if (!inst->direction)
                    inst->doorState = ST_POS;
                else if (inst->s2)
                    inst->doorState = ST_DEC_NEG;
                break;

            case ST_DEC_NEG:
                if (!inst->direction)
                    inst->doorState = ST_ACC_POS;
                break;

            default:
                inst->doorState = ST_UNKNOWN;
                return;
        }
    }

    switch (inst->doorState)
    {
        case ST_UNKNOWN:
            inst->speed = inst->direction ? -inst->speedSlow : inst->speedSlow;
            break;

        case ST_ACC_POS:
        case ST_DEC_POS:
            inst->speed = inst->speedSlow;
            break;

        case ST_ACC_NEG:
        case ST_DEC_NEG:
            inst->speed = -inst->speedSlow;
            break;

        case ST_POS:
            inst->speed = inst->speedFast;
            break;

        case ST_NEG:
            inst->speed = -inst->speedFast;
            break;

        default:
            inst->speed = 0;
            break;
    }
}
