#pragma once
class CAnimationButton
{
public:
	enum framestate
	{
		B_1,
		B_2,
		B_3,
		B_4,
		B_TOTAL
	};
	CAnimationButton();
	virtual ~CAnimationButton();

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

