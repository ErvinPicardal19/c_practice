#include <unistd.h>
#include "action.h"
#include "sensor.h"

void startMoving(TransitionInfo *t)
{
	/* Simulate obstacle getting close */
	decDistance(1);
	usleep(500000);
}

void stopMoving(TransitionInfo *t)
{
	State nextState = t->currState;

	t->currState = nextState;
}

void checkDirection(TransitionInfo *t)
{
	return;
}

void turnToDirection(TransitionInfo *t)
{
	return;
}

void fullStop(TransitionInfo *t)
{
	return;
}
