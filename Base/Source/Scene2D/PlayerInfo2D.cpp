#include "PlayerInfo2D.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Projectile.h"
#include "GoodiesFactory.h"
#include "../SoundEngine.h"

// Allocating and initializing CPlayerInfo2D's static data member.
// The pointer is allocated but not the object's constructor.
CPlayerInfo2D *CPlayerInfo2D::s_instance = 0;

CPlayerInfo2D::CPlayerInfo2D(void)
	: m_dSpeed(4.0)
	, m_dMoveSpeed(0.6)
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
	, hp(3)
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
	, attackBounceTime(0)
	, rollBounceTime(0)
	, attackBounceTimeLimit(0.5)
	, rollBounceTimeLimit(0.7)
	, rollSpeed(0.8f)
{
}

CPlayerInfo2D::~CPlayerInfo2D(void)
{
}

// Initialise this class instance
void CPlayerInfo2D::Init(void)
{
	// Set the default values
	defaultPosition.Set(0,0,8);
	defaultTarget.Set(0,0,0);
	defaultUp.Set(0,1,0);

	// Set the current values
	position.Set(0, 0, 8);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);

	// Set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	// Set default tile sizes
	tileSize_Width = 16;
	tileSize_Height = 16;

	InitSound();
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

// Returns true if the player is on ground
bool CPlayerInfo2D::isOnGround(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == false)
		return true;

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
	if (!isRolling()) // Air roll?
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

