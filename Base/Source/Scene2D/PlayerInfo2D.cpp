#include "PlayerInfo2D.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Projectile.h"
#include "GoodiesFactory.h"
#include "../SoundEngine.h"
#include "Slash.h"

// Allocating and initializing CPlayerInfo2D's static data member.
// The pointer is allocated but not the object's constructor.
CPlayerInfo2D *CPlayerInfo2D::s_instance = 0;

CPlayerInfo2D::CPlayerInfo2D(void)
	: m_dSpeed(4.0)
	, m_dMoveSpeed(0.6)
	, m_dRollSpeed(0.8)
	, m_dAcceleration(8.0)
	, m_bJumpUpwards(false)
	, m_bJumped(false)
	, m_dJumpSpeed(6.0)
	, m_dJumpAcceleration(-8.0)
	, m_bJumpKeyHeld(false)
	, m_bDoubleJump(false)
	, m_bDoubleJumped(false)
	, m_bFallDownwards(false)
	, m_dFallSpeed(0.0)
	, m_dFallAcceleration(-4.0)
	, m_dElapsedTime(0.0)
	, hp(1)
	, maxhp(1)
	, mapOffset_x(0)
	, mapOffset_y(0)
	, tileOffset_x(0)
	, tileOffset_y(0)
	, mapFineOffset_x(0)
	, mapFineOffset_y(0)
	, theMapReference(NULL)
	, theRearMapReference(NULL)
	, rearTileOffset_x(0)
	, rearTileOffset_y(0)
	, rearMapOffset_x(0)
	, rearMapOffset_y(0)
	, rearMapFineOffset_x(0)
	, rearMapFineOffset_y(0)
	, attackBounceTime(1)
	, rollBounceTime(1)
	, dashBounceTime(1)
	, damageBounceTime(1)
	, attackBounceTimeLimit(0.5)
	, rollBounceTimeLimit(0.5)
	, rollBounceTimeLimit2(0.7)
	, dashBounceTimeLimit(0.9)
	, damageBounceTimeLimit(1)
	, stamina(1.f)
	, secondAttack(false)
	, dashPower(0.f)
	, chargeAttack(0.f)
	, chargeTime(5.f)
	, XP(0)
	, maxXP(10)
	, level(0)
	, lifesteal(0)
	, lifestealLimit(10)
{
}

CPlayerInfo2D::~CPlayerInfo2D(void)
{
}

// Initialise this class instance
void CPlayerInfo2D::Init(void)
{
	// Set the default values
	defaultPosition.Set(0,0,0);
	defaultTarget.Set(0,0,0);
	defaultUp.Set(0,1,0);

	// Set the current values
	position.Set(0, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);

	// Set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	// Set default tile sizes
	tileSize_Width = 16;
	tileSize_Height = 16;

	InitSound();
	InitSkill();
}

// Set the boundary for the player info
void CPlayerInfo2D::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Set the tile size
void CPlayerInfo2D::SetTileSize(const int x, const int y)
{
	tileSize_Width = x;
	tileSize_Height = y;
}

// Set the map pointer to this class
void CPlayerInfo2D::SetMap(CMap* m_cMap)
{
	theMapReference = m_cMap;
}

CMap * CPlayerInfo2D::GetMap(void)
{
	return theMapReference;
}

// Returns true if the player is on ground
bool CPlayerInfo2D::isOnGround(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == false)
	{
		int checkPosition_X = (int)((position.x - (tileSize_Width >> 1)) / tileSize_Width);
		int checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
			(int)ceil(position.y / tileSize_Height);

		if (((int)(position.x - (tileSize_Width >> 1)) % tileSize_Width) == 0)
		{
			if (theMapReference->theScreenMap[checkPosition_Y + 1][checkPosition_X] >= 20 && theMapReference->theScreenMap[checkPosition_Y + 1][checkPosition_X] <= 23)
				TakeDamage();
		}
		else
		{
			if (theMapReference->theScreenMap[checkPosition_Y + 1][checkPosition_X] >= 20 &&
				theMapReference->theScreenMap[checkPosition_Y + 1][checkPosition_X] <= 23 ||
				theMapReference->theScreenMap[checkPosition_Y + 1][checkPosition_X + 1] >= 20 &&
				theMapReference->theScreenMap[checkPosition_Y + 1][checkPosition_X + 1] <= 23)
				TakeDamage();
		}
		return true;
	}

	return false;
}

// Returns true if the player is jumping upwards
bool CPlayerInfo2D::isJumpUpwards(void)
{
	if (m_bJumpUpwards == true && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is on freefall
bool CPlayerInfo2D::isFreeFall(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == true)
		return true;

	return false;
}

// Set the player's status to free fall mode
void CPlayerInfo2D::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		m_bJumpUpwards = false;
		m_bFallDownwards = true;
		m_dFallSpeed = 0.0;
	}
	if (!isRolling())
	{
		if (!isPogo())
		{
			if (isFacingRight())
				SetAnimationStatus(CAnimation::P_FALL_R1);
			else
				SetAnimationStatus(CAnimation::P_FALL_L1);
		}
	}
	UpdateAnimationIndex(1.5f);
}

// Set the player to jumping upwards
void CPlayerInfo2D::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		m_bJumpUpwards = true;
		m_bFallDownwards = false;
		if (skill[SK_HIGH_JUMP])
			m_dJumpSpeed = 8.0;
		else
			m_dJumpSpeed = 6.0;

		if (isFacingRight())
			SetAnimationStatus(CAnimation::P_JUMP_R1);
		else
			SetAnimationStatus(CAnimation::P_JUMP_L1);
		UpdateAnimationIndex(1.f);
		CSoundEngine::GetInstance()->PlayASound("jump");
	}
}

// Set position
void CPlayerInfo2D::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CPlayerInfo2D::SetTarget(const Vector3& target)
{
	this->target = target;
}

// Set position
void CPlayerInfo2D::SetUp(const Vector3& up)
{
	this->up = up;
}

void CPlayerInfo2D::SetHp(const int hp)
{
	this->hp = hp;
}

void CPlayerInfo2D::SetMaxHp(const int hp)
{
	this->maxhp = hp;
}

bool CPlayerInfo2D::TakeDamage(void)
{
	if (damageBounceTime > damageBounceTimeLimit)
	{
		--hp;
		CSoundEngine::GetInstance()->PlayASound("damaged");
		if (hp <= 0)
			Die();
		damageBounceTime = 0.f;
		return true;
	}
	return false;
}

