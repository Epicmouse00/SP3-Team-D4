#include "UserInterface.h"
#include "KeyboardController.h"
#include "GamePadXbox.h"

#include "MeshBuilder.h"
#include "Application.h"
#include "SoundEngine.h"
#include <fstream>

using namespace std;
UserInterface::UserInterface(CProjectile* temporo)
	: choice(4)
	, maxChoices(5)
	, screen(SC_MAIN)
	, theHeartInfo(NULL)
	, barStatus(0)
	, numOfLines(0)
	, textObj(NULL)
	, selectionIndex(0)
	, dieTimer(0)
	, corruptionTimer(0)
	, is_Respawned(false)
{
	temporop = temporo;
	theHeartInfo = Hearts::GetInstance();
	theHeartInfo->Init();

	{
		float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
		float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;

		heartEntity = new SpriteEntity*[theHeartInfo->GetFrameTotal()];
		heartEntity[0] = Create::Sprite2DObject("Heart_1",
			Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
			Vector3(16.0f, 16.0f, 0.0f));
		heartEntity[1] = Create::Sprite2DObject("Heart_2",
			Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
			Vector3(16.0f, 16.0f, 0.0f));
		heartEntity[2] = Create::Sprite2DObject("Heart_3",
			Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
			Vector3(16.0f, 16.0f, 0.0f));
		heartEntity[3] = Create::Sprite2DObject("Heart_4",
			Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
			Vector3(16.0f, 16.0f, 0.0f));


		thePlayerInfo = CPlayerInfo2D::GetInstance();

		staminaBar = new SpriteEntity*[3];
		staminaBar[0] = Create::Sprite2DObject("Stamina_Green",
			Vector3(thePlayerInfo->GetStamina() / 2 + 18, 208.f, 0.0f),
			Vector3(thePlayerInfo->GetStamina(), 14.f, 0.0f));
		staminaBar[1] = Create::Sprite2DObject("Stamina_Amber",
			Vector3(thePlayerInfo->GetStamina() / 2 + 18, 208.f, 0.0f),
			Vector3(thePlayerInfo->GetStamina(), 14.f, 0.0f));
		staminaBar[2] = Create::Sprite2DObject("Stamina_Red",
			Vector3(thePlayerInfo->GetStamina() / 2 + 18, 208.f, 0.0f),
			Vector3(thePlayerInfo->GetStamina(), 14.f, 0.0f));

		staminaBattery = Create::Sprite2DObject("Stamina_Bar",
			Vector3(32.f, 208.f, 0.0f),
			Vector3(32.0f, 16.0f, 0.0f));

		titleScreen = new SpriteEntity*[3];
		titleScreen[0] = Create::Sprite2DObject("Title_Screen",
			Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
			Vector3(360.0f, 240.0f, 0.0f));
		titleScreen[1] = Create::Sprite2DObject("Title_Screen2",
			Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
			Vector3(360.0f, 240.0f, 0.0f));
		titleScreen[2] = Create::Sprite2DObject("SCENE2D_BKGROUND",
			Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
			Vector3(360.0f, 240.0f, 0.0f));

		UI_Bar = Create::Sprite2DObject("UI_Bar",
			Vector3(halfWindowWidth, 208.f, 0.0f),
			Vector3(360.0f, 32.0f, 0.0f));

		xpBar = Create::Sprite2DObject("XP_Bar",
			Vector3(static_cast<float>(thePlayerInfo->GetXP() / 2 + 150), 208.f, 0.0f),
			Vector3(static_cast<float>(thePlayerInfo->GetXP()), 14.f, 0.0f));

		lifeBar = Create::Sprite2DObject("XP_Bar",
			Vector3(static_cast<float>(thePlayerInfo->GetXP() / 2 + 270), 208.f, 0.0f),
			Vector3(static_cast<float>(thePlayerInfo->GetXP()), 14.f, 0.0f));

		xpBlock = Create::Sprite2DObject("XP_Block",
			Vector3(180.f, 208.f, 0.0f),
			Vector3(64.f, 16.f, 0.0f));

		xpBlock2 = Create::Sprite2DObject("XP_Block2",
			Vector3(180.f, 208.f, 0.0f),
			Vector3(64.f, 16.f, 0.0f));

		lifeBlock = Create::Sprite2DObject("Life_Block",
			Vector3(300.f, 208.f, 0.0f),
			Vector3(64.f, 16.f, 0.0f));

		levelUpScreen = Create::Sprite2DObject("Level_Up_Screen",
			Vector3(halfWindowWidth, halfWindowHeight, 0.f),
			Vector3(360.f, 224.f, 1.f));

		skillUnlockedFrames = new SpriteEntity*[13];

		skillUnlockedFrames[0] = Create::Sprite2DObject("Skill_Unlocked_Frame",
			Vector3(halfWindowWidth - 102, halfWindowHeight + 23, 0.f),
			Vector3(24.f, 24.f, 0.f));

		for (int i = 1; i < 5; ++i)
		{
			skillUnlockedFrames[i] = Create::Sprite2DObject("Skill_Unlocked_Frame",
				Vector3((halfWindowWidth - 63) + (i - 1) * 51, halfWindowHeight + 62, 0.f),
				Vector3(24.f, 24.f, 0.f));
		}

		for (int i = 5; i < 9; ++i)
		{
			skillUnlockedFrames[i] = Create::Sprite2DObject("Skill_Unlocked_Frame",
				Vector3((halfWindowWidth - 51) + (i - 5) * 51, halfWindowHeight + 23, 0.f),
				Vector3(24.f, 24.f, 0.f));
		}

		for (int i = 9; i < 13; ++i)
		{
			skillUnlockedFrames[i] = Create::Sprite2DObject("Skill_Unlocked_Frame",
				Vector3((halfWindowWidth - 63) + (i - 9) * 51, halfWindowHeight - 16, 0.f),
				Vector3(24.f, 24.f, 0.f));
		}
		skillSelectedFrame = Create::Sprite2DObject("Skill_Selected_Frame",
			Vector3(skillUnlockedFrames[selectionIndex]->GetPosition()),
			Vector3(24.f, 24.f, 0.f));
	}

	float fontSize = 16.0f;
	float halfFontSize = fontSize / 2.0f;

	for (int i = 0; i < maxChoices; ++i)
	{
		buttonObj[i] = new UIButton;
		buttonObj[i]->SetPosition(Vector3(5, 5 + fontSize * i + halfFontSize, 0.0f));
	}

	ChangeScreen(screen);
	buttonObj[choice]->SetSelected(true);
}

