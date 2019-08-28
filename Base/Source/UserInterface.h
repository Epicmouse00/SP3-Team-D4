#pragma once
#include "UIButton.h"
#include "Scene2D/PlayerInfo2D.h"
#include "../../Base/Source/Scene2D/Hearts.h"
#include "Projectile/Projectile.h"

class UserInterface
{
public:
	enum SCREEN_TYPE {
		SC_MAIN = 0,
		SC_PLAY,
		SC_PAUSE,
		SC_SKILL_TREE,
		SC_GAMEOVER,
		SC_INSTRUCTIONS,
		SC_CREDIT,
		SC_TOTAL
	};
	UserInterface(CProjectile* temporo);
	~UserInterface();
	bool Update(double dt);
	bool GetScreenStatus();
	void Render();

private:
	void ChangeScreen(SCREEN_TYPE screenType);
	void SetWords(SCREEN_TYPE screenType);
	void SetChoiceText();

	int choice;
	int maxChoices;
	SCREEN_TYPE screen;

	SpriteEntity** titleScreen;

	SpriteEntity** heartEntity;
	UIButton* buttonObj[5];
	SpriteEntity* xpBar;
	SpriteEntity* xpBlock;
	SpriteEntity* xpBlock2;
	SpriteEntity** staminaBar;
	int barStatus;
	SpriteEntity* staminaBattery;
	SpriteEntity* lifeBlock;
	SpriteEntity* lifeBar;
	SpriteEntity* UI_Bar;
	int numOfLines;
	TextEntity** textObj;
	SpriteEntity* levelUpScreen;
	SpriteEntity** skillUnlockedFrames;
	SpriteEntity* skillSelectedFrame;
	CProjectile* temporop;
	int selectionIndex;
	double dieTimer;
	double corruptionTimer;

	CPlayerInfo2D* thePlayerInfo;
	Hearts* theHeartInfo;
};