// Set m_dJumpAcceleration of the player
void CPlayerInfo2D::SetJumpAcceleration(const double m_dJumpAcceleration)
{
	this->m_dJumpAcceleration = m_dJumpAcceleration;
}

// Set Fall Acceleration of the player
void CPlayerInfo2D::SetFallAcceleration(const double m_dFallAcceleration)
{
	this->m_dFallAcceleration = m_dFallAcceleration;
}

// Stop the player's movement
void CPlayerInfo2D::StopVerticalMovement(void)
{
	m_bJumpUpwards = false;
	m_bFallDownwards = false; 
	StepSound();
}

// Reset this player instance to default
void CPlayerInfo2D::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;

	// Stop vertical movement too
	StopVerticalMovement();
}

// Get position x of the player
Vector3 CPlayerInfo2D::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CPlayerInfo2D::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CPlayerInfo2D::GetUp(void) const
{
	return up;
}

int CPlayerInfo2D::GetHp(void) const
{
	return hp;
}

int CPlayerInfo2D::GetMaxHp(void) const
{
	return maxhp;
}

// Get m_dJumpAcceleration of the player
double CPlayerInfo2D::GetJumpAcceleration(void) const
{
	return m_dJumpAcceleration;
}

float CPlayerInfo2D::GetStamina(void) const
{
	return stamina;
}

double CPlayerInfo2D::GetXP(void) const
{
	return XP;
}

void CPlayerInfo2D::AddXP(double xp)
{
	XP += xp;
	XPLevelUp();
}

void CPlayerInfo2D::XPLevelUp(void)
{
	if (XP >= maxXP)
	{
		XP /= maxXP;
		++level;
	}
}

double CPlayerInfo2D::GetLifesteal(void) const
{
	return lifesteal;
}

void CPlayerInfo2D::AddLifesteal(int lifesteal)
{
	if (skill[SK_LIFESTEAL] && hp < maxhp)
	{
		this->lifesteal += lifesteal;
		LifestealLifeUp();
	}
	else
		lifesteal = 0;
}

void CPlayerInfo2D::LifestealLifeUp(void)
{
	if (lifesteal >= lifestealLimit)
	{
		lifesteal /= lifestealLimit;
		Heal(true, 1);
	}
}

double CPlayerInfo2D::GetLevel(void) const
{
	return level;
}

// Set Tile Offset for x-axis
int CPlayerInfo2D::GetTileOffset_x(void) const
{
	return tileOffset_x;
}

// Get Max Boundary
Vector3 CPlayerInfo2D::GetMaxBoundary() const
{
	return maxBoundary;
}
// Get Min Boundary
Vector3 CPlayerInfo2D::GetMinBoundary() const
{
	return minBoundary;
}

// Update Jump Upwards
void CPlayerInfo2D::UpdateJumpUpwards(double dt)
{
	if (m_bJumpUpwards == false)
		return;
	if (m_bDoubleJump && !m_bDoubleJumped)
	{
		m_dJumpSpeed = 6.0f; // Double Jump is normal height even if skill[SK_HIGH_JUMP]
		m_bDoubleJumped = true;
	}

	// Record current position before jump
	int currentPosition_Y = theMapReference->GetNumOfTiles_Height() - (int)ceil((float)position.y / tileSize_Height);

	// Update the jump
	position.y += static_cast<float>(m_dJumpSpeed);
	m_dJumpSpeed -= 0.5f;
	if (m_dJumpSpeed <= 0)
		SetOnFreeFall(true);

	// If the player has jumped out of the screen, 
	// then start free fall and lock loaction
	if (position.y + tileSize_Height > theMapReference->GetNumOfTiles_Height()*theMapReference->GetTileSize_Height())
	{
		SetOnFreeFall(true);
		position.y = static_cast<float>(theMapReference->GetNumOfTiles_Height()*theMapReference->GetTileSize_Height() - tileSize_Height);
		return;
	}
	// Check if the player is stopped by obstacles
	int checkPosition_X = (int)((position.x - (tileSize_Width >> 1)) / tileSize_Width);
	int checkPosition_Y = theMapReference->GetNumOfTiles_Height() - 
						  (int)ceil((float)(position.y + (tileSize_Height >> 1)) / tileSize_Height);
	for (int i = checkPosition_Y; i <= currentPosition_Y; i++)
	{
		if (((int)(position.x - (tileSize_Width >> 1)) % tileSize_Width) == 0)
		{
			if (theMapReference->theScreenMap[i][checkPosition_X] == 1 || 
				theMapReference->theScreenMap[i][checkPosition_X]>=200 && theMapReference->theScreenMap[i][checkPosition_X]<=203)
			{
				if (theMapReference->theScreenMap[i][checkPosition_X] != 1)
				{
					TakeDamage();
				}
				// Since the new position does not allow the player to move into, then go back to the old position
				position.y = static_cast<float>((theMapReference->GetNumOfTiles_Height() - i - 1) * tileSize_Height - (tileSize_Height >> 1));
				// Set on free fall
				SetOnFreeFall(true);
				break;
			}
		}
		else
		{
			if ((theMapReference->theScreenMap[i][checkPosition_X] == 1) ||
				(theMapReference->theScreenMap[i][checkPosition_X + 1] == 1)||
				theMapReference->theScreenMap[i][checkPosition_X] >= 20 && theMapReference->theScreenMap[i][checkPosition_X] <=23 ||
				theMapReference->theScreenMap[i][checkPosition_X + 1] >= 20 && theMapReference->theScreenMap[i][checkPosition_X + 1] <=23)
			{
				if((theMapReference->theScreenMap[i][checkPosition_X] != 1) &&
					(theMapReference->theScreenMap[i][checkPosition_X + 1] != 1))
				{
					TakeDamage();
				}
				// Since the new position does not allow the player to move into, then go back to the old position
				position.y = static_cast<float>((theMapReference->GetNumOfTiles_Height() - i - 1) * tileSize_Height - (tileSize_Height >> 1));
				// Set on free fall
				SetOnFreeFall(true);
				break;
			}
		}
	}
}

