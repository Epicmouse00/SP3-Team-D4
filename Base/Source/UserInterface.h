#pragma once
#include "UIButton.h"
#include "Scene2D/PlayerInfo2D.h"
#include "../../Base/Source/Scene2D/Hearts.h"

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
	bool Update(double dt);
	bool GetScreenStatus();
	void Render();

private:
	void ChangeScreen(SCREEN_TYPE screenType);

	int choice;
	int maxChoices;
	SCREEN_TYPE screen;

	SpriteEntity** titleScreen;

	SpriteEntity** heartEntity;
	UIButton* buttonObj[3];
	SpriteEntity** staminaBar;
	int barStatus;
	SpriteEntity* staminaBattery;
	SpriteEntity* UI_Bar;
	TextEntity* textObj[2];

	CPlayerInfo2D* thePlayerInfo;
	Hearts* theHeartInfo;
};

