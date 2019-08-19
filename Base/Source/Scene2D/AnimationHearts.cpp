#include "AnimationHearts.h"
#include "../SoundEngine.h"


CAnimationHearts::CAnimationHearts()
	: time(0)
	, frameState(H_1)
{
}


CAnimationHearts::~CAnimationHearts()
{
}

// Set Animation status; left or right
void CAnimationHearts::SetAnimationStatus(int frameState)
{
	if (this->frameState != GuessNextFrame(frameState))
		this->frameState = frameState;
}
// Update the Animation Index
void CAnimationHearts::UpdateAnimationIndex(double dt)
{
	time += dt;
	if (time > 3)
	{
		time = 0;
		frameState = GuessNextFrame(frameState);
	}
}

int CAnimationHearts::GetFrameState(void) const
{
	return frameState;
}

int CAnimationHearts::GetFrameTotal(void) const
{
	return H_TOTAL;
}

int CAnimationHearts::GuessNextFrame(int frame) const
{
	switch (frame)
	{
	case H_4:
		return H_1;
		break;
	default:
		return frame + 1;
		break;
	}
}