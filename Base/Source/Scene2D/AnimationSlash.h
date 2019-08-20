#pragma once
class CAnimationSlash
{
public:
	enum framestate
	{
		S_U1,
		S_U2,
		S_D1,
		S_D2,
		S_L1_1,
		S_L1_2,
		S_L2_1,
		S_L2_2,
		S_L3_1,
		S_L3_2,
		S_L3_3,
		S_R1_1,
		S_R1_2,
		S_R2_1,
		S_R2_2,
		S_R3_1,
		S_R3_2,
		S_R3_3,
		S_TOTAL
	};
	CAnimationSlash();
	virtual ~CAnimationSlash();

	// Set Animation status; left or right
	void SetAnimationStatus(int frameState);
	// Update the Animation Index
	void UpdateAnimationIndex(double dt);

	int GetFrameState(void) const;
	int GetFrameTotal(void) const;
	int GuessNextFrame(int frame) const;
	bool isCharge(void) const;

private:
	int frameState;
	double time;
};

