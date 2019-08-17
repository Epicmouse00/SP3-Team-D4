#pragma once
#include "UIButton.h"
#include "Scene2D/PlayerInfo2D.h"

class UserInterface
{
public:
	enum SCREEN_TYPE {
		SC_MAIN = 0,
		SC_PLAY,
		SC_PAUSE,
		SC_TOTAL
	};
	UserInterface();
	~UserInterface();
	bool Update();
	bool GetScreenStatus();
	void Render();

private:
	void ChangeScreen(SCREEN_TYPE screenType);

	int choice;
	int maxChoices;
	SCREEN_TYPE screen;

	UIButton* buttonObj[3];
	SpriteEntity* scene2DCorruption;
	Vector3 CorruptionPosition;
	SpriteEntity* scene2DHp;
	TextEntity* textObj[2];
	CPlayerInfo2D* thePlayerInfo;
};