UserInterface::~UserInterface()
{
	for (int i = 0; i < theHeartInfo->GetFrameTotal(); ++i)
	{
		delete heartEntity[i];
		heartEntity[i] = NULL;
	}
	delete heartEntity;
	heartEntity = NULL;
	delete staminaBattery;
	staminaBattery = NULL;
	delete xpBar;
	xpBar = NULL;
	delete xpBlock;
	xpBlock = NULL;
	delete lifeBar;
	lifeBar = NULL;
	delete lifeBlock;
	lifeBlock = NULL;
	delete levelUpScreen;
	levelUpScreen = NULL;
	for (int i = 0; i < 13; ++i)
	{
		delete skillUnlockedFrames[i];
		skillUnlockedFrames[i] = NULL;
	}
	delete skillSelectedFrame;
	skillSelectedFrame = NULL;
	for (int i = 0; i < 3; ++i)
	{
		delete buttonObj[i];
		buttonObj[i] = NULL;
	}
	for (int i = 0; i < 3; ++i)
	{
		delete titleScreen[i];
		titleScreen[i] = NULL;
	}
	delete titleScreen;
	titleScreen = NULL;
	for (int i = 0; i < 3; ++i)
	{
		delete staminaBar[i];
		staminaBar[i] = NULL;
	}
	delete staminaBar;
	staminaBar = NULL;
	delete UI_Bar;
	UI_Bar = NULL;

}

