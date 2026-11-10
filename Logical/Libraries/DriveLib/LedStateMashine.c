#include <bur/plctypes.h>
#include "DriveLib.h"

void LedStateMashine(struct LedStateMashine* inst)
{
    unsigned char pattern;

    inst->led1 = 0;
    inst->led2 = 0;
    inst->led3 = 0;
    inst->led4 = 0;

    if (inst->state == ST_UNKNOWN)
    {
        if ((inst->cycleTime > 0.0f) && (inst->cycleTime <= 0.5f))
        {
            inst->timer += inst->cycleTime;
            if (inst->timer >= 0.5f)
            {
                inst->timer -= 0.5f;
                inst->blink = !inst->blink;
            }
        }

        inst->led1 = inst->blink;
        inst->led2 = inst->blink;
        inst->led3 = inst->blink;
        inst->led4 = inst->blink;
        return;
    }

    inst->timer = 0.0f;
    inst->blink = 0;
    pattern = 0;

    switch (inst->state)
    {
        case ST_CLOSE:
            pattern = 15;
            break;

        case ST_ACC_POS:
        case ST_DEC_NEG:
            pattern = 14;
            break;

        case ST_POS:
        case ST_NEG:
            pattern = 12;
            break;

        case ST_DEC_POS:
        case ST_ACC_NEG:
            pattern = 8;
            break;

        default:
            break;
    }

    inst->led1 = (pattern & 8) != 0;
    inst->led2 = (pattern & 4) != 0;
    inst->led3 = (pattern & 2) != 0;
    inst->led4 = (pattern & 1) != 0;
}