// Update FreeFall
void CPlayerInfo2D::UpdateFreeFall(double dt)
{
	if (m_bFallDownwards == false)
		return;

	// Record current position before fall
	int currentPosition_Y = theMapReference->GetNumOfTiles_Height() - (int)ceil((float)position.y / tileSize_Height);

	// Update the free fall
	position.y -= static_cast<float>(m_dFallSpeed);
	if (m_dFallSpeed <= 6)
	m_dFallSpeed += 0.5;
	if (position.y - tileSize_Height < tileSize_Height/2 - 2)
	{
		StopVerticalMovement();// Note : Dies/-1 hp
		position.y = static_cast<float>(tileSize_Height + tileSize_Height/2);
		return;
	}
	// Check if the player is still in mid air...
	int checkPosition_X = (int)((position.x - (tileSize_Width >> 1)) / tileSize_Width);
	int checkPosition_Y = theMapReference->GetNumOfTiles_Height() - 
							(int)ceil((float)(position.y - m_dFallSpeed) / tileSize_Height);
	for (int i = currentPosition_Y; i <= checkPosition_Y; i++)
	{
		if (((int)(position.x - (tileSize_Width>>1)) % tileSize_Width) == 0)
		{
			if (theMapReference->theScreenMap[i][checkPosition_X] == 1 ||
				theMapReference->theScreenMap[i][checkPosition_X] >= 20 && theMapReference->theScreenMap[i][checkPosition_X] <=23)
			{
				if(theMapReference->theScreenMap[i][checkPosition_X] != 1)
				{
					TakeDamage();
				}
				// Since the new position does not allow the player to move into, then go back to the old position
				position.y = static_cast<float>((theMapReference->GetNumOfTiles_Height() - i) * tileSize_Height + (tileSize_Height >> 1));
				// Stop all vertical movement
				StopVerticalMovement();
				m_bJumped = false;
				m_bDoubleJump = false;
				m_bDoubleJumped = false;
				break;
			}
		}
		else
		{
			if ((theMapReference->theScreenMap[i][checkPosition_X] == 1) ||
				(theMapReference->theScreenMap[i][checkPosition_X + 1] == 1) ||
				theMapReference->theScreenMap[i][checkPosition_X] >= 20 && theMapReference->theScreenMap[i][checkPosition_X] <=23 ||
				theMapReference->theScreenMap[i][checkPosition_X + 1] >= 20 && theMapReference->theScreenMap[i][checkPosition_X + 1] <=23)
			{
				if ((theMapReference->theScreenMap[i][checkPosition_X] != 1) &&
					(theMapReference->theScreenMap[i][checkPosition_X + 1] != 1))
				{
					TakeDamage();
				}
				// Since the new position does not allow the player to move into, then go back to the old position
				position.y = static_cast<float>((theMapReference->GetNumOfTiles_Height() - i) * tileSize_Height + (tileSize_Height >> 1));
				// Stop all vertical movement
				StopVerticalMovement();
				m_bJumped = false;
				m_bDoubleJump = false;
				m_bDoubleJumped = false;
				break;
			}
		}
	}
}

/********************************************************************************
 Update
 ********************************************************************************/
