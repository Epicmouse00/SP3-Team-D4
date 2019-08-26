#include "UserInterface.h"
#include "KeyboardController.h"
#include "GamePadXbox.h"

#include "MeshBuilder.h"
#include "Application.h"
#include "SoundEngine.h"

using namespace std;
UserInterface::UserInterface()
	: choice(2)
	, maxChoices(3)
	, screen(SC_MAIN)
	, theHeartInfo(NULL)
	, barStatus(0)
	, selectionIndex(0)
{
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

		titleScreen = new SpriteEntity*[2];
		titleScreen[0] = Create::Sprite2DObject("Title_Screen",
			Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
			Vector3(360.0f, 240.0f, 0.0f));
		titleScreen[1] = Create::Sprite2DObject("Title_Screen2",
			Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
			Vector3(360.0f, 240.0f, 0.0f));

		UI_Bar = Create::Sprite2DObject("UI_Bar",
			Vector3(halfWindowWidth, 208.f, 0.0f),
			Vector3(360.0f, 32.0f, 0.0f));

		xpBar = Create::Sprite2DObject("XP_Bar",
			Vector3(static_cast<float>(thePlayerInfo->GetXP() / 2 + 150), 208.f, 0.0f),
			Vector3(static_cast<float>(thePlayerInfo->GetXP()), 14.f, 0.0f));

		xpBlock = Create::Sprite2DObject("XP_Block",
			Vector3(180.f, 208.f, 0.0f),
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
	for (int i = 0; i < 2; ++i)
	{//pointing at null...
		textObj[i] = Create::Text2DObject("text",
			Vector3(5, 5 + fontSize * i + halfFontSize, 0.0f),
			"",
			Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));
	}
	textObj[0]->SetText("Something");
	textObj[1]->SetText("Something2");

	for (int i = 0; i < 3; ++i)
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
	for (int i = 0; i < 2; ++i)
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
			buttonObj[choice]->SetSelected(false);
			choice = (choice + 1) % maxChoices;
			buttonObj[choice]->SetSelected(true);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN)
			|| KeyboardController::GetInstance()->IsKeyPressed('S')
			|| GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_DOWN))
		{
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
			switch (choice) {
			case 2:
				screen = SC_PLAY;
				ChangeScreen(screen);
				// Press F to at main menu to get all skills
				{
					if (KeyboardController::GetInstance()->IsKeyDown('F') || GamePadXbox::GetInstance()->IsKeyDown(GamePadXbox::GamePad_Button_START))
						thePlayerInfo->InitSkill(true);
				}
				CPlayerInfo2D::GetInstance()->Heal(false);
				return true;
				break;
			case 1: // (LPCSTR) for debug, (LPCWSTR) for release
				ShellExecute(0, 0, (LPCSTR)("https://www.teepublic.com/t-shirt/5731054-rollin-like-lonin"), 0, 0, SW_SHOW);
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
			buttonObj[choice]->SetSelected(false);
			choice = (choice + 1) % maxChoices;
			buttonObj[choice]->SetSelected(true);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN)
			|| KeyboardController::GetInstance()->IsKeyPressed('S')
			|| GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_DOWN))
		{
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
			switch (choice) {
			case 2:
				screen = SC_PLAY;
				ChangeScreen(screen);
				return true;
				break;
			case 1:
				break;
			case 0:
				break;
			}
		}
		return false;
		break;
	}
	case SC_SKILL_TREE:
	{
		if (KeyboardController::GetInstance()->IsKeyPressed('A') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_LEFT))
		{
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
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed('D') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_RIGHT))
		{
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
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed('W') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_UP))
		{
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
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed('S') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_DOWN))
		{
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
		}
		skillSelectedFrame->SetPosition(skillUnlockedFrames[selectionIndex]->GetPosition());
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_A))
		{
			thePlayerInfo->setSkill(selectionIndex, true);
		}

		if (KeyboardController::GetInstance()->IsKeyPressed('E') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_START))
		{
			screen = SC_PLAY;
			ChangeScreen(screen);
			CPlayerInfo2D::GetInstance()->Heal();
			return true;
		}
		return true;
		break;
	}
	case SC_SHOP:
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT)
			|| KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT)
			|| GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_LEFT)
			|| GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_DPAD_RIGHT))
		{
			screen = SC_SKILL_TREE;
			ChangeScreen(screen);
			return true;
		}
		if (thePlayerInfo->GetHp()>0&&!(thePlayerInfo->GetMap()->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X] == 30 ||
			thePlayerInfo->checkPosition_X + 1 < thePlayerInfo->GetMap()->GetNumOfTiles_Width() && thePlayerInfo->GetMap()->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X + 1] == 30))
		{
			screen = SC_PLAY;
			ChangeScreen(screen);
			return true;
		}
		return true;
		break;
	}
	case SC_PLAY: // This just checks for changes in UI* stuff while in play
	{
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

		xpBar->SetScale(Vector3(static_cast<float>((thePlayerInfo->GetXP()) * 6), xpBar->GetScale().y));
		xpBar->SetPosition(Vector3(xpBar->GetScale().x / 2 + 150, xpBar->GetPosition().y));

		std::ostringstream ss;
		ss.precision(5);
		ss << "CP: " << thePlayerInfo->checkPosition_X << ", " << thePlayerInfo->checkPosition_Y << endl
			<< "P: " << thePlayerInfo->position << endl;
		textObj[1]->SetText(ss.str());

		ss.str("");
		ss.clear();
		ss << "mapOffset_x: " << thePlayerInfo->mapOffset_x << endl;
		textObj[0]->SetText(ss.str());

		if (thePlayerInfo->GetHp() <= 0)
		{
			screen = SC_SHOP;
			ChangeScreen(screen);
			return false;
		}

		if ((KeyboardController::GetInstance()->IsKeyPressed('E') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_A))
			&& (thePlayerInfo->GetMap()->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X]==30||
			thePlayerInfo->checkPosition_X + 1 < thePlayerInfo->GetMap()->GetNumOfTiles_Width() && thePlayerInfo->GetMap()->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X + 1] == 30))
		{
			screen = SC_SKILL_TREE;
			ChangeScreen(screen);
			choice = 2;
			maxChoices = 3;
			return true;
		}

		if (KeyboardController::GetInstance()->IsKeyPressed('P') || GamePadXbox::GetInstance()->IsKeyPressed(GamePadXbox::GamePad_Button_START))
		{
			screen = SC_PAUSE;
			ChangeScreen(screen);
			choice = 2;
			maxChoices = 3;
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
		buttonObj[2]->SetText("Play");

		buttonObj[1]->SetText("Merch");

		buttonObj[0]->SetText("Exit");
		thePlayerInfo->setScreenState(SC_MAIN);
		break;
	case SC_PLAY:
		CSoundEngine::GetInstance()->PlayBGM("bgm");
		thePlayerInfo->setScreenState(SC_PLAY);
		break;
	case SC_PAUSE:
		CSoundEngine::GetInstance()->PlayBGM("bgmwalk");
		buttonObj[2]->SetText("Continue");

		buttonObj[1]->SetText("Save");

		buttonObj[0]->SetText("Load");
		thePlayerInfo->setScreenState(SC_PAUSE);
		break;
	case SC_SKILL_TREE:
		CSoundEngine::GetInstance()->PlayBGM("bgmmii");
		buttonObj[2]->SetText("SKILL TREE");

		buttonObj[1]->SetText("");

		buttonObj[0]->SetText("");

		levelUpScreen->SetPosition(Vector3(static_cast<float>(thePlayerInfo->GetMap()->getScreenWidth()) / 2, static_cast<float>(thePlayerInfo->GetMap()->getScreenHeight()) / 2, 0.0f));
		
		thePlayerInfo->setScreenState(SC_SKILL_TREE);
		break;
	case SC_SHOP:
		buttonObj[2]->SetText("Game Over?");

		buttonObj[1]->SetText("");

		buttonObj[0]->SetText("");

		thePlayerInfo->setScreenState(SC_SHOP);
		break;
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
	case SC_SHOP:
		return true;
		break;
	}
	return true;
}

