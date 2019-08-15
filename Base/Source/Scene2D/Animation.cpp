#include "Animation.h"


CAnimation::CAnimation()
	: time(0)
	, frameState(P_IDLE_R1)
{
}


CAnimation::~CAnimation()
{
}

// Set Animation status; left or right
void CAnimation::SetAnimationStatus(int frameState)
{
	if (this->frameState != GuessNextFrame(frameState))
		this->frameState = frameState;
}
// Update the Animation Index
void CAnimation::UpdateAnimationIndex(double dt)
{
	time += dt;
	if (time > 3)
	{
		time = 0;
		frameState = GuessNextFrame(frameState);
	}
}

int CAnimation::GetFrameState(void) const
{
	return frameState;
}

int CAnimation::GetFrameTotal(void) const
{
	return P_TOTAL;
}

int CAnimation::GuessNextFrame(int frame) const
{
	switch (frame)
	{
	case P_IDLE_L2:
	case P_IDLE_R2:
	case P_RUN_L2:
	case P_RUN_R2:
	case P_FALL_L2:
	case P_FALL_R2:
	case P_POGO_L2:
	case P_POGO_R2:
		return frame - 1;
		break;
	case P_ATTACK_L2:
	case P_ROLL_L4:
		return P_IDLE_L1;
		break;
	case P_ATTACK_R2:
	case P_ROLL_R4:
		return P_IDLE_R1;
		break;
	case P_JUMP_L2:
	case P_JUMP_R2:
		return frame;
		break;
	default:
		return frame + 1;
		break;
	}
}

bool CAnimation::isFacingRight() const
{
	return isFacingRight(frameState);
}

bool CAnimation::isFacingRight(int frame) const
{
	switch (frame)
	{
	case P_IDLE_R1:
	case P_IDLE_R2:
	case P_RUN_R1:
	case P_RUN_R2:
	case P_ATTACK_R1:
	case P_ATTACK_R2:
	case P_JUMP_R1:
	case P_JUMP_R2:
	case P_FALL_R1:
	case P_FALL_R2:
	case P_POGO_R1:
	case P_POGO_R2:
	case P_ROLL_R1:
	case P_ROLL_R2:
	case P_ROLL_R3:
	case P_ROLL_R4:
		return true;
		break;
	default:
		return false;
		break;
	}
}

bool CAnimation::isRolling(void) const
{
	switch (frameState)
	{
	case P_ROLL_R1:
	case P_ROLL_R2:
	case P_ROLL_R3:
	case P_ROLL_R4:
	case P_ROLL_L1:
	case P_ROLL_L2:
	case P_ROLL_L3:
	case P_ROLL_L4:
		return true;
		break;
	default:
		return false;
		break;
	}
}
