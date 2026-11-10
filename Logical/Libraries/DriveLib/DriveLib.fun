
FUNCTION_BLOCK DriveStateMashine
	VAR_INPUT
		state : UINT;
		enable : BOOL;
		speedSet : INT;
	END_VAR
	VAR_OUTPUT
		command : UINT;
		speed : INT;
		ready : BOOL;
	END_VAR
	VAR
		step : USINT;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK DoorStateMashine
	VAR_INPUT
		enable : BOOL;
		state : UINT;
		s0 : BOOL;
		s1 : BOOL;
		s2 : BOOL;
		s3 : BOOL;
		direction : BOOL;
		speedSlow : INT;
		speedFast : INT;
	END_VAR
	VAR_OUTPUT
		speed : INT;
		doorState : DoorStates;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK LedStateMashine
	VAR_INPUT
		state : DoorStates;
		cycleTime : REAL;
	END_VAR
	VAR_OUTPUT
		led1 : BOOL;
		led2 : BOOL;
		led3 : BOOL;
		led4 : BOOL;
	END_VAR
	VAR
		timer : REAL;
		blink : BOOL;
	END_VAR
END_FUNCTION_BLOCK


FUNCTION_BLOCK SimDoorPlant
    VAR_INPUT
        reset : BOOL;
        initialPosition : REAL;
        command : UINT;
        speedSet : INT;
        cycleTime : REAL;
        acceleration : REAL;
        motionGain : REAL;
        switchDelay : REAL;
    END_VAR
    VAR_OUTPUT
        state : UINT;
        position : REAL;
        actualSpeed : REAL;
        s0 : BOOL;
        s1 : BOOL;
        s2 : BOOL;
        s3 : BOOL;
        valid : BOOL;
    END_VAR
    VAR
        initialized : BOOL;
        commandTimer : REAL;
        previousCommand : UINT;
    END_VAR
END_FUNCTION_BLOCK