void CPlayerInfo2D::Update(double dt)
{
	attackBounceTime += dt;
	rollBounceTime += dt;
	dashBounceTime += dt;
	damageBounceTime += dt;
	// Update the player position
	//if (KeyboardController::GetInstance()->IsKeyDown('W'))
	//	MoveUpDown(true, 1.0f);
	//if (KeyboardController::GetInstance()->IsKeyDown('S'))
	//	MoveUpDown(false, 1.0f);
	if (!isDie())
	{
		if (dashPower && !KeyboardController::GetInstance()->IsKeyDown('K'))
		{
			chargeAttack = 0.f;
			MoveLeftRight(!isFacingRight(), 3.f);
			dashPower -= static_cast<float>(dt * 10.0f);
			if (dashPower <= 0.f)
			{
				dashPower = 0.f;
				Attack(!isFacingRight(), 0.5f);
			}
		}
		else if (isCharged() && KeyboardController::GetInstance()->IsKeyReleased('K'))
		{
			Attack(!isFacingRight(), 0.5f);
		}
		else if (isRolling()
			&& (isOnGround() || skill[SK_AIR_ROLL]))
		{
			if (skill[SK_FAST_ROLL])
				MoveLeftRight(!isFacingRight(), static_cast<float>(m_dRollSpeed));
			else
				MoveLeftRight(!isFacingRight(), static_cast<float>(m_dMoveSpeed));
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed('L')
			&& (rollBounceTime > rollBounceTimeLimit2 && skill[SK_ROLL_COST] || rollBounceTime > rollBounceTimeLimit)
			&& !isAttacking()
			&& (isOnGround() || skill[SK_AIR_ROLL]))
		{
			bool direction = !isFacingRight();
			if (KeyboardController::GetInstance()->IsKeyDown('A'))
				direction = true;
			else if (KeyboardController::GetInstance()->IsKeyDown('D'))
				direction = false;
			if (skill[SK_FAST_ROLL])
				MoveLeftRight(direction, static_cast<float>(m_dRollSpeed));
			else
				MoveLeftRight(direction, static_cast<float>(m_dMoveSpeed));
		}
		else if (KeyboardController::GetInstance()->IsKeyDown('A')
			&& !KeyboardController::GetInstance()->IsKeyDown('D')
			&& !KeyboardController::GetInstance()->IsKeyPressed('J')
			&& !isPogo()) // Move Left
		{
			MoveLeftRight(true, static_cast<float>(m_dMoveSpeed));
			StaminaRegen(0.1f, dt);
		}
		else if (KeyboardController::GetInstance()->IsKeyDown('D')
			&& !KeyboardController::GetInstance()->IsKeyDown('A')
			&& !KeyboardController::GetInstance()->IsKeyPressed('J')
			&& !isPogo()) // Move Right
		{
			MoveLeftRight(false, static_cast<float>(m_dMoveSpeed));
			StaminaRegen(0.1f, dt);
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed('J')
			&& KeyboardController::GetInstance()->IsKeyDown('W') || KeyboardController::GetInstance()->IsKeyPressed('J')
			&& KeyboardController::GetInstance()->IsKeyDown('S')
			&& !KeyboardController::GetInstance()->IsKeyDown('K')
			&& !isOnGround())
		{
			Attack((!isFacingRight()), 0.5f);
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed('J')
			&& KeyboardController::GetInstance()->IsKeyDown('A')
			&& !KeyboardController::GetInstance()->IsKeyDown('K')
			&& !isAttacking()) // Attack Left
		{
			Attack(true, 0.5f);
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed('J')
			&& KeyboardController::GetInstance()->IsKeyDown('D')
			&& !KeyboardController::GetInstance()->IsKeyDown('K')
			&& !isAttacking()) // Attack Right
		{
			Attack(false, 0.5f);
		}
		else if (KeyboardController::GetInstance()->IsKeyPressed('J')
			&& !KeyboardController::GetInstance()->IsKeyDown('K'))
		{
			Attack(!isFacingRight(), 0.5f);
		}
		else if (isAttacking())
		{
			UpdateAnimationIndex(1.f);
		}
		else if (isOnGround()) // Idle
		{
			if (isCharged())
			{
				if (isFacingRight())
					SetAnimationStatus(CAnimation::P_CHARGE_R);
				else
					SetAnimationStatus(CAnimation::P_CHARGE_L);
			}
			else
			{
				if (isFacingRight())
					SetAnimationStatus(CAnimation::P_IDLE_R1);
				else
					SetAnimationStatus(CAnimation::P_IDLE_L1);
			}

			StaminaRegen(0.3f, dt);

			UpdateAnimationIndex(0.1f);
		}
		else
		{
			UpdateAnimationIndex(1.f);
		}

		if (KeyboardController::GetInstance()->IsKeyDown('K') && isOnGround())
		{
			if (skill[SK_CHARGE_ATTACK])
				chargeAttack += static_cast<float>(10 * dt);
			if (isCharged())
			{
				dashPower = 0.f;
			}
			else if (skill[SK_DASH_ATTACK]
				&& !dashPower
				&& dashBounceTime > dashBounceTimeLimit)
			{
				if (StaminaDecrease(0.4f))
					dashPower = 0.7f;
			}
		}

		if (dashPower && KeyboardController::GetInstance()->IsKeyReleased('K'))
			dashBounceTime = 0.f;

		if (position.x + (tileSize_Width >> 1) > theMapReference->getNumOfTiles_MapWidth() * theMapReference->GetTileSize_Width())
			position.x = static_cast<float>(theMapReference->getNumOfTiles_MapWidth() * theMapReference->GetTileSize_Width() - (tileSize_Width >> 1));
		if (position.x - (tileSize_Width >> 1) < 0)
			position.x = static_cast<float>(tileSize_Width >> 1);

		if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE) && !m_bJumped && isOnAir() && !m_bDoubleJump && isRolling() && !isAttacking() && skill[SK_DOUBLE_JUMP])
		{
			m_bJumped = true;
			m_bDoubleJump = true;
			SetToJumpUpwards(true);
		}

		if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
		{
			m_bJumpKeyHeld = false;
		}

		if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE) && !m_bJumpKeyHeld && !m_bDoubleJump && m_bJumped && !isRolling() && !isAttacking() && skill[SK_DOUBLE_JUMP])
		{
			m_bJumpKeyHeld = true;
			m_bDoubleJump = true;
			SetToJumpUpwards(true);
		}

		// If the user presses SPACEBAR, then make him jump
		if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE) && !m_bJumped && !isOnAir() && !isRolling() && !isAttacking())
		{
			m_bJumped = true;
			m_bJumpKeyHeld = true;
			SetToJumpUpwards(true);
		}
		else
		{
			// Check if the player has walked off the platform
			if (isOnAir())
			{
				SetOnFreeFall(true);
			}
		}
		if (KeyboardController::GetInstance()->IsKeyPressed('K') && !isOnGround())
		{
			if (isFacingRight())
				SetAnimationStatus(CAnimation::P_POGO_R1);
			else
				SetAnimationStatus(CAnimation::P_POGO_L1);
		}
	}
	else
	{
		UpdateAnimationIndex(0.2f);
	}

	// Constrain the position
	Constrain();


	// If the user presses M key, then reset the view to default values
	if (KeyboardController::GetInstance()->IsKeyDown('M'))
	{
		//Reset();
	}
	else if (KeyboardController::GetInstance()->IsKeyReleased(VK_F5))
	{
		Load();
	}
	else if (KeyboardController::GetInstance()->IsKeyReleased(VK_F6))
	{
		if (Save() == true)
			cout << "Save to file is successful!" << endl;
		else
			cout << "Save to file is unsuccessful!" << endl;
	}
	else
	{
		UpdateJumpUpwards(dt);
		UpdateFreeFall(dt);
		UpdateTileOffset();
	}

	// Check for collision with CGoodies instances

	// Calculate the tile indices for the PlayerInfo2D's latest position
	checkPosition_X = (int)((position.x - (tileSize_Width >> 1)) / tileSize_Width);
	checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
		(int)ceil(position.y / theMapReference->GetTileSize_Height());

	UpdateGoodies(checkPosition_X, checkPosition_Y);
}

// Update tile offsets
void CPlayerInfo2D::UpdateTileOffset(void)
{
	tileOffset_x = (int)(mapOffset_x / theMapReference->GetTileSize_Width());
	//if (tileOffset_x + theMapReference->GetNumOfTiles_Width() > theMapReference->getNumOfTiles_MapWidth()) // idk what this does
	//	tileOffset_x = theMapReference->getNumOfTiles_MapWidth() - theMapReference->GetNumOfTiles_Width();
}

