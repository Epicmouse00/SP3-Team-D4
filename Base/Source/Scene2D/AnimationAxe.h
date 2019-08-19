#pragma once
class CAnimationAxe
{
public:
	enum framestate
	{
		A_ATTACK_1,
		A_ATTACK_2,
		A_ATTACK_3,
		A_ATTACK_4,
		A_ATTACK_5,
		A_DIE,
		A_IDLE_1,
		A_IDLE_2,
		A_RUN_1,
		A_RUN_2,
		A_TOTAL
	};
	CAnimationAxe();
	virtual ~CAnimationAxe();

	// Set Animation status; left or right
	void SetAnimationStatus(int frameState);
	// Update the Animation Index
	void UpdateAnimationIndex(double dt);

	int GetFrameState(void) const;
	int GetFrameTotal(void) const;
	int GuessNextFrame(int frame) const;
	bool isAttacking(void) const;

private:
	int frameState;
	double time;
};

