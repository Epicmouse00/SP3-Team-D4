#include "AnimationSlash.h"
#include "../SoundEngine.h"


CAnimationSlash::CAnimationSlash()
	: time(0)
	, frameState(S_TOTAL)
{
}


CAnimationSlash::~CAnimationSlash()
{
}

// Set Animation status; left or right
void CAnimationSlash::SetAnimationStatus(int frameState)
{
	if (this->frameState != GuessNextFrame(frameState))
		this->frameState = frameState;
}
// Update the Animation Index
void CAnimationSlash::UpdateAnimationIndex(double dt)
{
	time += dt;
	if (time > 3)
	{
		time = 0;
		frameState = GuessNextFrame(frameState);
	}
}

int CAnimationSlash::GetFrameState(void) const
{
	return frameState;
}

int CAnimationSlash::GetFrameTotal(void) const
{
	return S_TOTAL;
}

int CAnimationSlash::GuessNextFrame(int frame) const
{
	switch (frame)
	{
	case S_U2:
	case S_D2:
	case S_L1_2:
	case S_L2_2:
	case S_L3_3:
	case S_R1_2:
	case S_R2_2:
	case S_R3_3:
	case S_TOTAL:
		return S_TOTAL;
		break;
	default:
		return frame + 1;
		break;
	}
}

bool CAnimationSlash::isCharge(void) const
{
	switch (frameState) // delet dis
	{
	case S_L3_1:
	case S_L3_2:
	case S_L3_3:
	case S_R3_1:
	case S_R3_2:
	case S_R3_3:
		return true;
		break;
	default:
		return false;
		break;
	}
}