// Update side movements
void CPlayerInfo2D::UpdateSideMovements(void)
{
	// Calculate the position to check on, using the player's reference point
	checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
		(int)ceil(position.y / theMapReference->GetTileSize_Height());

	// Check if the hero can move sideways
	if (KeyboardController::GetInstance()->IsKeyPressed('L')
		&& (isOnGround() || skill[SK_AIR_ROLL])
		&& (KeyboardController::GetInstance()->IsKeyDown('A') || !isFacingRight()
		&& !KeyboardController::GetInstance()->IsKeyDown('D')) || isRolling()
		&& !isFacingRight())
	{
		// Find the tile number which the player's left side is on
		checkPosition_X = (int)((position.x - (tileSize_Width >> 1)) / tileSize_Width);
		if (KeyboardController::GetInstance()->IsKeyPressed('L') && (rollBounceTime > rollBounceTimeLimit2 && skill[SK_ROLL_COST] || rollBounceTime > rollBounceTimeLimit))
		{
			if (Roll())
				SetAnimationStatus(CAnimation::P_ROLL_L1);
		}
		if (checkPosition_X >= 0)
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1 ||
				theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] >= 20 && theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] <=23)
			{
				position.x = static_cast<float>((checkPosition_X + 1) * tileSize_Width + (tileSize_Width >> 1));
				if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] != 1)
				{
					TakeDamage();
					//position.x += 8;
				}
			}
		}
	}
	else if (KeyboardController::GetInstance()->IsKeyPressed('L')
			&& (isOnGround() || skill[SK_AIR_ROLL])
			&& (KeyboardController::GetInstance()->IsKeyDown('D') || isFacingRight()
			&& !KeyboardController::GetInstance()->IsKeyDown('A')) || isRolling()
			&& isFacingRight())
	{
		// Find the tile number which the player's right side is on
		checkPosition_X = (int)((position.x + (tileSize_Width >> 1)) / tileSize_Width);
		if (KeyboardController::GetInstance()->IsKeyPressed('L') && (rollBounceTime > rollBounceTimeLimit2 && skill[SK_ROLL_COST] || rollBounceTime > rollBounceTimeLimit))
		{
			if (Roll())
				SetAnimationStatus(CAnimation::P_ROLL_R1);
		}

		if (checkPosition_X < theMapReference->getNumOfTiles_MapWidth())
		{

			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1 ||
				theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] >= 20 && theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] <=23)
			{
				// this part causes the player to be stuck when there is a tile on its right
				position.x = static_cast<float>((checkPosition_X - 1) * tileSize_Width + (tileSize_Width >> 1));
				if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] != 1)
				{
					TakeDamage();
					//position.x -= 8;
				}
			}

		}
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('A') || dashPower && !isFacingRight())
	{
		// Find the tile number which the player's left side is on
		checkPosition_X = (int)((position.x - (tileSize_Width >> 1)) / tileSize_Width);
		if (isOnGround() && !isAttacking())
			SetAnimationStatus(CAnimation::P_RUN_L1);

		if (checkPosition_X >= 0)
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1 ||
				theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] >= 20 && theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] <=23)
			{
				position.x = static_cast<float>((checkPosition_X + 1) * tileSize_Width + (tileSize_Width >> 1));
				if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] != 1)
				{
					TakeDamage();
					//position.x += 8;
				}
			}
		}
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('D') || dashPower && isFacingRight())
	{
		// Find the tile number which the player's right side is on
		checkPosition_X = (int)((position.x + (tileSize_Width >> 1)) / tileSize_Width);
		if (isOnGround() && !isAttacking())
			SetAnimationStatus(CAnimation::P_RUN_R1);

		if (checkPosition_X < theMapReference->GetNumOfTiles_Width())
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1 ||
				theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] >= 20 && theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] <=23)
			{
				position.x = static_cast<float>((checkPosition_X - 1) * tileSize_Width + (tileSize_Width >> 1));
				if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] != 1)
				{
					TakeDamage();
					//position.x -= 8;
				}
			}
		}
	}
}

/********************************************************************************
Move Up Down
********************************************************************************/
void CPlayerInfo2D::MoveUpDown(const bool mode, const float timeDiff)
{
	if (mode)
	{
		position.y = position.y + (int)(m_dSpeed * timeDiff);
	}
	else
	{
		position.y = position.y - (int)(m_dSpeed * timeDiff);
	}
}

/********************************************************************************
Move Left Right
********************************************************************************/
void CPlayerInfo2D::MoveLeftRight(const bool mode, const float timeDiff)
{
	if (mode)
	{
		position.x = position.x - (int)(m_dSpeed * timeDiff);
	}
	else
	{
		position.x = position.x + (int)(m_dSpeed * timeDiff);
	}

	UpdateSideMovements();
	UpdateAnimationIndex(timeDiff);

}

void CPlayerInfo2D::Attack(const bool mode, const float timeDiff)
{
	if (attackBounceTime > attackBounceTimeLimit)
	{
		if (!KeyboardController::GetInstance()->IsKeyDown('W') && !KeyboardController::GetInstance()->IsKeyDown('S'))
			secondAttack = true;
		if (!isAttacking())
		{
			if (isCharged())
			{
				if (mode)
					SetAnimationStatus(CAnimation::P_CHARGE_ATTACK_L1);
				else
					SetAnimationStatus(CAnimation::P_CHARGE_ATTACK_R1);
				chargeAttack = 0.f;
			}
			else
			{
				if (mode)
					SetAnimationStatus(CAnimation::P_ATTACK_L1);
				else
					SetAnimationStatus(CAnimation::P_ATTACK_R1);
			}
			AttackSound();
			UpdateAnimationIndex(timeDiff);
			attackBounceTime = 0.f;
		}
	}
	else if (secondAttack && skill[SK_DOUBLE_ATTACK])
	{
		secondAttack = false;
		if (!isAttacking())
		{
			if (mode)
				SetAnimationStatus(CAnimation::P_ATTACK_L1);
			else
				SetAnimationStatus(CAnimation::P_ATTACK_R1);
			AttackSound();
			UpdateAnimationIndex(timeDiff);
			attackBounceTime = 0.f;
		}
	}
}

void CPlayerInfo2D::Die()
{
	CSoundEngine::GetInstance()->PlayASound("death");
	if (isFacingRight())
		SetAnimationStatus(P_DIE_R1);
	else
		SetAnimationStatus(P_DIE_L1);
}

void CPlayerInfo2D::Heal(bool sound, int life)
{
	if (skill[SK_HEART3])
		maxhp = 3;
	else if (skill[SK_HEART2])
		maxhp = 2;
	else
		maxhp = 1;

	if (hp + life > maxhp)
		hp = maxhp;
	else
		hp += life;
	if (sound)
		CSoundEngine::GetInstance()->PlayASound("heal");
}

bool CPlayerInfo2D::isCharged(void) const
{
	return chargeAttack > chargeTime;
}

// Check if the player is standing on air
bool CPlayerInfo2D::isOnAir(void)
{
	if ((m_bJumpUpwards == true) || (m_bFallDownwards == true))
		return false;

	// Check if the player is still in mid air...
	int checkPosition_X = (int)((position.x - (tileSize_Width >> 1)) / tileSize_Width);
	int checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
		(int)ceil(position.y / tileSize_Height);

	if (((int)(position.x - (tileSize_Width >> 1)) % tileSize_Width) == 0)
	{
		if (theMapReference->theScreenMap[checkPosition_Y + 1][checkPosition_X] == 0)
		{
				m_bJumped = true;// Counts as jump when falling?
			if (!(position.y - tileSize_Height < tileSize_Height))
			{
				m_bDoubleJump = false;
			}
			return true;
		}
	}
	else
	{
		if ((theMapReference->theScreenMap[checkPosition_Y + 1][checkPosition_X] == 0)
			&& (theMapReference->theScreenMap[checkPosition_Y + 1][checkPosition_X + 1] == 0))
		{
				m_bJumped = true;
			if (!(position.y - tileSize_Height < tileSize_Height ))
			{
				m_bDoubleJump = false;
			}
			return true;
		}
	}
	m_bJumped = false;
	m_bDoubleJump = false;
	m_bDoubleJumped = false;
	return false;
}

