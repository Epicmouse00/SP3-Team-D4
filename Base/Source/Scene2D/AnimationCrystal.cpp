#include "AnimationCrystal.h"
#include "../SoundEngine.h"


CAnimationCrystal::CAnimationCrystal()
	: time(0)
	, frameState(C_IDLE_1)
{
}


CAnimationCrystal::~CAnimationCrystal()
{
}

// Set Animation status; left or right
void CAnimationCrystal::SetAnimationStatus(int frameState)
{
	if (this->frameState != GuessNextFrame(frameState))
		this->frameState = frameState;
}
// Update the Animation Index
void CAnimationCrystal::UpdateAnimationIndex(double dt)
{
	time += dt;
	if (time > 3)
	{
		time = 0;
		frameState = GuessNextFrame(frameState);
	}
}

int CAnimationCrystal::GetFrameState(void) const
{
	return frameState;
}

int CAnimationCrystal::GetFrameTotal(void) const
{
	return C_TOTAL;
}

int CAnimationCrystal::GuessNextFrame(int frame) const
{
	switch (frame)
	{
	case C_IDLE_2:
	case C_ATTACK_2:
		return C_IDLE_1;
		break;
	case C_DIE_2:
		return C_TOTAL;
		break;
	default:
		return frame + 1;
		break;
	}
}

bool CAnimationCrystal::isAttacking(void) const
{
	switch (frameState)
	{
	case C_ATTACK_1:
	case C_ATTACK_2:
		return true;
		break;
	default:
		return false;
		break;
	}
}