bool UserInterface::Update(double dt)
{
	switch (screen) {
	case SC_MAIN: // This is the starting screen
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP)
			|| KeyboardController::GetInstance()->IsKeyPressed('W')
			|| GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_UP))
		{
			thePlayerInfo->SelectSound();
			buttonObj[choice]->SetSelected(false);
			choice = (choice + 1) % maxChoices;
			buttonObj[choice]->SetSelected(true);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN)
			|| KeyboardController::GetInstance()->IsKeyPressed('S')
			|| GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_DOWN))
		{
			thePlayerInfo->SelectSound();
			buttonObj[choice]->SetSelected(false);
			if (--choice < 0)
			{
				choice = maxChoices - 1;
			}
			buttonObj[choice]->SetSelected(true);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN)
			|| KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE)
			|| GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_A))
		{
			thePlayerInfo->SelectSound(1);
			switch (choice) {
			case 4:
				screen = SC_PLAY;
				ChangeScreen(screen);
				// Press F to at main menu to get all skills
				{
					if (KeyboardController::GetInstance()->IsKeyDown('F') || GamePadXbox::GetInstance()->IsKeyDown(GamePadXbox::GamePad_Button_START))
						thePlayerInfo->InitSkill(true);
				}
				CPlayerInfo2D::GetInstance()->Heal(false);
				CSoundEngine::GetInstance()->PlayBGM("bgm");
				return true;
				break;
			case 3:
				screen = SC_INSTRUCTIONS;
				ChangeScreen(screen);
				break;
			case 2:
				screen = SC_CREDIT;
				ChangeScreen(screen);
				break;
			case 1:
				ShellExecuteA(NULL, (LPCSTR)"open", (LPCSTR)"https://www.teepublic.com/user/leonidastee/albums/40379-lonin/", NULL, NULL, SW_SHOWNORMAL);
				thePlayerInfo->setExit(true);
				break;
			case 0:
				thePlayerInfo->setExit(true);
				break;
			}
		}
		return false;
		break;
	}
	case SC_PAUSE: // This is the pause menu
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP)
			|| KeyboardController::GetInstance()->IsKeyPressed('W')
			|| GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_UP))
		{
			thePlayerInfo->SelectSound();
			buttonObj[choice]->SetSelected(false);
			choice = (choice + 1) % maxChoices;
			buttonObj[choice]->SetSelected(true);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN)
			|| KeyboardController::GetInstance()->IsKeyPressed('S')
			|| GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_DOWN))
		{
			thePlayerInfo->SelectSound();
			buttonObj[choice]->SetSelected(false);
			if (--choice < 0)
			{
				choice = maxChoices - 1;
			}
			buttonObj[choice]->SetSelected(true);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN)
			|| KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE)
			|| GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_A))
		{
			thePlayerInfo->SelectSound(1);
			switch (choice) {
			case 2:
				screen = SC_PLAY;
				ChangeScreen(screen);
				CSoundEngine::GetInstance()->PlayBGM("bgm");
				return true;
				break;
			case 1:
				screen = SC_PLAY;
				ChangeScreen(screen);
				CSoundEngine::GetInstance()->PlayBGM("bgm");
				thePlayerInfo->Respawn();
				thePlayerInfo->Heal(false);
				break;
			case 0:
				thePlayerInfo->setExit(true);
				break;
			}
		}
		return false;
		break;
	}
	case SC_SKILL_TREE:
	{
		corruptionTimer -= dt;
		if (corruptionTimer < 0.f)
			temporop->SetSpeed(30.f);
		if (KeyboardController::GetInstance()->IsKeyPressed('A') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_LEFT))
		{
			thePlayerInfo->SelectSound();
			if (selectionIndex != 0)
			{
				if (selectionIndex != 1 && selectionIndex != 5 && selectionIndex != 9)
				{
					selectionIndex -= 1;
				}
				else
				{
					selectionIndex = 0;
				}
			}
			SetChoiceText();
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed('D') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_RIGHT))
		{
			thePlayerInfo->SelectSound();
			if (selectionIndex != 4 && selectionIndex != 8 && selectionIndex != 12)
			{
				if (selectionIndex != 0)
				{
					selectionIndex += 1;
				}
				else
				{
					selectionIndex = 5;
				}
			}
			SetChoiceText();
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed('W') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_UP))
		{
			thePlayerInfo->SelectSound();
			if (selectionIndex == 0 || selectionIndex > 4)
			{
				if (selectionIndex != 0)
				{
					selectionIndex -= 4;
				}
				else
				{
					selectionIndex = 1;
				}
			}
			SetChoiceText();
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed('S') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_DOWN))
		{
			thePlayerInfo->SelectSound();
			if (selectionIndex == 0 || selectionIndex < 9)
			{
				if (selectionIndex != 0)
				{
					selectionIndex += 4;
				}
				else
				{
					selectionIndex = 9;
				}
			}
			SetChoiceText();
		}
		skillSelectedFrame->SetPosition(skillUnlockedFrames[selectionIndex]->GetPosition());
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE) || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_A))
		{
			if (thePlayerInfo->isUnlockable(selectionIndex) && thePlayerInfo->MinusLevel())
			{
				thePlayerInfo->setSkill(selectionIndex, true);
			}
			else
				thePlayerInfo->SelectSound(2);
		}

		if (KeyboardController::GetInstance()->IsKeyPressed('E') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_X) || thePlayerInfo->position.x < temporop->GetPosition().x + temporop->GetScale().x / 2)
		{
			screen = SC_PLAY;
			ChangeScreen(screen);
			thePlayerInfo->SetSpawn();
			CPlayerInfo2D::GetInstance()->Heal();

			if (textObj != NULL) {
				for (int i = 0; i < numOfLines; ++i) {
					delete textObj[i];
					textObj[i] = NULL;
				}
				numOfLines = 0;
				delete textObj;
				textObj = NULL;
			}
			temporop->SetSpeed(30.f);
			corruptionTimer = 0;
			return true;
		}

		return true;
		break;
	}
	case SC_GAMEOVER:
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN)
			|| KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE)
			|| GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_A))
		{
			screen = SC_PLAY;
			ChangeScreen(screen);
			thePlayerInfo->Respawn();
			is_Respawned = true;
			temporop->SetPosition(Vector3(static_cast<float>(thePlayerInfo->GetPos().x - thePlayerInfo->GetMap()->getScreenWidth()), static_cast<float>(thePlayerInfo->GetMap()->getScreenHeight()) / 2, 0));
			thePlayerInfo->Heal(false);
			return true;
		}
		return true;
		break;
	}
	case SC_INSTRUCTIONS:
	case SC_CREDIT:
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN)
			|| KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE)
			|| GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_A))
		{
			thePlayerInfo->SelectSound(1);
			screen = SC_MAIN;
			ChangeScreen(screen);
			if (textObj != NULL) {
				for (int i = 0; i < numOfLines; ++i) {
					delete textObj[i];
					textObj[i] = NULL;
				}
				numOfLines = 0;
				delete textObj;
				textObj = NULL;
			}
		}
		return false;
		break;
	}
	case SC_PLAY: // This just checks for changes in UI* stuff while in play
	{
		corruptionTimer -= dt;
		if(corruptionTimer<0.f)
			temporop->SetSpeed(30.f);
		// Heart update
		theHeartInfo->Update(dt);
		barStatus = 0;
		float staminatemp = thePlayerInfo->GetStamina();
		if (staminatemp < 0.7f)
		{
			barStatus = 1;
			if (staminatemp < 0.4f)
				barStatus = 2;
		}
		staminaBar[barStatus]->SetScale(Vector3((staminatemp) * 25, staminaBar[barStatus]->GetScale().y));
		staminaBar[barStatus]->SetPosition(Vector3(staminaBar[barStatus]->GetScale().x / 2 + 18, staminaBar[barStatus]->GetPosition().y));

		xpBar->SetScale(Vector3(static_cast<float>((thePlayerInfo->GetXP() / (thePlayerInfo->GetLevel(true) + 2)) * 60), xpBar->GetScale().y));
		xpBar->SetPosition(Vector3(xpBar->GetScale().x / 2 + 150, xpBar->GetPosition().y));

		lifeBar->SetScale(Vector3(static_cast<float>((thePlayerInfo->GetLifesteal()) * 6), lifeBar->GetScale().y));
		lifeBar->SetPosition(Vector3(lifeBar->GetScale().x / 2 + 270, lifeBar->GetPosition().y));

		//std::ostringstream ss;
		//ss.precision(5);
		//ss << "CP: " << thePlayerInfo->checkPosition_X << ", " << thePlayerInfo->checkPosition_Y << endl
		//	<< "P: " << thePlayerInfo->position << endl;
		//textObj[1]->SetText(ss.str());

		//ss.str("");
		//ss.clear();
		//ss << "mapOffset_x: " << thePlayerInfo->mapOffset_x << endl;
		//textObj[0]->SetText(ss.str());

		if (thePlayerInfo->GetHp() <= 0)
		{
			if (dieTimer > 1.f)
			{
				screen = SC_GAMEOVER;
				ChangeScreen(screen);
				return false;
			}
			else
				dieTimer += 1 * dt;
		}
		else
			dieTimer = 0;

		if ((KeyboardController::GetInstance()->IsKeyPressed('E') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_X))
			&& thePlayerInfo->position.x > temporop->GetPosition().x + temporop->GetScale().x / 2
			&& (thePlayerInfo->GetMap()->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X]==30||
			thePlayerInfo->checkPosition_X + 1 < thePlayerInfo->GetMap()->GetNumOfTiles_Width() && thePlayerInfo->GetMap()->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X + 1] == 30))
		{
			CSoundEngine::GetInstance()->PlayASound("levelup");
			screen = SC_SKILL_TREE;
			if(temporop->GetPosition().x < static_cast<float>(thePlayerInfo->GetPos().x - thePlayerInfo->GetMap()->getScreenWidth()))
				temporop->SetPosition(Vector3(static_cast<float>(thePlayerInfo->GetPos().x - thePlayerInfo->GetMap()->getScreenWidth()), static_cast<float>(thePlayerInfo->GetMap()->getScreenHeight()) / 2, 0));
			if (thePlayerInfo->GetTileOffset_x() > 200 || thePlayerInfo->GetMap()->GetNumOfTiles_Width() - thePlayerInfo->GetTileOffset_x() < 300) // if(Move forward too much(delete back) / Move forward too much(add forward))
				is_Respawned = true;// i mean.. i "could" place this outside of this.. but i dw sudden lag for no reason
			corruptionTimer = 30.f;
			temporop->SetSpeed(5.f);
			ChangeScreen(screen);
			return true;
		}

		if (KeyboardController::GetInstance()->IsKeyPressed('P') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_START))
		{
			screen = SC_PAUSE;
			ChangeScreen(screen);
			return false;
		}
		return true;
		break;
	}
	default:
	{
		return false;
		break;
	}
	}
	return true;
}

