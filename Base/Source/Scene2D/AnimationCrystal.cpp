#include "AnimationCrystal.h"
#include "../SoundEngine.h"


CAnimationCrystal::CAnimationCrystal()
	: time(0)
	, frameState(P_IDLE_R1)
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

		switch (frameState)
		{
		case P_RUN_L1:
		case P_RUN_L2:
		case P_RUN_R1:
		case P_RUN_R2:
			CSoundEngine::GetInstance()->PlayASound("walk");
			break;
		default:
			break;
		}
	}
}

int CAnimationCrystal::GetFrameState(void) const
{
	return frameState;
}

int CAnimationCrystal::GetFrameTotal(void) const
{
	return P_TOTAL;
}

int CAnimationCrystal::GuessNextFrame(int frame) const
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

bool CAnimationCrystal::isFacingRight() const
{
	return isFacingRight(frameState);
}

bool CAnimationCrystal::isFacingRight(int frame) const
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

bool CAnimationCrystal::isRolling(void) const
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

bool CAnimationCrystal::isAttacking(void) const
{
	switch (frameState)
	{
	case P_ATTACK_R1:
	case P_ATTACK_R2:
	case P_ATTACK_L1:
	case P_ATTACK_L2:
		return true;
		break;
	default:
		return false;
		break;
	}
}
