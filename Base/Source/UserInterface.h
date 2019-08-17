#pragma once
#include "SpriteEntity.h"
#include "TextEntity.h"
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
	int choice;
	int maxChoices;
	SCREEN_TYPE screen;

	SpriteEntity* scene2DQuad;
	SpriteEntity* scene2DQuad2;
	TextEntity* textObj[3];
	CPlayerInfo2D* thePlayerInfo;
};

