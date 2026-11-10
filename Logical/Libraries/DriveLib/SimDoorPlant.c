#include <bur/plctypes.h>
#include "DriveLib.h"

static plcbit intermediateSensor(float previous, float current, float mark)
{
    if ((current >= mark - 0.5f) && (current <= mark + 0.5f))
        return 1;
    return ((previous < mark) && (current >= mark)) ||
           ((previous > mark) && (current <= mark));
}

void SimDoorPlant(struct SimDoorPlant* inst)
{
    float previousPosition;
    float previousSpeed;
    float targetSpeed;
    float maxChange;
    unsigned short command;

    inst->valid = (inst->cycleTime > 0.0f) && (inst->cycleTime <= 0.5f) &&
                  (inst->acceleration > 0.0f) && (inst->motionGain > 0.0f) &&
                  (inst->switchDelay >= 0.0f);

    if (inst->reset || !inst->initialized)
    {
        inst->position = inst->initialPosition;
        if (inst->position < 0.0f) inst->position = 0.0f;
        if (inst->position > 100.0f) inst->position = 100.0f;
        if (!((inst->position >= 0.0f) && (inst->position <= 100.0f)))
            inst->position = 50.0f;
        inst->actualSpeed = 0.0f;
        inst->state = DRV_SWITCH_ON_DISABLED;
        inst->commandTimer = 0.0f;
        inst->previousCommand = 0;
        inst->initialized = 1;
        previousPosition = inst->position;
    }
    else if (!inst->valid)
    {
        inst->actualSpeed = 0.0f;
        inst->state = DRV_SWITCH_ON_DISABLED;
        inst->commandTimer = 0.0f;
        previousPosition = inst->position;
    }
    else
    {
        previousPosition = inst->position;
        previousSpeed = inst->actualSpeed;
        command = inst->command & 0x000fU;

        if (command != inst->previousCommand)
        {
            inst->previousCommand = command;
            inst->commandTimer = 0.0f;
        }

        switch (inst->state)
        {
            case DRV_SWITCH_ON_DISABLED:
                if (command == DRV_CMD_SHUTDOWN)
                {
                    inst->commandTimer += inst->cycleTime;
                    if (inst->commandTimer >= inst->switchDelay)
                    {
                        inst->state = DRV_READY_TO_SWITCH_ON;
                        inst->commandTimer = 0.0f;
                    }
                }
                break;

            case DRV_READY_TO_SWITCH_ON:
                if (command == DRV_CMD_SWITCH_ON)
                {
                    inst->commandTimer += inst->cycleTime;
                    if (inst->commandTimer >= inst->switchDelay)
                    {
                        inst->state = DRV_SWITCHED_ON;
                        inst->commandTimer = 0.0f;
                    }
                }
                else if (command == 0)
                    inst->state = DRV_SWITCH_ON_DISABLED;
                break;

            case DRV_SWITCHED_ON:
                if (command == DRV_CMD_ENABLE)
                {
                    inst->commandTimer += inst->cycleTime;
                    if (inst->commandTimer >= inst->switchDelay)
                    {
                        inst->state = DRV_OPERATION_ENABLED;
                        inst->commandTimer = 0.0f;
                    }
                }
                else if (command == DRV_CMD_SHUTDOWN)
                    inst->state = DRV_READY_TO_SWITCH_ON;
                else if (command == 0)
                    inst->state = DRV_SWITCH_ON_DISABLED;
                break;

            case DRV_OPERATION_ENABLED:
                if (command == DRV_CMD_SHUTDOWN)
                    inst->state = DRV_READY_TO_SWITCH_ON;
                else if (command == DRV_CMD_SWITCH_ON)
                    inst->state = DRV_SWITCHED_ON;
                else if (command == 0)
                    inst->state = DRV_SWITCH_ON_DISABLED;
                break;

            default:
                inst->state = DRV_SWITCH_ON_DISABLED;
                inst->commandTimer = 0.0f;
                break;
        }

        targetSpeed = (inst->state == DRV_OPERATION_ENABLED) ?
                      (float)inst->speedSet : 0.0f;
        maxChange = inst->acceleration * inst->cycleTime;
        if (targetSpeed > inst->actualSpeed + maxChange)
            inst->actualSpeed += maxChange;
        else if (targetSpeed < inst->actualSpeed - maxChange)
            inst->actualSpeed -= maxChange;
        else
            inst->actualSpeed = targetSpeed;

        inst->position += (previousSpeed + inst->actualSpeed) * 0.5f *
                          inst->motionGain * inst->cycleTime;

        if (inst->position >= 100.0f)
        {
            inst->position = 100.0f;
            if (inst->actualSpeed > 0.0f) inst->actualSpeed = 0.0f;
        }
        if (inst->position <= 0.0f)
        {
            inst->position = 0.0f;
            if (inst->actualSpeed < 0.0f) inst->actualSpeed = 0.0f;
        }
    }

    inst->s0 = inst->position >= 99.9999f;
    inst->s1 = intermediateSensor(previousPosition, inst->position, 80.0f);
    inst->s2 = intermediateSensor(previousPosition, inst->position, 20.0f);
    inst->s3 = inst->position <= 0.0001f;
}