void UserInterface::ChangeScreen(SCREEN_TYPE screenType)
{
	switch (screenType) {
	case SC_MAIN:
		CSoundEngine::GetInstance()->PlayBGM("bgmrroll");

		maxChoices = 5;
		buttonObj[choice]->SetSelected(false);
		choice = 4;
		buttonObj[choice]->SetSelected(true);

		buttonObj[4]->SetText("Play");

		buttonObj[3]->SetText("Instructions");

		buttonObj[2]->SetText("Credits");

		buttonObj[1]->SetText("Merch");

		buttonObj[0]->SetText("Exit");
		thePlayerInfo->setScreenState(SC_MAIN);
		break;
	case SC_PLAY:
		thePlayerInfo->setScreenState(SC_PLAY);
		break;
	case SC_PAUSE:
		CSoundEngine::GetInstance()->PlayBGM("bgmwalk");

		maxChoices = 3;
		buttonObj[choice]->SetSelected(false);
		choice = 2;
		buttonObj[choice]->SetSelected(true);

		buttonObj[2]->SetText("Continue");

		buttonObj[1]->SetText("Load");

		buttonObj[0]->SetText("Exit");
		thePlayerInfo->setScreenState(SC_PAUSE);
		break;
	case SC_SKILL_TREE:
		thePlayerInfo->SetAnimationStatus(CPlayerInfo2D::P_IDLE_R1);
		for (int i = 1; i < maxChoices; ++i) {
			buttonObj[i]->SetText("");
		}
		maxChoices = 0;
		buttonObj[choice]->SetSelected(false);
		choice = 0;

		levelUpScreen->SetPosition(Vector3(static_cast<float>(thePlayerInfo->GetMap()->getScreenWidth()) / 2, static_cast<float>(thePlayerInfo->GetMap()->getScreenHeight()) / 2, 0.0f));

		SetChoiceText();

		thePlayerInfo->setScreenState(SC_SKILL_TREE);
		break;
	case SC_GAMEOVER:
		for (int i = 1; i < maxChoices; ++i) {
			buttonObj[i]->SetText("");
		}
		maxChoices = 1;

		buttonObj[choice]->SetSelected(false);
		choice = 0;
		buttonObj[choice]->SetSelected(true);

		buttonObj[0]->SetText("Game Over?");

		thePlayerInfo->setScreenState(SC_GAMEOVER);
		break;
	case SC_INSTRUCTIONS:
		for (int i = 1; i < maxChoices; ++i) {
			buttonObj[i]->SetText("");
		}
		maxChoices = 1;

		buttonObj[choice]->SetSelected(false);
		choice = 0;
		buttonObj[choice]->SetSelected(true);

		buttonObj[0]->SetText("Back");

		SetWords(screen);

		break;
	case SC_CREDIT:
		for (int i = 1; i < maxChoices; ++i) {
			buttonObj[i]->SetText("");
		}
		maxChoices = 1;

		buttonObj[choice]->SetSelected(false);
		choice = 0;
		buttonObj[choice]->SetSelected(true);

		buttonObj[0]->SetText("Back");

		SetWords(screen);

		break;
	}
}

