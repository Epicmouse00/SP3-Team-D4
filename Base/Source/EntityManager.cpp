#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	CheckForCollision();

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Render();
	}
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity)
{
	entityList.push_back(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);
		return true;	
	}
	// Return false if not found
	return false;
}

// Constructor
EntityManager::EntityManager()
{
	thePlayerInfo = CPlayerInfo2D::GetInstance();
}

// Destructor
EntityManager::~EntityManager()
{
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{	
	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{

	return false;
}

bool EntityManager::CheckCircleCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	Vector3 dist = ThisEntity->GetPosition() - ThatEntity->GetPosition();
	if (dist.Length() < ThisEntity->GetScale().x + ThatEntity->GetScale().x)
	{
		return true;
	}
	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{

	return false;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(void)
{
	EntityBase* player = new EntityBase;
	player->SetType(player->E_PLAYER);
	player->SetPosition(thePlayerInfo->GetPos());
	player->SetScale(Vector3(1, 1, 1));
	player->SetCollider(true);

	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = entityList.end();
	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		if ((*colliderThis)->HasCollider())
		{
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			if (thisEntity->GetType() != thisEntity->E_PLAYER && thisEntity->GetType() != thisEntity->E_CORRUPTION) {// set based on what can hit the player(specifically the player)
				if (CheckCircleCollision(thisEntity, player) == true)// can use multiple of this based on thePlayerInfoState
				{//we probably are not calling the function checkCircleCollision.. since it needs hard code
					thisEntity->SetIsDone(true);
					if(thePlayerInfo->GetHp()!=0) // Note : temporary placeholder
						thePlayerInfo->SetHp(thePlayerInfo->GetHp()-1);
				}
			}
			else {//this just checks between enemies and player projectiles, Because anything that does not hurt player probably hurts enemies?
				colliderThatEnd = entityList.end();
				for (colliderThat = colliderThis; colliderThat != colliderThatEnd; ++colliderThat)
				{
					if (colliderThat == colliderThis)
						continue;
					if ((*colliderThat)->HasCollider())
					{
						EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
						if (CheckCircleCollision(thisEntity, thatEntity) == true)
						{
							thisEntity->SetIsDone(true);
							thatEntity->SetIsDone(true);
						}
					}
				}
			}
		}
	}
	delete player;
	player = NULL;
	return false;
}
