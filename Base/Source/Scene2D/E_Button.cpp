#include "E_Button.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "GamePadXbox.h"
#include "Mtx44.h"
#include "GoodiesFactory.h"

// Allocating and initializing E_Button's static data member.  
// The pointer is allocated but not the object's constructor.
E_Button *E_Button::s_instance = 0;

E_Button::E_Button(void)
	: position(Vector3(0, 0, 8))
{
}

E_Button::~E_Button(void)
{
}

// Initialise this class instance
void E_Button::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 8);

	// Set the current values
	position.Set(0, 0, 8);

	// Set default tile sizes
	tileSize_Width = 16;
	tileSize_Height = 16;
}

// Set position
void E_Button::SetPos(const Vector3& pos)
{
	position = pos;
}

Vector3 E_Button::GetPos(void) const
{
	return position;
}

/********************************************************************************
 Update
 ********************************************************************************/
void E_Button::Update(double dt)
{
	if (CPlayerInfo2D::GetInstance()->GetMap()->theScreenMap[CPlayerInfo2D::GetInstance()->checkPosition_Y][CPlayerInfo2D::GetInstance()->checkPosition_X] == 30 ||
		(CPlayerInfo2D::GetInstance()->checkPosition_X + 1 < CPlayerInfo2D::GetInstance()->GetMap()->GetNumOfTiles_Width() && CPlayerInfo2D::GetInstance()->GetMap()->theScreenMap[CPlayerInfo2D::GetInstance()->checkPosition_Y][CPlayerInfo2D::GetInstance()->checkPosition_X + 1] == 30))
	{
		if (GetFrameState() == B_TOTAL)
		{
			if (GamePadXbox::GetInstance()->is_connected())
				SetAnimationStatus(B_3);
			else
				SetAnimationStatus(B_1);
		}
		UpdateAnimationIndex(0.3f);
		position = CPlayerInfo2D::GetInstance()->GetPos();
		position.y += 16.f;
	}
	else
	{
		SetAnimationStatus(B_TOTAL);
	}
}