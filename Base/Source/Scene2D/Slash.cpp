#include "Slash.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Projectile.h"
#include "GoodiesFactory.h"
#include "../SoundEngine.h"

// Allocating and initializing Slash's static data member.  
// The pointer is allocated but not the object's constructor.
Slash *Slash::s_instance = 0;

Slash::Slash(void)
	: position(Vector3(0, 0, 8))
	, direction(NOPE)
{
}

Slash::~Slash(void)
{
}

// Initialise this class instance
void Slash::Init(void)
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
void Slash::SetPos(const Vector3& pos)
{
	position = pos;
}

Vector3 Slash::GetPos(void) const
{
	return position;
}

/********************************************************************************
 Update
 ********************************************************************************/
void Slash::Update(double dt)
{
	if (CPlayerInfo2D::GetInstance()->isAttacking())
	{
		if (direction == NOPE)
		{
			if (KeyboardController::GetInstance()->IsKeyDown('W'))
				direction = UP;
			else if (KeyboardController::GetInstance()->IsKeyDown('S') && !CPlayerInfo2D::GetInstance()->isOnGround())
				direction = DOWN;
			else if (CPlayerInfo2D::GetInstance()->isFacingRight())
				direction = RIGHT;
			else
				direction = LEFT;
		}
		position = CPlayerInfo2D::GetInstance()->GetPos();
		position.x -= CPlayerInfo2D::GetInstance()->GetMapOffset_x();
		switch (direction)
		{
		case UP:
			SetAnimationStatus(S_U1);
			position.y += 12;
			break;
			
		case DOWN:
			SetAnimationStatus(S_D1);
			position.y -= 12;
			break;
		case RIGHT:
			SetAnimationStatus(S_R1_1);
			position.x += 12;
			break;
		case LEFT:
			SetAnimationStatus(S_L1_1);
			position.x -= 12;
			break;
		default:
			break;
		}
		UpdateAnimationIndex(0.5f);
	}
	else
	{
		direction = NOPE;
		SetAnimationStatus(S_NOPE);
	}
}