#pragma once
class CAnimationHearts
{
public:
	enum framestate
	{
		H_1,
		H_2,
		H_3,
		H_4,
		H_TOTAL,
	};
	CAnimationHearts();
	virtual ~CAnimationHearts();

	// Set Animation status; left or right
	void SetAnimationStatus(int frameState);
	// Update the Animation Index
	void UpdateAnimationIndex(double dt);

	int GetFrameState(void) const;
	int GetFrameTotal(void) const;
	int GuessNextFrame(int frame) const;

private:
	int frameState;
	double time;
};

