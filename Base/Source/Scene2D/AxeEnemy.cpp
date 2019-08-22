#include "AxeEnemy.h"
#include "../EntityManager.h"
#include <iostream>

/********************************************************************************
 Constructor
 ********************************************************************************/
CAxeEnemy::CAxeEnemy(void)
	: theStrategy(NULL)
	, theMapReference(nullptr)
{
	hp = 2;
	maxHp = 2;
}

/********************************************************************************
 Destructor
 ********************************************************************************/
CAxeEnemy::~CAxeEnemy(void)
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
void CAxeEnemy::Init(CMap* m_cMap)
{
	theMapReference = m_cMap;
	position.x=0;
	position.y=0;
}

/********************************************************************************
 Set position x of the player
 ********************************************************************************/
void CAxeEnemy::SetPos_x(int pos_x)
{
	position.x = static_cast<float>(pos_x);
}

/********************************************************************************
 Set position y of the player
 ********************************************************************************/
void CAxeEnemy::SetPos_y(int pos_y)
{
	position.y = static_cast<float>(pos_y);
}

/********************************************************************************
 Set position of the player
 ********************************************************************************/
void CAxeEnemy::SetPos(const int pos_x, const int pos_y)
{
	position.x = static_cast<float>(pos_x);
	position.y = static_cast<float>(pos_y);
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
void CAxeEnemy::SetDestination(const int pos_x, const int pos_y)
{
	theDestination.x = static_cast<float>(pos_x);
	theDestination.y = static_cast<float>(pos_y);
	if (theStrategy != nullptr)
	{
		theStrategy->SetDestination(theDestination.x, theDestination.y);
	}
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
void CAxeEnemy::SetDestination(Vector3 pos)
{
	theDestination = pos;
	if (theStrategy != nullptr)
	{
		theStrategy->SetDestination(theDestination.x, theDestination.y);
	}
}

/********************************************************************************
 Get position x of the player
 ********************************************************************************/
int CAxeEnemy::GetPos_x(void) const
{
	return static_cast<int>(position.x);
}

/********************************************************************************
 Get position y of the player
 ********************************************************************************/
int CAxeEnemy::GetPos_y(void) const
{
	return static_cast<int>(position.y);
}

/********************************************************************************
 Get position of the player in Vector3 form
 ********************************************************************************/
Vector3 CAxeEnemy::GetPos(void) const
{
	return position;
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
int CAxeEnemy::GetDestination_x(void) const
{
	return static_cast<int>(theDestination.x);
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
int CAxeEnemy::GetDestination_y(void) const
{
	return static_cast<int>(theDestination.y);
}

/********************************************************************************
 Get the destination of this enemy as a Vector3
 ********************************************************************************/
Vector3 CAxeEnemy::GetDestination(void) const
{
	return theDestination;
}

void CAxeEnemy::SetIsDone(const bool _value)
{
	CPlayerInfo2D::GetInstance()->KillSound(1);
	EntityBase::SetIsDone(_value);
}

bool CAxeEnemy::IsAttacking(void)
{
	return isAttacking();
}

/********************************************************************************
 Update
 ********************************************************************************/
void CAxeEnemy::Update(void)
{
	if (theStrategy != NULL)
	{
		if (GetFrameState() == GetFrameTotal())
			isDead = true;
		else
		{
			if (!isDead && !isDone)
			{
				theStrategy->Update(theDestination, position);
				constrain();
			}
			if (isDone)
				SetAnimationStatus(A_DIE_1);
			else
				switch (theStrategy->GetType())
				{
					case CStrategy::E_STRATEGY_KILL:
					{
						if (dynamic_cast<CStrategy_Kill*>(theStrategy)->GetState() == CStrategy_Kill::IDLE)
							SetAnimationStatus(A_IDLE_1);
						else if (dynamic_cast<CStrategy_Kill*>(theStrategy)->GetState() == CStrategy_Kill::REPEL)
							SetAnimationStatus(A_RUN_1);
						else if (dynamic_cast<CStrategy_Kill*>(theStrategy)->GetState() == CStrategy_Kill::ATTACK && !isAttacking())
							SetAnimationStatus(A_ATTACK_1);
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
void CAxeEnemy::ChangeStrategy(CStrategy* theNewStrategy, bool bDelete)
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

void CAxeEnemy::constrain()
{
	int checkPosition_Y = theMapReference->GetNumOfTiles_Height() - (int)ceil(position.y / theMapReference->GetTileSize_Height());

	if (theStrategy->GetLR() < 0) // Left
	{
		int checkPosition_X = (int)((position.x - (theMapReference->GetTileSize_Width() >> 1)) / theMapReference->GetTileSize_Width());

		if (checkPosition_X >= 0)
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
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
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
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
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
			{
				position.y = static_cast<float>((theMapReference->GetNumOfTiles_Height() - checkPosition_Y - 1) * theMapReference->GetTileSize_Height() - (theMapReference->GetTileSize_Height() >> 1));
			}
		}
		else
		{
			if ((theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1) ||
				(theMapReference->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 1))
			{
				position.y = static_cast<float>((theMapReference->GetNumOfTiles_Height() - checkPosition_Y - 1) * theMapReference->GetTileSize_Height() - (theMapReference->GetTileSize_Height() >> 1));
			}
		}
	}
	else if (!theStrategy->GetUD() < 0) // Down
	{
		int checkPosition_X = (int)((position.x - (theMapReference->GetTileSize_Width() >> 1)) / theMapReference->GetTileSize_Width());
		int checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
			(int)ceil((float)(position.y) / theMapReference->GetTileSize_Height());

		if (((int)(position.x - (theMapReference->GetTileSize_Width() >> 1)) % theMapReference->GetTileSize_Width()) == 0)
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
			{
				position.y = static_cast<float>((theMapReference->GetNumOfTiles_Height() - checkPosition_Y) * theMapReference->GetTileSize_Height() + (theMapReference->GetTileSize_Height() >> 1));
			}
		}
		else
		{
			if ((theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1) ||
				(theMapReference->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 1))
			{
				position.y = static_cast<float>((theMapReference->GetNumOfTiles_Height() - checkPosition_Y) * theMapReference->GetTileSize_Height() + (theMapReference->GetTileSize_Height() >> 1));
			}
		}
	}
}

CAxeEnemy * Create::AxeEnemyEntity(CMap* m_cMap, CStrategy* theNewStrategy, bool bDelete, const Vector3 & _position)
{
	CAxeEnemy* result = new CAxeEnemy();
	result->SetType(result->E_ENEMY);
	result->Init(m_cMap);
	result->ChangeStrategy(theNewStrategy, bDelete);
	result->SetPosition(_position);
	result->SetScale(Vector3(16, 16, 16));
	result->SetCollider(true);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}
