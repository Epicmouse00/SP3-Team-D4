#include "AnimationAxe.h"
#include "../SoundEngine.h"


CAnimationAxe::CAnimationAxe()
	: time(0)
	, frameState(A_IDLE_1)
{
}


CAnimationAxe::~CAnimationAxe()
{
}

// Set Animation status; left or right
void CAnimationAxe::SetAnimationStatus(int frameState)
{
	if (this->frameState != GuessNextFrame(frameState))
		this->frameState = frameState;
}
// Update the Animation Index
void CAnimationAxe::UpdateAnimationIndex(double dt)
{
	time += dt;
	if (time > 3)
	{
		time = 0;
		frameState = GuessNextFrame(frameState);
	}
}

int CAnimationAxe::GetFrameState(void) const
{
	return frameState;
}

int CAnimationAxe::GetFrameTotal(void) const
{
	return A_TOTAL;
}

int CAnimationAxe::GuessNextFrame(int frame) const
{
	switch (frame)
	{
	case A_ATTACK_5:
	case A_IDLE_2:
		return A_IDLE_1;
		break;
	case A_RUN_2:
		return frame - 1;
		break;
	case A_DIE_2:
	case A_TOTAL:
		return A_TOTAL;
		break;
	default:
		return frame + 1;
		break;
	}
}

bool CAnimationAxe::isAttacking(void) const
{
	switch (frameState)
	{
	case A_ATTACK_1:
	case A_ATTACK_2:
	case A_ATTACK_3:
	case A_ATTACK_4:
	case A_ATTACK_5:
		return true;
		break;
	default:
		return false;
		break;
	}
}