// Constrain the position within the borders
void CPlayerInfo2D::Constrain(void)
{
	// Constrain player within the boundary
	//if (position.x >= maxBoundary.x + mapOffset_x - (tileSize_Width >> 1))
	//{
	//	// 0.325 ~ 0.675 = 0.25 of screen
	//	//mapOffset_x += m_dSpeed * (m_dMoveSpeed + ((position.x - maxBoundary.x - mapOffset_x) / (maxBoundary.x * 0.25f) * (stamina - m_dMoveSpeed)));
	//	if (position.x >= maxBoundary.x * 1.25f + mapOffset_x)
	//		mapOffset_x += m_dSpeed * m_dMoveSpeed;// this part is still a fail-safe
	//	else
	//	{
	//		if (rollBounceTime < rollBounceTimeLimit)
	//		{
	//			mapOffset_x += m_dSpeed * stamina;// Note : causes problem when rolling in place
	//		}
	//		else
	//			mapOffset_x += m_dSpeed * m_dMoveSpeed;
	//	}
	//	if (mapOffset_x + theMapReference->getScreenWidth() > theMapReference->GetNumOfTiles_Width() * theMapReference->GetTileSize_Width())
	//		mapOffset_x = theMapReference->GetNumOfTiles_Width() * theMapReference->GetTileSize_Width() - theMapReference->getScreenWidth();
	//}
	if (position.y > maxBoundary.y - tileSize_Height) // for y-scrolling
	{
		position.y = maxBoundary.y - (tileSize_Height >> 1);
	}
	//if (position.x <= minBoundary.x + mapOffset_x)
	//{
	//	if (position.x <= minBoundary.x * 0.75f + mapOffset_x)
	//		mapOffset_x -= m_dSpeed * (m_dMoveSpeed - 0.1f);
	//	else
	//	{
	//		if (rollBounceTime < rollBounceTimeLimit)
	//			mapOffset_x -= m_dSpeed * (stamina - 0.1f);
	//		else
	//			mapOffset_x -= m_dSpeed * (m_dMoveSpeed - 0.1f);
	//	}
	//	if (mapOffset_x < 0)
	//		mapOffset_x = 0;
	//}
	mapOffset_x = static_cast<int>(position.x - (tileSize_Width >> 1) - maxBoundary.x);// whatevbs.. idc anymore...
	if (mapOffset_x + theMapReference->getScreenWidth() > theMapReference->GetNumOfTiles_Width() * theMapReference->GetTileSize_Width())
			mapOffset_x = theMapReference->GetNumOfTiles_Width() * theMapReference->GetTileSize_Width() - theMapReference->getScreenWidth();
	if (mapOffset_x < 0)
		mapOffset_x = 0;

	if (position.y < minBoundary.y)
	{
		position.y = minBoundary.y + (tileSize_Height >> 1);
	}
}

// Set mapFineOffset_x values
void CPlayerInfo2D::UpdateMapFineOffset(void)
{
	mapFineOffset_x = mapOffset_x % theMapReference->GetTileSize_Width();
}

// Get mapFineOffset_x values
int CPlayerInfo2D::GetMapFineOffset_x(void) const
{
	return mapFineOffset_x;
}

int CPlayerInfo2D::GetMapOffset_x(void) const
{
	return mapOffset_x;
}

/*
For Parallax Scrolling
*/

// Set the rear map pointer to this class
void CPlayerInfo2D::SetRearMap(CMap* m_cRearMap)
{
	theRearMapReference = m_cRearMap;
}

// Update the rear map
void CPlayerInfo2D::UpdateRearMap(void)
{
	rearMapOffset_x = (int)(mapOffset_x / 2);
	rearTileOffset_x = (int)(rearMapOffset_x / theRearMapReference->GetTileSize_Width());
	rearMapFineOffset_x = rearMapOffset_x % theRearMapReference->GetTileSize_Width();
}

// Get Tile Offset for x-axis
int CPlayerInfo2D::GetRearTileOffset_x(void) const
{
	return rearTileOffset_x;
}

// Get the rear map fine offset X value
int  CPlayerInfo2D::GetRearMapFineOffset_x(void) const
{
	return rearMapFineOffset_x;
}

// Methods to tokenize a string into a specific data type variable
Vector3 CPlayerInfo2D::Token2Vector(string token)
{
	Vector3 tempVector(0.0f, 0.0f, 0.0f);

	std::istringstream ss(token);
	std::string aToken = "";
	std::getline(ss, aToken, ',');
	tempVector.x = static_cast<float>(Token2Double(aToken));
	std::getline(ss, aToken, ',');
	tempVector.y = static_cast<float>(Token2Double(aToken));
	std::getline(ss, aToken, ',');
	tempVector.z = static_cast<float>(Token2Double(aToken));

	return tempVector;
}
double CPlayerInfo2D::Token2Double(const string token)
{
	return atof(token.c_str());
}
bool CPlayerInfo2D::Token2Bool(const string token)
{
	return token.at(0) == '1';
}


