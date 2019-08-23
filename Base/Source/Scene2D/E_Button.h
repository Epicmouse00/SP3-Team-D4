#pragma once
#include "Vector3.h"
#include "Map.h"
#include "AnimationButton.h"
#include "PlayerInfo2D.h"

class E_Button : public CAnimationButton
{
protected:
	static E_Button *s_instance;
	E_Button(void);

public:
	static E_Button *GetInstance()
	{
		if (!s_instance)
			s_instance = new E_Button;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	~E_Button(void);

	// Initialise this class instance
	void Init(void);

	// Set position
	void SetPos(const Vector3& pos);

	// Get position
	Vector3 GetPos(void) const;
	// Update
	void Update(double dt = 0.0333f);

	int checkPosition_X, checkPosition_Y;
	Vector3 position;
	int mapOffset_x, mapOffset_y;

private:
	Vector3 defaultPosition;
	int tileSize_Width, tileSize_Height;
};