void UserInterface::Render()// this is at the back since it needs to be on top? (eg. real time skill tree upgrading / shoping)
{
	switch(screen) {
	case SC_MAIN:
		if (KeyboardController::GetInstance()->IsKeyDown('F') || GamePadXbox::GetInstance()->IsKeyDown(GamePadXbox::GamePad_Button_START))
			titleScreen[1]->RenderUI();
		else
			titleScreen[0]->RenderUI();
		buttonObj[0]->RenderUI();
		buttonObj[1]->RenderUI();
		buttonObj[2]->RenderUI();
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
		xpBlock->RenderUI();
		textObj[0]->RenderUI();
		textObj[1]->RenderUI();
		return;
		break;
	case SC_PAUSE:
		buttonObj[0]->RenderUI();
		buttonObj[1]->RenderUI();
		buttonObj[2]->RenderUI();
		break;
	case SC_SKILL_TREE:
		buttonObj[0]->RenderUI();
		buttonObj[1]->RenderUI();
		buttonObj[2]->RenderUI();
		levelUpScreen->RenderUI();
		for (int i = 0; i < 13; ++i)
		{
			if (thePlayerInfo->getSkill(i) == true)
			{
				skillUnlockedFrames[i]->RenderUI();
			}
		}
		skillSelectedFrame->RenderUI();
		break;
	case SC_SHOP:
		buttonObj[0]->RenderUI();
		buttonObj[1]->RenderUI();
		buttonObj[2]->RenderUI();
		break;
	}
}