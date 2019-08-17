#include "UserInterface.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
// Currently able to select choices on main & pause screen, it does not render anything rn tho (req: Taga files &/ just some simple quads and text entities)
// Able to swap screens
using namespace std;
UserInterface::UserInterface()
	: choice(0)
	, maxChoices(3)
	, screen(SC_MAIN)
{
	scene2DQuad = Create::Sprite2DObject("UI_BOX",
		Vector3(120, 120, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	scene2DQuad2 = Create::Sprite2DObject("UI_BOX2",
		Vector3(120, 120, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	thePlayerInfo = CPlayerInfo2D::GetInstance();

	float fontSize = 16.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{//pointing at null...
		textObj[i] = Create::Text2DObject("text",
			Vector3(5, 5 + fontSize * i + halfFontSize, 0.0f),
			"",
			Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));
	}
	textObj[0]->SetText("Something");
	textObj[1]->SetText("Something2");
	textObj[2]->SetText("Something3");
}

UserInterface::~UserInterface()
{
	delete scene2DQuad;
	scene2DQuad = NULL;
	delete scene2DQuad2;
	scene2DQuad2 = NULL;
}

bool UserInterface::Update()
{
	switch (screen) {
	case SC_MAIN: // This is the starting screen
	{




		textObj[2]->SetText("Play");

		textObj[1]->SetText("Load");

		textObj[0]->SetText("Exit");

		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
		{
			choice = (choice + 1) % maxChoices;
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
		{
			if (--choice < 0)
			{
				choice = maxChoices - 1;
			}
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			switch (choice) {
			case 0:
				screen = SC_PLAY;
				return true;
				break;
			case 1:
				break;
			case 2:
				break;
			}
		}
		return false;
		break;
	}
	case SC_PAUSE: // This is the pause menu
	{


		textObj[2]->SetText("Continue");

		textObj[1]->SetText("Save");

		textObj[0]->SetText("Load");

		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
		{
			choice = (choice + 1) % maxChoices;
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
		{
			if (--choice < 0)
			{
				choice = maxChoices - 1;
			}
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			switch (choice) {
			case 0:
				screen = SC_PLAY;
				return true;
				break;
			case 1:
				break;
			case 2:
				break;
			}
		}
		return false;
		break;
	}
	case SC_PLAY: // This just checks for changes in UI* stuff while in play
	{


		std::ostringstream ss;
		ss.precision(5);
		textObj[2]->SetText(ss.str());

		ss.str("");
		ss.clear();
		ss << "CP: " << thePlayerInfo->checkPosition_X << ", " << thePlayerInfo->checkPosition_Y << endl
			<< "P: " << thePlayerInfo->position << endl;
		textObj[1]->SetText(ss.str());

		ss.str("");
		ss.clear();
		ss << "mapOffset_x: " << thePlayerInfo->mapOffset_x << endl;
		textObj[0]->SetText(ss.str());

		if (KeyboardController::GetInstance()->IsKeyPressed('P'))
		{
			screen = SC_PAUSE;
			choice = 0;
			maxChoices = 3;
			return false;
		}
		return true;
		break;
	}
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
	}
	return true;
}

void UserInterface::Render()// this is at the back since it needs to be on top? (eg. real time skill tree upgrading / shoping)
{
	switch(screen) {
	case SC_MAIN:
		scene2DQuad2->SetPosition(Vector3(30, 10, 0));
		scene2DQuad2->SetScale(Vector3(30, 10, 0));
		scene2DQuad2->RenderUI();
		textObj[0]->RenderUI();
		scene2DQuad2->SetPosition(Vector3(30, 30, 0));
		scene2DQuad2->RenderUI();
		textObj[1]->RenderUI();
		scene2DQuad2->SetPosition(Vector3(30, 50, 0));
		scene2DQuad2->RenderUI();
		textObj[2]->RenderUI();
		break;
	case SC_PLAY:
		for (int i = 0; i < thePlayerInfo->GetHp(); ++i) {
			scene2DQuad->SetPosition(Vector3(30*(i+1), 210, 0));
			scene2DQuad->RenderUI();
		}
		textObj[0]->RenderUI();
		textObj[1]->RenderUI();
		textObj[2]->RenderUI();
		return;
		break;
	case SC_PAUSE:
		scene2DQuad2->SetPosition(Vector3(160, 120, 0));
		scene2DQuad2->SetScale(Vector3(360, 240));
		scene2DQuad2->RenderUI();
		textObj[0]->RenderUI();
		textObj[1]->RenderUI();
		textObj[2]->RenderUI();
		break;
	}
}