#include <stddef.h>
#include <stdbool.h>
#include "fsm.h"
#include "action.h"
#include "sensor.h"

#define STOP_DISTANCE 5 /* Meter */

State checkNextState(State currState)
{
	State nextState = STOP;

	switch(currState)
	{
		case MOVE_FORWARD:
			if(readSensor() <= STOP_DISTANCE)
				nextState = STOP;
			else
				nextState = MOVE_FORWARD;
		break;

		case STOP:
			if(readSensor() > STOP_DISTANCE)
				nextState = MOVE_FORWARD;
			else
				nextState = CHECK_LEFT_RIGHT;
		break;

		case CHECK_LEFT_RIGHT:
			nextState = TURN;
		break;

		case TURN:
			nextState = STOP;
		break;

		default:
			nextState = OFF;
	}

	return nextState;
}

TransitionInfo fsm[] =
{
	{ MOVE_FORWARD, STOP, stopMoving }, /* MOVE_FORWARD -> STOP */
	{ STOP, MOVE_FORWARD, startMoving }, /* STOP -> CHECK_LEFT_RIGHT */
	{ STOP, CHECK_LEFT_RIGHT, checkDirection }, /* STOP -> CHECK_LEFT_RIGHT */
	{ CHECK_LEFT_RIGHT, TURN, turnToDirection }, /* CHECK_LEFT_RIGHT -> TURN */
	{ TURN, STOP, stopMoving }, /* TURN -> STOP */
	{ OFF, OFF, fullStop }, /* OFF */
};

