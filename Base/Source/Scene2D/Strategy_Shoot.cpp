#include "Strategy_Shoot.h"
#include <iostream>

using namespace std;

/********************************************************************************
Constructor
********************************************************************************/
CStrategy_Shoot::CStrategy_Shoot()
{
	SetType(E_STRATEGY_SHOOT);
	bounce = 60;
}

/********************************************************************************
Destructor
********************************************************************************/
CStrategy_Shoot::~CStrategy_Shoot()
{
}

/********************************************************************************
 Update method
********************************************************************************/
void CStrategy_Shoot::Update(Vector3& PlayerPosition, Vector3& theEnemyPosition, double playerLevel)
{
	// Decide which state to change to
	int distanceHeroToEnemy = CalculateDistance(PlayerPosition, theEnemyPosition);
	if (playerLevel >= 3)
	{
		if (distanceHeroToEnemy < (AI_STATE_ATTACK + 32)*(AI_STATE_ATTACK + 32))
		{
			if (distanceHeroToEnemy < AI_STATE_REPEL*AI_STATE_REPEL)
				CurrentState = REPEL;
			else
				CurrentState = ATTACK;
		}
	}
	else if (distanceHeroToEnemy < AI_STATE_ATTACK*AI_STATE_ATTACK)
	{
		if (distanceHeroToEnemy < AI_STATE_REPEL*AI_STATE_REPEL)
			CurrentState = REPEL;
		else
			CurrentState = ATTACK;
	}
	else
		CurrentState = IDLE;

	// Based on the current state, determine enemy behaviour
	switch (CurrentState)
	{
	case ATTACK:
		++bounce;
		if (bounce > 59)
		{
			if (playerLevel >= 4)
			{
				bounce -= Math::RandIntMinMax(10, 40);
			}
			else if (playerLevel <= 3)
			{
				bounce -= Math::RandIntMinMax(20, 50);
			}
			else if (playerLevel <= 1)
			{
				bounce -= Math::RandIntMinMax(30, 60);
			}
			Vector3 direction = (PlayerPosition - theEnemyPosition).Normalized();
			Create::Projectile("Crystal_Projectile_1", theEnemyPosition, Vector3(10, 10, 10), direction, 1.5f, 100, EntityBase::E_ENEMY_PROJECTILES);
		}
		break;
	case REPEL:
		if (theEnemyPosition.x > 16 && theEnemyPosition.x < (theMapReference->GetNumOfTiles_Width() - 1) * theMapReference->GetTileSize_Width())
		{
			if (theEnemyPosition.x != PlayerPosition.x)
			{
				n = 1;
				if (theEnemyPosition.x < PlayerPosition.x)
				{
					n = -n;
				}
				theEnemyPosition.x = theEnemyPosition.x + n;
			}
		}
		break;
	default:
		// Do nothing if idling
		break;
	}
}

/********************************************************************************
 Set the destination for this strategy
 ********************************************************************************/
void CStrategy_Shoot::SetDestination(const float x, const float y)
{
	theDestination.x = x;
	theDestination.y = y;
}

/********************************************************************************
 Get the destination for this strategy
 ********************************************************************************/
int CStrategy_Shoot::GetDestination_x(void)
{
	return static_cast<int>(theDestination.x);
}

/********************************************************************************
 Get the destination for this strategy
 ********************************************************************************/
int CStrategy_Shoot::GetDestination_y(void)
{
	return static_cast<int>(theDestination.y);
}

/********************************************************************************
 Get the destination for this strategy
 ********************************************************************************/
Vector3 CStrategy_Shoot::GetDestination(void)
{
	return theDestination;
}

/********************************************************************************
 Get the FSM state for this strategy
 ********************************************************************************/
CStrategy_Shoot::CURRENT_STATE CStrategy_Shoot::GetState(void)
{
	return CurrentState;
}

/********************************************************************************
 Set the FSM state for this strategy
 ********************************************************************************/
void CStrategy_Shoot::SetState(CStrategy_Shoot::CURRENT_STATE theEnemyState)
{
	CurrentState = theEnemyState;
}
