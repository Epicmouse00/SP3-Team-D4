#pragma once
class CAnimationCrystal
{
public:
	enum framestate
	{
		C_ATTACK_1,
		C_ATTACK_2,
		C_DIE_1,
		C_DIE_2,
		C_IDLE_1,
		C_IDLE_2,
		C_TOTAL
	};
	CAnimationCrystal();
	virtual ~CAnimationCrystal();

	void SetAnimationStatus(int frameState);
	void UpdateAnimationIndex(double dt);

	int GetFrameState(void) const;
	int GetFrameTotal(void) const;
	int GuessNextFrame(int frame) const;
	bool isAttacking(void) const;

private:
	int frameState;
	double time;
};

