#pragma once
#include "Strategy.h"
#include "Vector3.h"
#include "../Projectile/Projectile.h"

class CStrategy_Shoot
	: public CStrategy
{
public:
	CStrategy_Shoot(void);
	~CStrategy_Shoot(void);

	void Update(Vector3& PlayerPosition, Vector3& theEnemyPosition);
	void SetDestination(const float x, const float y);

	int GetDestination_x(void);
	int GetDestination_y(void);
	Vector3 GetDestination(void);

	// The AI states of the enemy
	enum CURRENT_STATE
	{
		IDLE,
		REPEL,
		ATTACK,
		NUM_ENEMY_STATE,
	};
	enum AI_STATE_RANGE
	{
		AI_STATE_REPEL = 64,
		AI_STATE_ATTACK = 112,
		NUM_AI_STATE_RANGE,
	};
	CStrategy_Shoot::CURRENT_STATE GetState(void);
	void SetState(CStrategy_Shoot::CURRENT_STATE);

private:
	// Enemy AI State
	CStrategy_Shoot::CURRENT_STATE CurrentState;
};
