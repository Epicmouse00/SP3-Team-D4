#include "EntityBase.h"

EntityBase::EntityBase(ENTITY_TYPE typeValue)
	: type(typeValue)
	, position(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f)
	, theDirection(0.f, 0.f, 0.f)
	, hp(1)
	, maxHp(1)
	, DamageBounceTime(0.f)
	, isDone(false)
	, isDead(false)
	, m_bCollider(false)
{
}

EntityBase::~EntityBase()
{
}

void EntityBase::Update(double _dt)
{
	DamageBounceTime += static_cast<float>(_dt);
}

void EntityBase::Render()
{
}

void EntityBase::RenderUI()
{
}

bool EntityBase::IsDead()
{
	return isDead;
}

void EntityBase::SetIsDead(bool _value)
{
	isDead = _value;
}
void EntityBase::TakeDamage()
{
	if (DamageBounceTime > 0.2f)
	{
		--hp;
		DamageBounceTime = 0.f;
	}
	if (!hp)
		SetIsDone(true);
}
bool EntityBase::IsDone()
{
	return isDone;
}

void EntityBase::SetIsDone(bool _value)
{
	isDone = _value;
}

// Check if this entity has a collider class parent
bool EntityBase::HasCollider(void) const
{
	return m_bCollider;
}

// Set the flag to indicate if this entity has a collider class parent
void EntityBase::SetCollider(const bool _value)
{
	m_bCollider = _value;
}