void UserInterface::SetWords(SCREEN_TYPE screenType)
{
	ifstream file;
	switch(screenType)
	{
	case SC_INSTRUCTIONS:
	{
		if (GamePadXbox::GetInstance()->is_connected())
			file.open(".//Image//Instructions.txt", ios::in);
		else
			file.open(".//Image//Instructions2.txt", ios::in);
		break;
	}
	case SC_CREDIT:
		file.open(".//Image//Credits.txt", ios::in);
		break;
	}
	if (file.is_open())
	{
		string aText = "";
		getline(file, aText);
		numOfLines = stoi(aText);
		textObj = new TextEntity*[numOfLines];
		for (int i = 0; file.good() && i < numOfLines; ++i)
		{
			string aLineOfText = "";
			getline(file, aLineOfText);
			textObj[i] = Create::Text2DObject("text",
				Vector3(5, 230 - 16 * i - 8, 0.0f),
				aLineOfText,
				Vector3(8, 8), Color(1.0f, 0.0f, 0.0f));
		}
	}
}

void UserInterface::SetChoiceText()
{
	if (textObj == NULL) {
		numOfLines = 2;
		textObj = new TextEntity*[numOfLines];
		textObj[0] = Create::Text2DObject("text",
			Vector3(90, 65, 0.0f),
			"",
			Vector3(10, 10), Color(1.0f, 0.0f, 0.0f));
		textObj[1] = Create::Text2DObject("text",
			Vector3(92, 55, 0.0f),
			"",
			Vector3(7, 7), Color(1.0f, 0.0f, 0.0f));

	}

	ifstream file(".//Image//Skills.txt");
	if (file.is_open())
	{
		string aLineOfText = "";
		for (int i = 0; i < selectionIndex * 2; ++i)
			getline(file, aLineOfText);
		getline(file, aLineOfText);
		textObj[0]->SetText(aLineOfText);
		getline(file, aLineOfText);
		textObj[1]->SetText(aLineOfText);
		file.close();
	}
}

