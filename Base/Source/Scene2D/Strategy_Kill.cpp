#include "Strategy_Kill.h"
#include <iostream>

using namespace std;

/********************************************************************************
Constructor
********************************************************************************/
CStrategy_Kill::CStrategy_Kill()
{
	bounce = 0;
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
void CStrategy_Kill::Update(Vector3& PlayerPosition, Vector3& theEnemyPosition)
{
	// Decide which state to change to
	int distanceHeroToEnemy = CalculateDistance(PlayerPosition, theEnemyPosition);
	if (distanceHeroToEnemy < AI_STATE_ATTACK*AI_STATE_ATTACK)
	{		
		CurrentState = ATTACK;
	}
	else
		CurrentState = PATROL;

	++bounce;
	if (bounce > 60)
	{
		bounce = 0;
		n = Math::RandIntMinMax(0, 1);
	}

	// Based on the current state, determine enemy behaviour
	switch(CurrentState)
	{
	case PATROL:
		if (bounce < 16)
			theEnemyPosition.x = theEnemyPosition.x + (n ? 1 : -1);
		break;
	case ATTACK:
		theEnemyPosition.x = theEnemyPosition.x + (PlayerPosition.x - theEnemyPosition.x > 0 ? 1.5 : -1.5);
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
	return theDestination.x;
}

/********************************************************************************
 Get the destination for this strategy
 ********************************************************************************/
int CStrategy_Kill::GetDestination_y(void)
{
	return theDestination.y;
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
