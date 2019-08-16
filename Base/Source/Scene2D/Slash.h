#pragma once
#include "Vector3.h"
#include "Map.h"
#include "AnimationSlash.h"
#include "PlayerInfo2D.h"

class Slash : public CAnimationSlash
{
protected:
	static Slash *s_instance;
	Slash(void);

public:
	static Slash *GetInstance()
	{
		if (!s_instance)
			s_instance = new Slash;
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
	~Slash(void);

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
	enum Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		NOPE
	};
	Vector3 defaultPosition;
	int tileSize_Width, tileSize_Height;
	int direction;
};
