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
			if (!((*it)->GetType() == EntityBase::E_ENEMY))
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

	CMap* thePlayerInfoGetMap = thePlayerInfo->GetMap(); // Remove lag
	int TileSize_Width = thePlayerInfoGetMap->GetTileSize_Width();
	int TileSize_Height = thePlayerInfoGetMap->GetTileSize_Height();

	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		if ((*colliderThis)->HasCollider() && !(*colliderThis)->IsDone())
		{
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			int thisEntityGetType = thisEntity->GetType(); // Remove lag
			Vector3 thisEntityGetPosition = thisEntity->GetPosition();
			Vector3 thisEntityGetScale = thisEntity->GetScale();

			if (thisEntityGetType == EntityBase::E_CORRUPTION)
			{
				if (thePlayerInfo->position.x < thisEntityGetPosition.x + thisEntityGetScale.x / 2)
				{
					thePlayerInfo->TakeDamage(); // corruption X player
				}
			}
			if (!thisEntity->IsDead() && (thisEntityGetType == EntityBase::E_PLAYER_PROJECTILES || thisEntityGetType == EntityBase::E_ENEMY_PROJECTILES))
			{
				int checkPosition_X = (int)((thisEntityGetPosition.x - (TileSize_Width >> 1)) / TileSize_Width);
				int checkPosition_Y = thePlayerInfoGetMap->GetNumOfTiles_Height() -
					(int)ceil((float)((thisEntityGetPosition.y + (TileSize_Height >> 1)) / TileSize_Height));

				if (thisEntityGetPosition.x > thePlayerInfoGetMap->getNumOfTiles_MapWidth() * TileSize_Width ||
					thisEntityGetPosition.x < 0 ||
					thisEntityGetPosition.y > thePlayerInfoGetMap->getNumOfTiles_MapHeight() * TileSize_Height ||
					thisEntityGetPosition.y < 0)
				{
					thisEntity->SetIsDead(true); // projectiles X World limits
					continue;
				}

				if (((int)(thisEntityGetPosition.x - (TileSize_Width >> 1)) % TileSize_Width) == 0)
				{
					if (thePlayerInfoGetMap->theScreenMap[checkPosition_Y][checkPosition_X] > 0)
					{
						thisEntity->SetIsDead(true); // projectiles X walls
						continue;
					}
				}
				else
				{
					if ((thePlayerInfoGetMap->theScreenMap[checkPosition_Y][checkPosition_X] > 0) ||
						(checkPosition_X + 1 < thePlayerInfoGetMap->GetNumOfTiles_Width() && (thePlayerInfoGetMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] > 0)))
					{
						thisEntity->SetIsDead(true); // projectiles X walls 2
						continue;
					}
				}
				if (theSlashInfo->GetFrameState() != Slash::S_TOTAL)
				{
					if ((thisEntityGetPosition - theSlashInfo->position).Length() < thisEntityGetScale.x / 2 + TileSize_Width / 2 * 3 / 2)//hitbox X 1.5
					{
						if (thisEntityGetType == thisEntity->E_ENEMY_PROJECTILES && thePlayerInfo->getSkill(CPlayerInfo2D::SK_DEFLECT) &&
							(thisEntityGetPosition - theSlashInfo->position).Length() < thisEntityGetScale.x / 2)
						{
							thisEntity->SetType(EntityBase::E_PLAYER_PROJECTILES);// Player Slash X Enemy Proj
							Create::Projectile("Crystal_Projectile_2"
								, thisEntityGetPosition
								, thisEntityGetScale
								, -thisEntity->GetDirection()//cuz he scales by direction
								, 1.f, 100.f, EntityBase::E_PLAYER_PROJECTILES);
							thisEntity->SetIsDead(true);
							thePlayerInfo->DeflectSound();
							continue;
						}
					}
				}
				if ((thisEntityGetPosition - thePlayerInfo->position).Length() < thisEntityGetScale.x / 2 + TileSize_Width / 2)// Only run this when enemy is attacking
				{
					if (thisEntityGetType == thisEntity->E_ENEMY_PROJECTILES && !thePlayerInfo->isRolling())
					{
						thePlayerInfo->TakeDamage(); // Player ~Pogo~(still take dmg) X Projectiles 
						thisEntity->SetIsDead(true);
					}
				}
			}
			if (thisEntityGetType == thisEntity->E_ENEMY)// Only run this when enemy is attacking
			{
				if (!thePlayerInfo->isRolling() && (thisEntityGetPosition - thePlayerInfo->position).Length() < thisEntityGetScale.x / 2 + TileSize_Width / 2)
				{
					if (thePlayerInfo->isPogo())
					{
						thisEntity->SetIsDone(true); // Player Pogo X Enemy (instant kill)

						if (thisEntity->IsDone())
						{
							thePlayerInfo->AddXP();
							thePlayerInfo->AddLifesteal();
						}
						continue;
					}
					else
						if (thisEntity->IsAttacking() && !(thePlayerInfo->GetDashPower())) //invincible while dashing (still take dmg X proj)
							thePlayerInfo->TakeDamage(); // Enemy atack X Player
				}
				if (theSlashInfo->GetFrameState() != Slash::S_TOTAL)
				{
					if ((thisEntityGetPosition - theSlashInfo->position).Length() < thisEntityGetScale.x / 2 + TileSize_Width / 2 * 3 / 2)//hitbox X 1.5
					{
						int damage = 1;
						if (theSlashInfo->isCharge())
						{
							damage = 3;
						}
						if (thisEntity->TakeDamage(damage)) // Player slash X Enemy
							thePlayerInfo->AddLifesteal();
						if (thisEntity->IsDone())
							thePlayerInfo->AddXP();
					}
				}
			}
			for (colliderThat = entityList.begin(); colliderThat != colliderThisEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;
				if ((*colliderThat)->HasCollider())
				{
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);

					if (!thisEntity->IsDead())
					{
						if (thatEntity->GetType() == EntityBase::E_CORRUPTION)
						{
							if (thisEntityGetPosition.x < thatEntity->GetPosition().x + thatEntity->GetScale().x / 2 - TileSize_Width / 2)
							{
								if (thisEntityGetType == EntityBase::E_ENEMY_PROJECTILES)
									thisEntity->SetIsDead(true); // corruption X Enemy Projectile
								else if (thisEntityGetType == EntityBase::E_ENEMY)
									thisEntity->SetIsDone(true); // corruption X Enemy
							}

						}
						
						if (thatEntity->GetType() == EntityBase::E_PLAYER_PROJECTILES)
						{
							if (CheckCircleCollision(thisEntity, thatEntity) == true)
							{
								if (thisEntityGetType == thisEntity->E_ENEMY_PROJECTILES)
									thisEntity->SetIsDead(true);
								else if (thisEntityGetType == EntityBase::E_ENEMY)
								{
									if (thisEntity->TakeDamage()) // Player proj X Enemy
										thePlayerInfo->AddLifesteal();
									if (thisEntity->IsDone())
										thePlayerInfo->AddXP();
								}
								thatEntity->SetIsDead(true);
								break; // break cuz that enemy is already dead.. no need to check against player
							}
						}
					}
				}
			}
		}
	}
	return false;
}