// Load this class
bool CPlayerInfo2D::Load(const string saveFileName)
{
	ifstream myfile(saveFileName.c_str(), ios::in);
	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			cout << line << '\n';

			std::istringstream ss(line);
			std::string aToken = "";

			// Get the tag from the line
			while (std::getline(ss, aToken, '=')) {
				std::cout << aToken << endl;
				
				// Get the data from the line
				std::string theTag = aToken;
				std::getline(ss, aToken, '=');
				std::cout << aToken << endl;
				if (theTag == "defaultPosition")
				{
					defaultPosition = Token2Vector(aToken);
				}
				else if (theTag == "defaultTarget")
				{
					defaultTarget = Token2Vector(aToken);
				}
				else if (theTag == "defaultUp")
				{
					defaultUp = Token2Vector(aToken);
				}
				else if (theTag == "position")
				{
					position = Token2Vector(aToken);
				}
				else if (theTag == "target")
				{
					target = Token2Vector(aToken);
				}
				else if (theTag == "up")
				{
					up = Token2Vector(aToken);
				}
				else if (theTag == "maxBoundary")
				{
					maxBoundary = Token2Vector(aToken);
				}
				else if (theTag == "minBoundary")
				{
					minBoundary = Token2Vector(aToken);
				}
				else if (theTag == "m_dSpeed")
				{
					m_dSpeed = Token2Double(aToken);
				}
				else if (theTag == "m_dAcceleration")
				{
					m_dAcceleration = Token2Double(aToken);
				}
				else if (theTag == "m_dJumpSpeed")
				{
					m_dJumpSpeed = Token2Double(aToken);
				}
				else if (theTag == "m_dJumpAcceleration")
				{
					m_dJumpAcceleration = Token2Double(aToken);
				}
				else if (theTag == "m_dFallSpeed")
				{
					m_dFallSpeed = Token2Double(aToken);
				}
				else if (theTag == "m_dFallAcceleration")
				{
					m_dFallAcceleration = Token2Double(aToken);
				}
				else if (theTag == "m_dElapsedTime")
				{
					m_dElapsedTime = Token2Double(aToken);
				}
				else if (theTag == "m_bJumpUpwards")
				{
					m_bJumpUpwards = Token2Bool(aToken);
				}
				else if (theTag == "m_bFallDownwards")
				{
					m_bFallDownwards = Token2Bool(aToken);
				}
				else if (theTag == "m_dFallAcceleration")
				{
					m_dFallAcceleration = Token2Bool(aToken);
				}
				else if (theTag == "m_dElapsedTime")
				{
					m_dElapsedTime = Token2Bool(aToken);
				}
				else if (theTag == "tileOffset_x")
				{
					tileOffset_x = Token2Bool(aToken);
				}
				else if (theTag == "tileOffset_y")
				{
					tileOffset_y = Token2Bool(aToken);
				}
				else if (theTag == "mapFineOffset_x")
				{
					mapFineOffset_x = Token2Bool(aToken);
				}
				else if (theTag == "mapFineOffset_y")
				{
					mapFineOffset_y = Token2Bool(aToken);
				}
				else if (theTag == "rearTileOffset_x")
				{
					rearTileOffset_x = Token2Bool(aToken);
				}
				else if (theTag == "rearTileOffset_y")
				{
					rearTileOffset_y = Token2Bool(aToken);
				}
				else if (theTag == "rearMapOffset_x")
				{
					rearMapOffset_x = Token2Bool(aToken);
				}
				else if (theTag == "rearMapOffset_y")
				{
					rearMapOffset_y = Token2Bool(aToken);
				}
				else if (theTag == "rearMapFineOffset_x")
				{
					rearMapFineOffset_x = Token2Bool(aToken);
				}			
				else if (theTag == "rearMapFineOffset_y")
				{
					rearMapFineOffset_y = Token2Bool(aToken);
				}
			}
		}
		myfile.close();
	}
	else
	{
#if(_DEBUG == TRUE)
		cout << "PlayerInfo: Unable to load " << saveFileName.c_str() << endl;
#endif
		myfile.close();
		return false;
	}

	return true;
}

// Save this class
bool CPlayerInfo2D::Save(const string saveFileName)
{
	ofstream myfile;
	myfile.open(saveFileName.c_str(), ios::out | ios::ate);
	if (myfile.is_open())
	{
		myfile << "defaultPosition=" << defaultPosition.x << "," << defaultPosition.y << "," 
										<< defaultPosition.z << endl;
		myfile << "defaultTarget=" << defaultTarget.x << "," << defaultTarget.y << "," 
										<< defaultTarget.z << endl;
		myfile << "defaultUp=" << defaultUp.x << "," << defaultUp.y << "," << defaultUp.z << endl;
		myfile << "position=" << position.x << "," << position.y << "," << position.z << endl;
		myfile << "target=" << target.x << "," << target.y << "," << target.z << endl;
		myfile << "up=" << up.x << "," << up.y << "," << up.z << endl;
		myfile << "maxBoundary=" << maxBoundary.x << "," << maxBoundary.y << "," << maxBoundary.z << endl;
		myfile << "minBoundary=" << minBoundary.x << "," << minBoundary.y << "," << minBoundary.z << endl;
		myfile << "tileSize_Width=" << tileSize_Width << endl;
		myfile << "tileSize_Height=" << tileSize_Height << endl;
		myfile << "m_dSpeed=" << m_dSpeed << endl;
		myfile << "m_dAcceleration=" << m_dAcceleration << endl;
		myfile << "m_bJumpUpwards=" << m_bJumpUpwards << endl;
		myfile << "m_dJumpSpeed=" << m_dJumpSpeed << endl;
		myfile << "m_dJumpAcceleration=" << m_dJumpAcceleration << endl;
		myfile << "m_dFallSpeed=" << m_dFallSpeed << endl;
		myfile << "m_bFallDownwards=" << m_bFallDownwards << endl;
		myfile << "m_dFallAcceleration=" << m_dFallAcceleration << endl;
		myfile << "m_dElapsedTime=" << m_dElapsedTime << endl;
		myfile << "tileOffset_x=" << tileOffset_x << endl;
		myfile << "tileOffset_y=" << tileOffset_y << endl;
		myfile << "mapFineOffset_x=" << mapFineOffset_x << endl;
		myfile << "mapFineOffset_y=" << mapFineOffset_y << endl;
		myfile << "rearTileOffset_x=" << rearTileOffset_x << endl;
		myfile << "rearTileOffset_y=" << rearTileOffset_y << endl;
		myfile << "rearMapOffset_x=" << rearMapOffset_x << endl;
		myfile << "rearMapOffset_y=" << rearMapOffset_y << endl;
		myfile << "rearMapFineOffset_x=" << rearMapFineOffset_x << endl;
		myfile << "rearMapFineOffset_y=" << rearMapFineOffset_y << endl;

		myfile.close();
		return true;
	}
	else
	{
#ifdef _DEBUG
		cout << "PlayerInfo: Unable to save " << saveFileName.c_str() << endl;
#endif
		myfile.close();
		return false;
	}
}

