#include "Enemy.h"
#include "../EntityManager.h"
#include <iostream>

/********************************************************************************
 Constructor
 ********************************************************************************/
CEnemy::CEnemy(void)
	: theStrategy(NULL)
	, theMapReference(nullptr)
{
	hp = 1;
	maxHp = 1;
}

/********************************************************************************
 Destructor
 ********************************************************************************/
CEnemy::~CEnemy(void)
{
	if (theStrategy != NULL)
	{
		delete theStrategy;
		theStrategy = NULL;
	}
}

/********************************************************************************
 Initialise this class instance
 ********************************************************************************/
void CEnemy::Init(CMap* m_cMap)
{
	theMapReference = m_cMap;
	position.x=0;
	position.y=0;
}

/********************************************************************************
 Set position x of the player
 ********************************************************************************/
void CEnemy::SetPos_x(int pos_x)
{
	position.x = static_cast<float>(pos_x);
}

/********************************************************************************
 Set position y of the player
 ********************************************************************************/
void CEnemy::SetPos_y(int pos_y)
{
	position.y = static_cast<float>(pos_y);
}

/********************************************************************************
 Set position of the player
 ********************************************************************************/
void CEnemy::SetPos(const int pos_x, const int pos_y)
{
	position.x = static_cast<float>(pos_x);
	position.y = static_cast<float>(pos_y);
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
void CEnemy::SetDestination(const int pos_x, const int pos_y)
{
	theDestination.x = static_cast<float>(pos_x);
	theDestination.y = static_cast<float>(pos_y);
	if (theStrategy != NULL)
	{
		theStrategy->SetDestination(theDestination.x, theDestination.y);
	}
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
void CEnemy::SetDestination(Vector3 pos)
{
	theDestination = pos;
	if (theStrategy != NULL)
	{
  		theStrategy->SetDestination(theDestination.x, theDestination.y);
	}
}

/********************************************************************************
 Get position x of the player
 ********************************************************************************/
int CEnemy::GetPos_x(void) const
{
	return static_cast<int>(position.x);
}

/********************************************************************************
 Get position y of the player
 ********************************************************************************/
int CEnemy::GetPos_y(void) const
{
	return static_cast<int>(position.y);
}

/********************************************************************************
 Get position of the player in Vector3 form
 ********************************************************************************/
Vector3 CEnemy::GetPos(void) const
{
	return position;
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
int CEnemy::GetDestination_x(void) const
{
	return static_cast<int>(theDestination.x);
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
int CEnemy::GetDestination_y(void) const
{
	return static_cast<int>(theDestination.y);
}

/********************************************************************************
 Get the destination of this enemy as a Vector3
 ********************************************************************************/
Vector3 CEnemy::GetDestination(void) const
{
	return theDestination;
}

void CEnemy::SetIsDone(const bool _value)
{
	CPlayerInfo2D::GetInstance()->KillSound(0);
	EntityBase::SetIsDone(_value);
}

/********************************************************************************
 Update
 ********************************************************************************/
void CEnemy::Update(void)
{
	if (theStrategy != nullptr)
	{
		if (GetFrameState() == GetFrameTotal())
			isDead = true;
		else
		{
			if (!isDead && !isDone)
			{
				theStrategy->Update(theDestination, position, CPlayerInfo2D::GetInstance()->GetLevel(true));
				constrain();
			}
			if (isDone)
				SetAnimationStatus(C_DIE_1);
			else
				switch (theStrategy->GetType())
				{
					case CStrategy::E_STRATEGY_SHOOT:
					{
						int tempstate = dynamic_cast<CStrategy_Shoot*>(theStrategy)->GetState();
						if (tempstate == CStrategy_Shoot::ATTACK && !isAttacking())
							SetAnimationStatus(C_ATTACK_1);
						else if (tempstate == CStrategy_Shoot::IDLE ||
							tempstate == CStrategy_Shoot::REPEL)
							SetAnimationStatus(C_IDLE_1);
						break;
					}
					default:
						break;
				}
			UpdateAnimationIndex(0.2f);
		}
	}
}

/********************************************************************************
 Strategy
 ********************************************************************************/
void CEnemy::ChangeStrategy(CStrategy* theNewStrategy, bool bDelete)
{
	if (bDelete == true)
	{
		if (theStrategy != NULL)
		{
			delete theStrategy;
			theStrategy = NULL;
		}
	}

	theStrategy = theNewStrategy;
	theStrategy->Init(theMapReference);
}

void CEnemy::constrain()
{
	int checkPosition_Y = theMapReference->GetNumOfTiles_Height() - (int)ceil(position.y / theMapReference->GetTileSize_Height());

	if (theStrategy->GetLR() < 0) // Left
	{
		int checkPosition_X = (int)((position.x - (theMapReference->GetTileSize_Width() >> 1)) / theMapReference->GetTileSize_Width());

		if (checkPosition_X >= 0)
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] > 0 && theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] < 100)
			{
				position.x = static_cast<float>((checkPosition_X + 1) * theMapReference->GetTileSize_Width() + (theMapReference->GetTileSize_Width() >> 1));
			}
		}
	}
	else if (theStrategy->GetLR() > 0) // Right
	{
		int checkPosition_X = (int)((position.x + (theMapReference->GetTileSize_Width() >> 1)) / theMapReference->GetTileSize_Width());

		if (checkPosition_X < theMapReference->GetNumOfTiles_Width())
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] > 0 && theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] < 100)
			{
				position.x = static_cast<float>((checkPosition_X - 1) * theMapReference->GetTileSize_Width() + (theMapReference->GetTileSize_Width() >> 1));
			}
		}
	}

	if (theStrategy->GetUD() > 0) // Up
	{
		int checkPosition_X = (int)((position.x - (theMapReference->GetTileSize_Width() >> 1)) / theMapReference->GetTileSize_Width());
		int checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
			(int)ceil((float)(position.y + (theMapReference->GetTileSize_Height() >> 1)) / theMapReference->GetTileSize_Height());

		if (((int)(position.x - (theMapReference->GetTileSize_Width() >> 1)) % theMapReference->GetTileSize_Width()) == 0)
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] > 0 && theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] < 100)
			{
				position.y = static_cast<float>((theMapReference->GetNumOfTiles_Height() - checkPosition_Y - 1) * theMapReference->GetTileSize_Height() - (theMapReference->GetTileSize_Height() >> 1));
			}
		}
		else
		{
			if ((theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] > 0 && theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] < 100) ||
				(theMapReference->theScreenMap[checkPosition_Y][checkPosition_X + 1] > 0 && theMapReference->theScreenMap[checkPosition_Y][checkPosition_X + 1] < 100))
			{
				position.y = static_cast<float>((theMapReference->GetNumOfTiles_Height() - checkPosition_Y - 1) * theMapReference->GetTileSize_Height() - (theMapReference->GetTileSize_Height() >> 1));
			}
		}
	}
	else if (theStrategy->GetUD() < 0) // Down
	{
		int checkPosition_X = (int)((position.x - (theMapReference->GetTileSize_Width() >> 1)) / theMapReference->GetTileSize_Width());
		int checkPosition_Y = theMapReference->GetNumOfTiles_Height() - (int)ceil((float)(position.y) / theMapReference->GetTileSize_Height());

		if (((int)(position.x - (theMapReference->GetTileSize_Width() >> 1)) % theMapReference->GetTileSize_Width()) == 0)
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] > 0 && theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] < 100)
			{
				position.y = static_cast<float>((theMapReference->GetNumOfTiles_Height() - checkPosition_Y) * theMapReference->GetTileSize_Height() + (theMapReference->GetTileSize_Height() >> 1));
			}
		}
		else
		{
			if ((theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] > 0 && theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] < 100) ||
				(theMapReference->theScreenMap[checkPosition_Y][checkPosition_X + 1] > 0 && theMapReference->theScreenMap[checkPosition_Y][checkPosition_X + 1] < 100))
			{
				position.y = static_cast<float>((theMapReference->GetNumOfTiles_Height() - checkPosition_Y) * theMapReference->GetTileSize_Height() + (theMapReference->GetTileSize_Height() >> 1));
			}
		}
	}
}

CEnemy * Create::EnemyEntity(CMap* m_cMap, CStrategy* theNewStrategy, bool bDelete, const Vector3 & _position)
{
	CEnemy* result = new CEnemy();
	result->SetType(result->E_ENEMY);
	result->Init(m_cMap);
	result->ChangeStrategy(theNewStrategy, bDelete);
	result->SetPosition(_position);
	result->SetScale(Vector3(16, 16, 16));
	result->SetCollider(true);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}
