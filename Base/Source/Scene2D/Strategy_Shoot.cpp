#include "Strategy_Shoot.h"
#include <iostream>

using namespace std;

/********************************************************************************
Constructor
********************************************************************************/
CStrategy_Shoot::CStrategy_Shoot()
{
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
void CStrategy_Shoot::Update(Vector3& PlayerPosition, Vector3& theEnemyPosition)
{
	++bounce;

	// Decide which state to change to
	int distanceHeroToEnemy = CalculateDistance(PlayerPosition, theEnemyPosition);
	if (distanceHeroToEnemy < AI_STATE_ATTACK*AI_STATE_ATTACK)
	{		
		if (distanceHeroToEnemy < AI_STATE_REPEL*AI_STATE_REPEL)
			CurrentState = REPEL;
		else
			CurrentState = ATTACK;
	}
	else
		CurrentState = IDLE;

	// Based on the current state, determine enemy behaviour
	switch(CurrentState)
	{
	case ATTACK:
		//theEnemyPosition.x = theEnemyPosition.x + (theDestination.x - theEnemyPosition.x > 0 ? 1 : -1);
		if (bounce > 59)
		{
			bounce -= Math::RandIntMinMax(10, 60);
			Vector3 direction = (PlayerPosition - theEnemyPosition).Normalized();
			/*
			enemybullet->dir = direction;
			*/
		}
		
		break;
	case REPEL:
		theEnemyPosition.x = theEnemyPosition.x + (PlayerPosition.x - theEnemyPosition.x > 0 ? -1 : 1 );
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
	return theDestination.x;
}

/********************************************************************************
 Get the destination for this strategy
 ********************************************************************************/
int CStrategy_Shoot::GetDestination_y(void)
{
	return theDestination.y;
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
