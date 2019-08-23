#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"

class EntityBase
{
public:
	enum ENTITY_TYPE {
		E_PLAYER = 0,
		E_PLAYER_PROJECTILES,
		E_ENEMY,
		E_ENEMY_PROJECTILES,
		E_CORRUPTION,
		E_TOTAL
	};
	EntityBase(ENTITY_TYPE typeValue = E_TOTAL);
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	inline void SetPosition(const Vector3& _value){ position = _value; };
	inline Vector3 GetPosition(){ return position; };

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

	inline void SetDirection(const Vector3& _value){ theDirection = _value; };
	inline Vector3 GetDirection(){ return theDirection; };

	inline void SetType(const ENTITY_TYPE& _value) { type = _value; };
	inline ENTITY_TYPE GetType(){ return type; };

	inline void SetHp(const int& _value) { hp = _value; };
	inline int GetHp(){ return hp; };
	inline int GetMaxHp(){ return maxHp; };
	bool TakeDamage();

	bool IsDone();
	bool IsDead();
	virtual bool IsAttacking(void);
	virtual void SetIsDone(const bool _value);
	void SetIsDead(const bool _value);

	// Check if this entity has a collider class parent
	virtual bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool _value);

protected:
	ENTITY_TYPE type;
	Vector3 position;
	Vector3 scale;
	Vector3 theDirection;

	float DamageBounceTime;
	int hp;
	int maxHp;
	bool isDone;
	bool isDead;
	bool m_bCollider;
};

#endif // ENTITY_BASE_H