// Get m_dJumpAcceleration of the player
double CPlayerInfo2D::GetJumpAcceleration(void) const
{
	return m_dJumpAcceleration;
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
		m_dJumpSpeed = 6.0f;
		m_bDoubleJumped = true;
	}

	// Record current position before jump
	int currentPosition_Y = theMapReference->GetNumOfTiles_Height() - (int)ceil((float)position.y / tileSize_Height);

	// Update the jump
	position.y += m_dJumpSpeed;
	m_dJumpSpeed -= 0.5f;
	if (m_dJumpSpeed <= 0)
		SetOnFreeFall(true);

	// If the player has jumped out of the screen, 
	// then start free fall and lock loaction
	if (position.y + tileSize_Height > theMapReference->GetNumOfTiles_Height()*theMapReference->GetTileSize_Height())
	{
		SetOnFreeFall(true);
		position.y = theMapReference->GetNumOfTiles_Height()*theMapReference->GetTileSize_Height() - tileSize_Height;
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
			if (theMapReference->theScreenMap[i][checkPosition_X] == 1)
			{
				// Since the new position does not allow the player to move into, then go back to the old position
				position.y = (theMapReference->GetNumOfTiles_Height() - i - 1) * tileSize_Height - (tileSize_Height >> 1);
				// Set on free fall
				SetOnFreeFall(true);
				break;
			}
		}
		else
		{
			if ((theMapReference->theScreenMap[i][checkPosition_X] == 1) ||
				(theMapReference->theScreenMap[i][checkPosition_X + 1] == 1))
			{
				// Since the new position does not allow the player to move into, then go back to the old position
				position.y = (theMapReference->GetNumOfTiles_Height() - i - 1) * tileSize_Height - (tileSize_Height >> 1);
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
	position.y -= m_dFallSpeed;
	if (m_dFallSpeed <= 6)
	m_dFallSpeed += 0.5;
	if (position.y - tileSize_Height < tileSize_Height/2 - 2)
	{
		StopVerticalMovement();// Note : Dies/-1 hp
		position.y = tileSize_Height + tileSize_Height/2;
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
			if (theMapReference->theScreenMap[i][checkPosition_X] == 1)
			{
				// Since the new position does not allow the player to move into, then go back to the old position
				position.y = (theMapReference->GetNumOfTiles_Height() - i) * tileSize_Height + (tileSize_Height >> 1);
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
				(theMapReference->theScreenMap[i][checkPosition_X + 1] == 1))
			{
				// Since the new position does not allow the player to move into, then go back to the old position
				position.y = (theMapReference->GetNumOfTiles_Height() - i) * tileSize_Height + (tileSize_Height >> 1);
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
	if (rollSpeed <= 0.8f)
		rollSpeed += dt * 0.05;
	attackBounceTime += dt;
	rollBounceTime += dt;
	// Update the player position
	//if (KeyboardController::GetInstance()->IsKeyDown('W'))
	//	MoveUpDown(true, 1.0f);
	//if (KeyboardController::GetInstance()->IsKeyDown('S'))
	//	MoveUpDown(false, 1.0f);
	

	if (KeyboardController::GetInstance()->IsKeyPressed('Q') && rollBounceTime > rollBounceTimeLimit || !isFacingRight() && isRolling()) // Roll Left
		MoveLeftRight(true, rollSpeed);
	else if (KeyboardController::GetInstance()->IsKeyPressed('E') && rollBounceTime > rollBounceTimeLimit || isFacingRight() && isRolling()) // Roll Right
		MoveLeftRight(false, rollSpeed);
	else if (KeyboardController::GetInstance()->IsKeyDown('A')  && !KeyboardController::GetInstance()->IsKeyPressed('J')) // Move Left
		MoveLeftRight(true, m_dMoveSpeed);
	else if (KeyboardController::GetInstance()->IsKeyDown('D') && !KeyboardController::GetInstance()->IsKeyPressed('J')) // Move Right
		MoveLeftRight(false, m_dMoveSpeed);
	else if (KeyboardController::GetInstance()->IsKeyPressed('J') && KeyboardController::GetInstance()->IsKeyDown('W') || KeyboardController::GetInstance()->IsKeyPressed('J') && KeyboardController::GetInstance()->IsKeyDown('S') && !isOnGround())
	{
		Attack((!isFacingRight()), 0.5f);
	}
	else if (KeyboardController::GetInstance()->IsKeyPressed('J') && KeyboardController::GetInstance()->IsKeyDown('A') && !isAttacking()) // Attack Left
	{
			Attack(true, 0.5f);
	}
	else if (KeyboardController::GetInstance()->IsKeyPressed('J') && KeyboardController::GetInstance()->IsKeyDown('D') && !isAttacking()) // Attack Right
	{
		Attack(false, 0.5f);
	}
	else if (KeyboardController::GetInstance()->IsKeyPressed('J'))
	{
		Attack(!isFacingRight(), 0.5f);
	}
	else if (isAttacking())
	{
		UpdateAnimationIndex(0.5f);
	}
	else if (isOnGround()) // Idle
	{
		if (isFacingRight())
			SetAnimationStatus(CAnimation::P_IDLE_R1);
		else
			SetAnimationStatus(CAnimation::P_IDLE_L1);
		UpdateAnimationIndex(0.1f);
	}

	if (position.x + (tileSize_Width >> 1) > theMapReference->getNumOfTiles_MapWidth() * theMapReference->GetTileSize_Width())
		position.x = theMapReference->getNumOfTiles_MapWidth() * theMapReference->GetTileSize_Width() - (tileSize_Width>>1);
	if (position.x - (tileSize_Width >> 1) < 0)
		position.x = 0 + tileSize_Width>>1;

	if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE) && !m_bJumped && isOnAir() && !m_bDoubleJump && isRolling() && !isAttacking())
	{
		m_bJumped = true;
		m_bDoubleJump = true;
		SetToJumpUpwards(true);
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	{
		m_bJumpKeyHeld = false;
	}

	if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE) && !m_bJumpKeyHeld && !m_bDoubleJump && m_bJumped && !isRolling() && !isAttacking())
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
	if (KeyboardController::GetInstance()->IsKeyPressed('Q') || isRolling() && !isFacingRight())
	{
	// Find the tile number which the player's left side is on
	checkPosition_X = (int)((position.x - (tileSize_Width >> 1)) / tileSize_Width);
	if (KeyboardController::GetInstance()->IsKeyPressed('Q') && rollBounceTime > rollBounceTimeLimit)
	{
		SetAnimationStatus(CAnimation::P_ROLL_L1);
		CSoundEngine::GetInstance()->PlayASound("roll");
		rollBounceTime = 0;
		if (rollSpeed > 0.4f)
			rollSpeed -= 0.1;

	}
	if (checkPosition_X >= 0)
	{
		if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
		{
			position.x = (checkPosition_X + 1) * tileSize_Width + (tileSize_Width >> 1);
		}
	}
	}
	else if (KeyboardController::GetInstance()->IsKeyPressed('E') || isRolling() && isFacingRight())
	{
	// Find the tile number which the player's right side is on
	checkPosition_X = (int)((position.x + (tileSize_Width >> 1)) / tileSize_Width);
	if (KeyboardController::GetInstance()->IsKeyPressed('E') && rollBounceTime > rollBounceTimeLimit)
	{
		SetAnimationStatus(CAnimation::P_ROLL_R1);
		CSoundEngine::GetInstance()->PlayASound("roll");
		rollBounceTime = 0;
		if (rollSpeed > 0.4f)
			rollSpeed -= 0.1;
	}

	if (checkPosition_X < theMapReference->getNumOfTiles_MapWidth())
	{

		if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
		{
			// this part causes the player to be stuck when there is a tile on its right
			position.x = (checkPosition_X - 1) * tileSize_Width + (tileSize_Width >> 1);
		}

	}
	}
	// Check if the hero can move sideways
	else if (KeyboardController::GetInstance()->IsKeyDown('A'))
	{
		// Find the tile number which the player's left side is on
		checkPosition_X = (int)((position.x - (tileSize_Width >> 1)) / tileSize_Width);
		if (isOnGround() && !isAttacking())
			SetAnimationStatus(CAnimation::P_RUN_L1);

		if (checkPosition_X >= 0)
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
			{
				position.x = (checkPosition_X + 1) * tileSize_Width + (tileSize_Width >> 1);
			}
		}

	}
	else if (KeyboardController::GetInstance()->IsKeyDown('D'))
	{
		// Find the tile number which the player's right side is on
		checkPosition_X = (int)((position.x + (tileSize_Width >> 1)) / tileSize_Width);
		if (isOnGround() && !isAttacking())
			SetAnimationStatus(CAnimation::P_RUN_R1);

		if (checkPosition_X < theMapReference->GetNumOfTiles_Width())
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
			{
				position.x = (checkPosition_X - 1) * tileSize_Width + (tileSize_Width >> 1);
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
		if (!isAttacking())
		{
			if (mode)
			{
				SetAnimationStatus(CAnimation::P_ATTACK_L1);
				AttackSound();
			}
			else
			{
				SetAnimationStatus(CAnimation::P_ATTACK_R1);
				AttackSound();
			}
		}
		UpdateAnimationIndex(timeDiff);
		attackBounceTime = 0.f;
	}
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
	if (position.x >= maxBoundary.x + mapOffset_x - (tileSize_Width >> 1))
	{
		//position.x = maxBoundary.x - (tileSize_Width >> 1);
		// 0.325 ~ 0.675 = 0.25 of screen
		mapOffset_x += m_dSpeed * (m_dMoveSpeed + ((position.x - maxBoundary.x - mapOffset_x) / (maxBoundary.x * 0.25f) * (rollSpeed - m_dMoveSpeed)));
		if (mapOffset_x + theMapReference->getScreenWidth() > theMapReference->GetNumOfTiles_Width() * theMapReference->GetTileSize_Width())
			mapOffset_x = theMapReference->GetNumOfTiles_Width() * theMapReference->GetTileSize_Width() - theMapReference->getScreenWidth();
	}
	if (position.y > maxBoundary.y - tileSize_Height) // for y-scrolling
	{
		position.y = maxBoundary.y - (tileSize_Height >> 1);
	}
	if (position.x <= minBoundary.x + mapOffset_x)
	{
		//position.x = minBoundary.x;

		if (position.x <= minBoundary.x * 0.75f + mapOffset_x)
			mapOffset_x -= m_dSpeed * (rollSpeed - 0.1f);
		else
			mapOffset_x -= m_dSpeed * (m_dMoveSpeed - 0.1f);

		if (mapOffset_x < 0)
			mapOffset_x = 0;
	}
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
	rearMapOffset_x = (int)(mapOffset_x/* / 2*/);
	rearTileOffset_x = (int)(rearMapOffset_x / theRearMapReference->GetTileSize_Width());
	if (rearTileOffset_x + theRearMapReference->GetNumOfTiles_Width() > 
			theRearMapReference->getNumOfTiles_MapWidth())
		rearTileOffset_x = theRearMapReference->getNumOfTiles_MapWidth() - 
								theRearMapReference->GetNumOfTiles_Width();
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
	tempVector.x = Token2Double(aToken);
	std::getline(ss, aToken, ',');
	tempVector.y = Token2Double(aToken);
	std::getline(ss, aToken, ',');
	tempVector.z = Token2Double(aToken);

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
#if(_DEBUG == TRUE)
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

void CPlayerInfo2D::InitSound(void) const
{
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("bgm", "Sound//bgm.mp3");
	CSoundEngine::GetInstance()->AddSound("bgmwalk", "Sound//bgmwalk.mp3");
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

	CSoundEngine::GetInstance()->PlayBGM("bgmwalk");
}
