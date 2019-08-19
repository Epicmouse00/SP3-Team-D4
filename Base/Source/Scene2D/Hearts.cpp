#include "Hearts.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Projectile.h"
#include "GoodiesFactory.h"

// Allocating and initializing Slash's static data member.  
// The pointer is allocated but not the object's constructor.
Hearts *Hearts::s_instance = 0;

Hearts::Hearts(void)
	: position(Vector3(0, 0, 8))
{
}

Hearts::~Hearts(void)
{
}

// Initialise this class instance
void Hearts::Init(void)
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
void Hearts::SetPos(const Vector3& pos)
{
	position = pos;
}

Vector3 Hearts::GetPos(void) const
{
	return position;
}

/********************************************************************************
 Update
 ********************************************************************************/
void Hearts::Update(double dt)
{
	UpdateAnimationIndex(0.5f);
}