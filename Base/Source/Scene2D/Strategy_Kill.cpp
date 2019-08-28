#include "Strategy_Kill.h"
#include <iostream>

using namespace std;

/********************************************************************************
Constructor
********************************************************************************/
CStrategy_Kill::CStrategy_Kill()
{
	SetType(E_STRATEGY_KILL);
}

/********************************************************************************
Destructor
********************************************************************************/
CStrategy_Kill::~CStrategy_Kill()
{
}

/********************************************************************************
 Update method
********************************************************************************/
void CStrategy_Kill::Update(Vector3& PlayerPosition, Vector3& theEnemyPosition, double playerLevel)
{
	// Gravity
	if (isOnAir)
	{
		theEnemyPosition.y += static_cast<float>(m);
		m -= 0.5;
	}

	// Decide which state to change to
	int distanceHeroToEnemy = CalculateDistance(PlayerPosition, theEnemyPosition);

	if (playerLevel >= 12)
	{
		if (distanceHeroToEnemy < (AI_STATE_ATTACK + 32)*(AI_STATE_ATTACK + 32))
		{
			CurrentState = ATTACK;
		}
	}
	else if (distanceHeroToEnemy < AI_STATE_ATTACK*AI_STATE_ATTACK)
	{		
		CurrentState = ATTACK;
	}
	else
		CurrentState = PATROL;

	if (bounce > 60)
	{
		bounce = 0;
		n = Math::RandIntMinMax(-1, 1);
	}

	// Based on the current state, determine enemy behaviour
	switch(CurrentState)
	{
	case PATROL:
		++bounce;
		if (theEnemyPosition.x < 16 || theEnemyPosition.x > (theMapReference->GetNumOfTiles_Width() - 1) * theMapReference->GetTileSize_Width())
			n = -n;
		if (bounce < 17)
			theEnemyPosition.x = static_cast<float>(theEnemyPosition.x + n);
		else CurrentState = IDLE;

		break;
	case ATTACK:
		if (playerLevel >= 8 && !isOnAir && PlayerPosition.y > theEnemyPosition.y)
		{
			m = 6;
		}
		if (theEnemyPosition.x != PlayerPosition.x)
		{
			if (playerLevel >= 5)
			{
				n = 1.5;
			}
			else n = 1;

			if (theEnemyPosition.x > PlayerPosition.x)
			{
				n = -n;
			}

			theEnemyPosition.x = static_cast<float>(theEnemyPosition.x + n);
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
void CStrategy_Kill::SetDestination(const float x, const float y)
{
	theDestination.x = x;
	theDestination.y = y;
}

/********************************************************************************
 Get the destination for this strategy
 ********************************************************************************/
int CStrategy_Kill::GetDestination_x(void)
{
	return static_cast<int>(theDestination.x);
}

/********************************************************************************
 Get the destination for this strategy
 ********************************************************************************/
int CStrategy_Kill::GetDestination_y(void)
{
	return static_cast<int>(theDestination.y);
}

/********************************************************************************
 Get the destination for this strategy
 ********************************************************************************/
Vector3 CStrategy_Kill::GetDestination(void)
{
	return theDestination;
}

/********************************************************************************
 Get the FSM state for this strategy
 ********************************************************************************/
CStrategy_Kill::CURRENT_STATE CStrategy_Kill::GetState(void)
{
	return CurrentState;
}

/********************************************************************************
 Set the FSM state for this strategy
 ********************************************************************************/
void CStrategy_Kill::SetState(CStrategy_Kill::CURRENT_STATE theEnemyState)
{
	CurrentState = theEnemyState;
}
