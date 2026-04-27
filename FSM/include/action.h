#ifndef __ACTION_H__
#define __ACTION_H__

#include "fsm.h"

void startMoving(TransitionInfo *t);

void stopMoving(TransitionInfo *t);

void checkDirection(TransitionInfo *t);

void turnToDirection(TransitionInfo *t);

void fullStop(TransitionInfo *t);

#endif /*__ACTION_H__*/