// Check for collision with CGoodies instances
void CPlayerInfo2D::UpdateGoodies(const int tileIndex_Column, const int tileIndex_Row)
{
	CGoodies* theGoodie = CGoodiesFactory::GetInstance()->GetGoodies(tileIndex_Column, tileIndex_Row);
	if (theGoodie)
	{
		// Perform any special operation

		// Remove the goodie from the factory
		if (CGoodiesFactory::GetInstance()->RemoveGoodies(tileIndex_Column, tileIndex_Row) == true)
		{
			// Remove the instance from the map
			theMapReference->theScreenMap[tileIndex_Row][tileIndex_Column] = 0;
		}
	}
}

void CPlayerInfo2D::AttackSound(void) const
{
	switch (rand() % 6)
	{
	case 0:
		CSoundEngine::GetInstance()->PlayASound("slash1");
		break;
	case 1:
		CSoundEngine::GetInstance()->PlayASound("slash2");
		break;
	case 2:
		CSoundEngine::GetInstance()->PlayASound("slash3");
		break;
	case 3:
		CSoundEngine::GetInstance()->PlayASound("slash4");
		break;
	case 4:
		CSoundEngine::GetInstance()->PlayASound("slash5");
		break;
	case 5:
		CSoundEngine::GetInstance()->PlayASound("slash6");
		break;
	default:
		break;
	}
}

void CPlayerInfo2D::StepSound(void) const
{
	switch (rand() % 4)
	{
	case 0:
		CSoundEngine::GetInstance()->PlayASound("step1");
		break;
	case 1:
		CSoundEngine::GetInstance()->PlayASound("step2");
		break;
	case 2:
		CSoundEngine::GetInstance()->PlayASound("step3");
		break;
	case 3:
		CSoundEngine::GetInstance()->PlayASound("step4");
		break;
	default:
		break;
	}
}

void CPlayerInfo2D::DoorSound(void) const
{
	switch (rand() % 4)
	{
	case 0:
		CSoundEngine::GetInstance()->PlayASound("door1");
		break;
	case 1:
		CSoundEngine::GetInstance()->PlayASound("door2");
		break;
	case 2:
		CSoundEngine::GetInstance()->PlayASound("door3");
		break;
	case 3:
		CSoundEngine::GetInstance()->PlayASound("door4");
		break;
	default:
		break;
	}
}

void CPlayerInfo2D::DeflectSound(void) const
{
	CSoundEngine::GetInstance()->PlayASound("deflect");
}

void CPlayerInfo2D::KillSound(int type) const
{
	switch (type)
	{
	case 0:
		CSoundEngine::GetInstance()->PlayASound("killcrystal");
		break;
	case 1:
		CSoundEngine::GetInstance()->PlayASound("killaxe");
		break;
	default:
		break;
	}
}

bool CPlayerInfo2D::getSecondAttack(void) const
{
	return secondAttack;
}

bool CPlayerInfo2D::getSkill(int skill) const
{
	return this->skill[skill];
}

void CPlayerInfo2D::setSkill(int skill, bool state)
{
	this->skill[skill] = state;
}

void CPlayerInfo2D::InitSound(void) const
{
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("bgm", "Sound//bgm.mp3");
	CSoundEngine::GetInstance()->AddSound("bgmwalk", "Sound//bgmwalk.mp3");
	CSoundEngine::GetInstance()->AddSound("bgmrroll", "Sound//rroll.mp3");
	CSoundEngine::GetInstance()->AddSound("bgmmii", "Sound//bgmmii.mp3");
	CSoundEngine::GetInstance()->AddSound("jump", "Sound//jump.wav");
	CSoundEngine::GetInstance()->AddSound("step1", "Sound//step1.wav");
	CSoundEngine::GetInstance()->AddSound("step2", "Sound//step2.wav");
	CSoundEngine::GetInstance()->AddSound("step3", "Sound//step3.wav");
	CSoundEngine::GetInstance()->AddSound("step4", "Sound//step4.wav");
	CSoundEngine::GetInstance()->AddSound("slash1", "Sound//slash1.wav");
	CSoundEngine::GetInstance()->AddSound("slash2", "Sound//slash2.wav");
	CSoundEngine::GetInstance()->AddSound("slash3", "Sound//slash3.wav");
	CSoundEngine::GetInstance()->AddSound("slash4", "Sound//slash4.wav");
	CSoundEngine::GetInstance()->AddSound("slash5", "Sound//slash5.wav");
	CSoundEngine::GetInstance()->AddSound("slash6", "Sound//slash6.wav");
	CSoundEngine::GetInstance()->AddSound("roll", "Sound//roll.wav");
	CSoundEngine::GetInstance()->AddSound("door1", "Sound//door1.wav");
	CSoundEngine::GetInstance()->AddSound("door2", "Sound//door2.wav");
	CSoundEngine::GetInstance()->AddSound("door3", "Sound//door3.wav");
	CSoundEngine::GetInstance()->AddSound("door4", "Sound//door4.wav");
	CSoundEngine::GetInstance()->AddSound("death", "Sound//death.wav");
	CSoundEngine::GetInstance()->AddSound("deflect", "Sound//deflect.wav");
	CSoundEngine::GetInstance()->AddSound("killcrystal", "Sound//killcrystal.wav");
	CSoundEngine::GetInstance()->AddSound("killaxe", "Sound//killaxe.wav");
	CSoundEngine::GetInstance()->AddSound("damaged", "Sound//damaged.wav");
	CSoundEngine::GetInstance()->AddSound("heal", "Sound//heal.wav");
}

bool CPlayerInfo2D::Roll()
{
	bool willRoll = false;
	if (skill[SK_ROLL_COST])
		willRoll = StaminaDecrease(0.2f);
	else
		willRoll = StaminaDecrease(0.3f);
	if (willRoll)
	{
		CSoundEngine::GetInstance()->PlayASound("roll");
		rollBounceTime = 0;
		return true;
	}
	else
		return false;
}

bool CPlayerInfo2D::StaminaDecrease(float decrease)
{
	if (stamina - decrease >= 0.f)
	{
		stamina -= decrease;
		return true;
	}
	else
		return false;
}

void CPlayerInfo2D::StaminaRegen(float regen, double dt)
{
	stamina += static_cast<float>(dt * regen);
	if (stamina > 1.f)
		stamina = 1.f;
}

void CPlayerInfo2D::InitSkill(void)
{
	for (int i = 0; i < SK_TOTAL; ++i)
	{
		skill[i] = true;
	}
	// triple jump
	// heart 2
	// heart 3
	// lifesteal
}
