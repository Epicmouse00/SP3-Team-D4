#include "Enemy.h"
#include <iostream>

/********************************************************************************
 Constructor
 ********************************************************************************/
CEnemy::CEnemy(void)
	: theStrategy(NULL)
	, theENEMYPosition(Vector3(0.0f, 0.0f, 0.0f))
	, theMapReference(nullptr)
{
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
	theENEMYPosition.x=0;
	theENEMYPosition.y=0;
}

/********************************************************************************
 Set position x of the player
 ********************************************************************************/
void CEnemy::SetPos_x(int pos_x)
{
	theENEMYPosition.x = pos_x;
}

/********************************************************************************
 Set position y of the player
 ********************************************************************************/
void CEnemy::SetPos_y(int pos_y)
{
	theENEMYPosition.y = pos_y;
}

/********************************************************************************
 Set position of the player
 ********************************************************************************/
void CEnemy::SetPos(const int pos_x, const int pos_y)
{
	theENEMYPosition.x = pos_x;
	theENEMYPosition.y = pos_y;
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
void CEnemy::SetDestination(const int pos_x, const int pos_y)
{
	theDestination.x = pos_x;
	theDestination.y = pos_y;
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
	return theENEMYPosition.x;
}

/********************************************************************************
 Get position y of the player
 ********************************************************************************/
int CEnemy::GetPos_y(void) const
{
	return theENEMYPosition.y;
}

/********************************************************************************
 Get position of the player in Vector3 form
 ********************************************************************************/
Vector3 CEnemy::GetPos(void) const
{
	return theENEMYPosition;
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
int CEnemy::GetDestination_x(void) const
{
	return theDestination.x;
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
int CEnemy::GetDestination_y(void) const
{
	return theDestination.y;
}

/********************************************************************************
 Get the destination of this enemy as a Vector3
 ********************************************************************************/
Vector3 CEnemy::GetDestination(void) const
{
	return theDestination;
}

/********************************************************************************
 Update
 ********************************************************************************/
void CEnemy::Update(void)
{
	if (theStrategy != NULL)
	{
		theStrategy->Update(theDestination, theENEMYPosition);
		constrain(theStrategy->GetLR(), theStrategy->GetUD());
		if (dynamic_cast<CStrategy_Kill*>(theStrategy)->GetState() == CStrategy_Kill::IDLE ||
			dynamic_cast<CStrategy_Kill*>(theStrategy)->GetState() == CStrategy_Kill::REPEL)
			SetAnimationStatus(C_IDLE_1);
		else if (dynamic_cast<CStrategy_Kill*>(theStrategy)->GetState() == CStrategy_Kill::ATTACK)
			SetAnimationStatus(C_ATTACK_1);
		UpdateAnimationIndex(0.2f);
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
}

void CEnemy::constrain(int a, int b)
{
	int checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
		(int)ceil(theENEMYPosition.y / theMapReference->GetTileSize_Height());

	// Check if the hero can move sideways
	if (/*if left*/1)
	{
		// Find the tile number which the player's left side is on
		int checkPosition_X = (int)((theENEMYPosition.x - (theMapReference->GetTileSize_Width() >> 1)) / theMapReference->GetTileSize_Width());

		if (checkPosition_X >= 0)
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
			{
				theENEMYPosition.x = (checkPosition_X + 1) * theMapReference->GetTileSize_Width() + (theMapReference->GetTileSize_Width() >> 1);
			}
		}
	}
	else if (/*right*/1)
	{
		// Find the tile number which the player's right side is on
		int checkPosition_X = (int)((theENEMYPosition.x + (theMapReference->GetTileSize_Width() >> 1)) / theMapReference->GetTileSize_Width());
		if (checkPosition_X < theMapReference->GetTileSize_Width())
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
			{
				theENEMYPosition.x = (checkPosition_X - 1) * theMapReference->GetTileSize_Width() + (theMapReference->GetTileSize_Width() >> 1);
			}
		}

	}


	if (/*up*/1)
	{
		// Check if the player is stopped by obstacles
		int checkPosition_X = (int)((theENEMYPosition.x - (theMapReference->GetTileSize_Width() >> 1)) / theMapReference->GetTileSize_Width());
		int checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
			(int)ceil((float)(theENEMYPosition.y + (theMapReference->GetTileSize_Height() >> 1)) / theMapReference->GetTileSize_Height());
		if (((int)(theENEMYPosition.x - (theMapReference->GetTileSize_Width() >> 1)) % theMapReference->GetTileSize_Width()) == 0)
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
			{
				// Since the new position does not allow the player to move into, then go back to the old position
				theENEMYPosition.y = (theMapReference->GetNumOfTiles_Height() - checkPosition_Y - 1) * theMapReference->GetTileSize_Height() - (theMapReference->GetTileSize_Height() >> 1);
				// Set on free fall
			}
		}
		else
		{
			if ((theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1) ||
				(theMapReference->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 1))
			{
				// Since the new position does not allow the player to move into, then go back to the old position
				theENEMYPosition.y = (theMapReference->GetNumOfTiles_Height() - checkPosition_Y - 1) * theMapReference->GetTileSize_Height() - (theMapReference->GetTileSize_Height() >> 1);
				// Set on free fall
			}
		}
	}
	else if (/*down*/1)
	{
		// Check if the player is still in mid air...
		int checkPosition_X = (int)((theENEMYPosition.x - (theMapReference->GetTileSize_Width() >> 1)) / theMapReference->GetTileSize_Width());
		int checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
			(int)ceil((float)(theENEMYPosition.y) / theMapReference->GetTileSize_Height());
		if (((int)(theENEMYPosition.x - (theMapReference->GetTileSize_Width() >> 1)) % theMapReference->GetTileSize_Width()) == 0)
		{
			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
			{
				// Since the new position does not allow the player to move into, then go back to the old position
				theENEMYPosition.y = (theMapReference->GetNumOfTiles_Height() - checkPosition_Y) * theMapReference->GetTileSize_Height() + (theMapReference->GetTileSize_Height() >> 1);
			}
		}
		else
		{
			if ((theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1) ||
				(theMapReference->theScreenMap[checkPosition_Y][checkPosition_X + 1] == 1))
			{
				// Since the new position does not allow the player to move into, then go back to the old position
				theENEMYPosition.y = (theMapReference->GetNumOfTiles_Height() - checkPosition_Y) * theMapReference->GetTileSize_Height() + (theMapReference->GetTileSize_Height() >> 1);
			}
		}

	}
}