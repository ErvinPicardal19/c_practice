#include <stdio.h>
#include <stdbool.h>
#include "fsm.h"

int main(void)
{
	State currState = STOP;
	State nextState = STOP;

	while(true)
	{
		nextState = checkNextState(currState);
	}

	return 0;
}
