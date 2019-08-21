#include "UserInterface.h"
#include "KeyboardController.h"

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
			Vector3(thePlayerInfo->GetStamina() / 2 + 18, 208.f, 0.0f),// Note : replace 0.5 with stamina????
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
	for (int i = 0; i < 3; ++i)
	{
		delete buttonObj[i];
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
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP) || KeyboardController::GetInstance()->IsKeyPressed('W'))
		{
			buttonObj[choice]->SetSelected(false);
			choice = (choice + 1) % maxChoices;
			buttonObj[choice]->SetSelected(true);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN) || KeyboardController::GetInstance()->IsKeyPressed('S'))
		{
			buttonObj[choice]->SetSelected(false);
			if (--choice < 0)
			{
				choice = maxChoices - 1;
			}
			buttonObj[choice]->SetSelected(true);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
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
	case SC_PAUSE: // This is the pause menu
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP) || KeyboardController::GetInstance()->IsKeyPressed('W'))
		{
			buttonObj[choice]->SetSelected(false);
			choice = (choice + 1) % maxChoices;
			buttonObj[choice]->SetSelected(true);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN) || KeyboardController::GetInstance()->IsKeyPressed('S'))
		{
			buttonObj[choice]->SetSelected(false);
			if (--choice < 0)
			{
				choice = maxChoices - 1;
			}
			buttonObj[choice]->SetSelected(true);
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
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
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT) || KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
		{
			screen = SC_SHOP;
			ChangeScreen(screen);
			return true;
		}
		if (!(thePlayerInfo->GetMap()->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X] == 30 ||
			thePlayerInfo->GetMap()->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X + 1] == 30))
		{
			screen = SC_PLAY;
			ChangeScreen(screen);
			return true;
		}
		return true;
		break;
	}
	case SC_SHOP:
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT) || KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
		{
			screen = SC_SKILL_TREE;
			ChangeScreen(screen);
			return true;
		}
		if (!(thePlayerInfo->GetMap()->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X] == 30 ||
			thePlayerInfo->GetMap()->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X + 1] == 30))
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
		if (thePlayerInfo->GetStamina() < 0.7f)
		{
			barStatus = 1;
			if (thePlayerInfo->GetStamina() < 0.4f)
				barStatus = 2;
		}
		staminaBar[barStatus]->SetScale(Vector3((thePlayerInfo->GetStamina()) * 25, staminaBar[barStatus]->GetScale().y, staminaBar[barStatus]->GetScale().z));
		staminaBar[barStatus]->SetPosition(Vector3(staminaBar[barStatus]->GetScale().x / 2 + 18, staminaBar[barStatus]->GetPosition().y, staminaBar[barStatus]->GetPosition().z));

		std::ostringstream ss;
		ss.precision(5);
		ss << "CP: " << thePlayerInfo->checkPosition_X << ", " << thePlayerInfo->checkPosition_Y << endl
			<< "P: " << thePlayerInfo->position << endl;
		textObj[1]->SetText(ss.str());

		ss.str("");
		ss.clear();
		ss << "mapOffset_x: " << thePlayerInfo->mapOffset_x << endl;
		textObj[0]->SetText(ss.str());

		if (KeyboardController::GetInstance()->IsKeyPressed('E') && (thePlayerInfo->GetMap()->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X]==30||
			thePlayerInfo->GetMap()->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X + 1] == 30))
		{
			screen = SC_SKILL_TREE;
			ChangeScreen(screen);
			choice = 2;
			maxChoices = 3;
			return false;
		}

		if (KeyboardController::GetInstance()->IsKeyPressed('P'))
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
	}
}

void UserInterface::ChangeScreen(SCREEN_TYPE screenType)
{
	switch (screenType) {
	case SC_MAIN:
		CSoundEngine::GetInstance()->PlayBGM("bgmrroll");
		buttonObj[2]->SetText("Play");

		buttonObj[1]->SetText("Load");

		buttonObj[0]->SetText("Exit");
		break;
	case SC_PLAY:
		CSoundEngine::GetInstance()->PlayBGM("bgm");
		break;
	case SC_PAUSE:
		CSoundEngine::GetInstance()->PlayBGM("bgmwalk");
		buttonObj[2]->SetText("Continue");

		buttonObj[1]->SetText("Save");

		buttonObj[0]->SetText("Load");
		break;
	case SC_SKILL_TREE:
		CSoundEngine::GetInstance()->PlayBGM("bgmmii");
		buttonObj[2]->SetText("SKILL TREE");

		buttonObj[1]->SetText("");

		buttonObj[0]->SetText("");
		break;
	case SC_SHOP:
		buttonObj[2]->SetText("SHOP");

		buttonObj[1]->SetText("");

		buttonObj[0]->SetText("");
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
		if (KeyboardController::GetInstance()->IsKeyDown('F'))
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
			heartEntity[theHeartInfo->GetFrameState()]->SetPosition(Vector3(24 * i + 64, 208, 0));
			heartEntity[theHeartInfo->GetFrameState()]->RenderUI();
		}
		staminaBar[barStatus]->RenderUI();
		staminaBattery->RenderUI();
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
		break;
	case SC_SHOP:
		buttonObj[0]->RenderUI();
		buttonObj[1]->RenderUI();
		buttonObj[2]->RenderUI();
		break;
	}
}