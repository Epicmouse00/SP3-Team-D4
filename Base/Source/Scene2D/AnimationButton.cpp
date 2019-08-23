#include "AnimationButton.h"
#include "../SoundEngine.h"


CAnimationButton::CAnimationButton()
	: time(0)
	, frameState(B_1)
{
}


CAnimationButton::~CAnimationButton()
{
}

// Set Animation status; left or right
void CAnimationButton::SetAnimationStatus(int frameState)
{
	if (this->frameState != GuessNextFrame(frameState))
		this->frameState = frameState;
}
// Update the Animation Index
void CAnimationButton::UpdateAnimationIndex(double dt)
{
	time += dt;
	if (time > 3)
	{
		time = 0;
		frameState = GuessNextFrame(frameState);
	}
}

int CAnimationButton::GetFrameState(void) const
{
	return frameState;
}

int CAnimationButton::GetFrameTotal(void) const
{
	return B_TOTAL;
}

int CAnimationButton::GuessNextFrame(int frame) const
{
	switch (frame)
	{
	case B_1:
		return B_2;
		break;
	case B_2:
		return B_1;
		break;
	default:
		return B_TOTAL;
		break;
	}
}