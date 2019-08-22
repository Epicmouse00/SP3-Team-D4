#include "Projectile.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Scene2D/PlayerInfo2D.h"

CProjectile::CProjectile(void)
	: modelMesh(NULL)
	, m_bStatus(false)
	, m_fLifetime(-1.0f)
	, m_fSpeed(10.0f)
{
}

CProjectile::CProjectile(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
	, m_bStatus(false)
	, m_fLifetime(-1)
	, m_fSpeed(10.0f)
{
}

CProjectile::~CProjectile(void)
{
	modelMesh = NULL;
}

// Activate the projectile. true == active, false == inactive
void CProjectile::SetStatus(const bool m_bStatus)
{
	if (m_bStatus == false)
		m_fLifetime = -1;
	this->m_bStatus = m_bStatus;
}

// get status of the projectile. true == active, false == inactive
bool CProjectile::GetStatus(void) const
{
	return m_bStatus;
}

// Set the position and direction of the projectile
void CProjectile::Set(Vector3 theNewPosition, Vector3 theNewDirection, const float m_fLifetime, const float m_fSpeed)
{
	position = theNewPosition;
	theDirection = theNewDirection;
	this->m_fLifetime = m_fLifetime;
	this->m_fSpeed = m_fSpeed;
}

// Get the direction of the projectile
Vector3 CProjectile::GetDirection(void)
{
	return theDirection;
}

// Get the lifetime of the projectile
float CProjectile::GetLifetime(void) const
{
	return m_fLifetime;
}

// Get the speed of the projectile
float CProjectile::GetSpeed(void) const
{
	return m_fSpeed;
}

// Set the direction of the projectile
void CProjectile::SetDirection(Vector3 theNewDirection)
{
	theDirection = theNewDirection;
}

// Set the lifetime of the projectile
void CProjectile::SetLifetime(const float m_fLifetime)
{
	this->m_fLifetime = m_fLifetime;
}

// Set the speed of the projectile
void CProjectile::SetSpeed(const float m_fSpeed)
{
	this->m_fSpeed = m_fSpeed;
}

// Update the status of this projectile
void CProjectile::Update(double dt)
{
	if (m_bStatus == false)
		return;

	// Update TimeLife of projectile. Set to inactive if too long
	if (!(type == ENTITY_TYPE::E_CORRUPTION))
		m_fLifetime -= (float)dt;

	if (m_fLifetime < 0.0f)
	{
		SetStatus(false);
		SetIsDone(true);	// This method is to inform the EntityManager that it should remove this instance
		return;
	}

	if (!isDone)
	{
		// Update Position
		position.Set(position.x + (float)(theDirection.x * dt * m_fSpeed),
			position.y + (float)(theDirection.y * dt * m_fSpeed),
			position.z + (float)(theDirection.z * dt * m_fSpeed));
	}
}


// Render this projectile
void CProjectile::RenderUI(void)
{
	if (m_bStatus == false)
		return;

	if (m_fLifetime < 0.0f)
		return;

	CPlayerInfo2D* thePlayerInfo = CPlayerInfo2D::GetInstance();

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x-thePlayerInfo->GetMapOffset_x(), position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

// Create a projectile and add it into EntityManager
CProjectile* Create::Projectile(const std::string& _meshName, 
								const Vector3& _position, 
								const Vector3& _scale, 
								const Vector3& _direction, 
								const float m_fLifetime, 
								const float m_fSpeed,
								EntityBase::ENTITY_TYPE _type)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CProjectile* result = new CProjectile(modelMesh);
	result->Set(_position, _direction, m_fLifetime, m_fSpeed);
	result->SetType(_type);
	result->SetScale(_scale);
	result->SetStatus(true);
	result->SetCollider(true);
	EntityManager::GetInstance()->AddEntity(result);

	return result;
}