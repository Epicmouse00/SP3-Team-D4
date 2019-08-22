#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "Projectile/Projectile.h"
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
		if ((*it)->IsDead())
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
	theSlashInfo = Slash::GetInstance();
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
	if (dist.Length() < ThisEntity->GetScale().x/2 + ThatEntity->GetScale().x/2)
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

	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = entityList.end();
	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		if ((*colliderThis)->HasCollider() && !(*colliderThis)->IsDone())
		{
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			if (thisEntity->GetType() == EntityBase::E_CORRUPTION)
			{
				if (thisEntity->GetPosition().x - thisEntity->GetScale().x / 2 - 16/2 > 64 * 16)
				{
					thisEntity->SetIsDone(true); // end of map X corruption
				}
				// set based on what can hit the player(specifically the player)
				if (thePlayerInfo->position.x < thisEntity->GetPosition().x + thisEntity->GetScale().x / 2)
				{
					thePlayerInfo->TakeDamage(); // corruption X player
				}
			}

			colliderThatEnd = entityList.end();
			for (colliderThat = colliderThis; colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;
				if ((*colliderThat)->HasCollider())
				{
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);

					if (!thisEntity->IsDead() && !thisEntity->IsDone() && (thisEntity->GetType() == EntityBase::E_ENEMY || thisEntity->GetType() == EntityBase::E_ENEMY_PROJECTILES))
					{
						if (thatEntity->GetType() == EntityBase::E_CORRUPTION)
						{
							if (thisEntity->GetPosition().x < thatEntity->GetPosition().x + thatEntity->GetScale().x / 2 - 16 / 2)
							{
								if (thisEntity->GetType() == EntityBase::E_ENEMY_PROJECTILES)
									thisEntity->SetIsDead(true); // corruption X Enemy Projectile
								else
									thisEntity->SetIsDone(true); // corruption X Enemy
							}

						}
						if (theSlashInfo->GetFrameState() != Slash::S_TOTAL)
						{
							if ((thisEntity->GetPosition() - theSlashInfo->position).Length() < thisEntity->GetScale().x / 2 + 16 / 2 * 3 / 2)//hitbox X 1.5
							{
								if (thisEntity->GetType() == thisEntity->E_ENEMY_PROJECTILES)
								{
									thisEntity->SetType(EntityBase::E_PLAYER_PROJECTILES);// Player Slash X Enemy Proj
									thisEntity->SetDirection(-thisEntity->GetDirection());
									Create::Projectile("UI_BOX"//change this
										, thisEntity->GetPosition()
										, thisEntity->GetScale()
										, -thisEntity->GetDirection()
										, 10.f, 4.f, EntityBase::E_PLAYER_PROJECTILES);
									thisEntity->SetIsDead(true);
								}
								else
								{
									thisEntity->TakeDamage();// Player slash X Enemy
									if (thisEntity->IsDone()) 
										thePlayerInfo->AddXP(1);
									break;
								}
							}
						}
						if (thatEntity->GetType() == EntityBase::E_PLAYER_PROJECTILES)
						{
							if (CheckCircleCollision(thisEntity, thatEntity) == true)
							{
								thisEntity->SetIsDone(true); // Player proj X Enemy
								thatEntity->SetIsDead(true);
								break; // break cuz that enemy is already dead.. no need to check against player
							}
						}
						if ((thisEntity->GetPosition() - thePlayerInfo->position).Length() < thisEntity->GetScale().x / 2 + 16 / 2)// Only run this when enemy is attacking
						{
							if (thePlayerInfo->isPogo()) // Player Pogo X Enemy
							{
								thisEntity->SetIsDone(true);
								break;
							}
							//if(thisEntity->)// some anim thing
							//{
							//	thePlayerInfo->TakeDamage(); // Enemy atack X Player
							//}
							break;
						}
					}
				}
			}
		}
	}
	return false;
}
