#ifndef __FSM_H__
#define __FSM_H__

/*
	States
	1. Move forward
	2. If Obstacle; Stop
	3. Check Left and Right
	4. Turn
*/
typedef enum
{
	MOVE_FORWARD = 0,
	STOP,
	CHECK_LEFT_RIGHT,
	TURN,
	OFF /* Always at last */
} State;

typedef struct TransitionInfo
{
	State currState;
	State nextState;
	void (*action)(struct TransitionInfo *);
} TransitionInfo;

State checkNextState(State currState);

#endif /*__FSM_H__*/
