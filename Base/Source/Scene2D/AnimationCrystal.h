#pragma once
class CAnimationCrystal
{
public:
	enum framestate
	{
		P_IDLE_R1,
		P_IDLE_R2,
		P_IDLE_L1,
		P_IDLE_L2,
		P_RUN_R1,
		P_RUN_R2,
		P_RUN_L1,
		P_RUN_L2,
		P_ATTACK_R1,
		P_ATTACK_R2,
		P_ATTACK_L1,
		P_ATTACK_L2,
		P_JUMP_R1,
		P_JUMP_R2,
		P_JUMP_L1,
		P_JUMP_L2,
		P_FALL_R1,
		P_FALL_R2,
		P_FALL_L1,
		P_FALL_L2,
		P_POGO_R1,
		P_POGO_R2,
		P_POGO_L1,
		P_POGO_L2,
		P_ROLL_R1,
		P_ROLL_R2,
		P_ROLL_R3,
		P_ROLL_R4,
		P_ROLL_L1,
		P_ROLL_L2,
		P_ROLL_L3,
		P_ROLL_L4,
		P_TOTAL
	};
	CAnimationCrystal();
	virtual ~CAnimationCrystal();

	// Set Animation status; left or right
	void SetAnimationStatus(int frameState);
	// Update the Animation Index
	void UpdateAnimationIndex(double dt);

	int GetFrameState(void) const;
	int GetFrameTotal(void) const;
	int GuessNextFrame(int frame) const;
	bool isFacingRight() const;
	bool isFacingRight(int frame) const;
	bool isRolling(void) const;
	bool isAttacking(void) const;

private:
	int frameState;
	double time;
};