bool UserInterface::GetScreenStatus()
{
	switch (screen) {// true == renders the game ... false == don't render the game
	case SC_MAIN:
		return false;
		break;
	case SC_PLAY:
		return true;
		break;
	case SC_PAUSE:
		return true;
		break;
	case SC_SKILL_TREE:
		return true;
		break;
	case SC_GAMEOVER:
		return true;
		break;
	case SC_INSTRUCTIONS:
		return false;
		break;
	case SC_CREDIT:
		return false;
		break;
	}
	return true;
}

bool UserInterface::IsRespawned()
{
	if(is_Respawned)
	{
		is_Respawned = false;
		return true;
	}
	return false;
}

void UserInterface::Render()// this is at the back since it needs to be on top? (eg. real time skill tree upgrading / shoping)
{
	switch(screen) {
	case SC_MAIN:
		if (KeyboardController::GetInstance()->IsKeyDown('F') || GamePadXbox::GetInstance()->IsKeyDown(GamePadXbox::GamePad_Button_START))
			titleScreen[1]->RenderUI();
		else
			titleScreen[0]->RenderUI();

		for (int i = 0; i < maxChoices; ++i) {
			buttonObj[i]->RenderUI();
		}
		break;
	case SC_PLAY:
		UI_Bar->RenderUI();
		for (int i = 0; i < thePlayerInfo->GetHp(); ++i)
		{
			heartEntity[theHeartInfo->GetFrameState()]->SetPosition(Vector3(static_cast<float>(32 * i + 64), 208.f, 0.f));
			heartEntity[theHeartInfo->GetFrameState()]->RenderUI();
		}
		staminaBar[barStatus]->RenderUI();
		staminaBattery->RenderUI();
		xpBar->RenderUI();
		if (thePlayerInfo->GetLevel())
			xpBlock2->RenderUI();
		else
			xpBlock->RenderUI();
		if (thePlayerInfo->getSkill(CPlayerInfo2D::SK_LIFESTEAL))
		{
			lifeBar->RenderUI();
			lifeBlock->RenderUI();
		}
		return;
		break;
	case SC_PAUSE:
		for (int i = 0; i < maxChoices; ++i) {
			buttonObj[i]->RenderUI();
		}
		break;
	case SC_SKILL_TREE:
		levelUpScreen->RenderUI();
		for (int i = 0; i < 13; ++i)
		{
			if (thePlayerInfo->getSkill(i) == true)
			{
				skillUnlockedFrames[i]->RenderUI();
			}
		}
		skillSelectedFrame->RenderUI();
		for (int i = 0; i < numOfLines; ++i)
			textObj[i]->RenderUI();
		break;
	case SC_GAMEOVER:
		for (int i = 0; i < maxChoices; ++i) {
			buttonObj[i]->RenderUI();
		}
		break;
	case SC_INSTRUCTIONS:
		titleScreen[2]->RenderUI();
		for (int i = 0; i < numOfLines; ++i)
			textObj[i]->RenderUI();
		buttonObj[0]->RenderUI();
		break;
	case SC_CREDIT:
		titleScreen[2]->RenderUI();
		for (int i = 0; i < numOfLines; ++i)
			textObj[i]->RenderUI();
		buttonObj[0]->RenderUI();
		break;
	}
}