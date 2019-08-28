#include "Strategy.h"
#include <iostream>

using namespace std;

CStrategy::CStrategy()
{
	bounce = 0;
	m = 0;
	n = 0;
	isOnAir = false;
}

CStrategy::~CStrategy()
{
}

void CStrategy::Init(CMap* m_cMap)
{
	theMapReference = m_cMap;
}

void CStrategy::Update(Vector3& theDestination, Vector3& theEnemyPosition, double playerLevel)
{
	cout << "void CStrategy::Update()" << endl;
}

int CStrategy::CalculateDistance(Vector3 theDestination, Vector3 theEnemyPosition)
{
	return static_cast<int>((theDestination.x - theEnemyPosition.x)*(theDestination.x - theEnemyPosition.x) + 
			(theDestination.y - theEnemyPosition.y)*(theDestination.y - theEnemyPosition.y));
}

CStrategy::STRATEGY_TYPE CStrategy::GetType()
{
	return type;
}

double CStrategy::GetLR()
{
	return n;
}

double CStrategy::GetUD()
{
	return m;
}

void CStrategy::SetUD(double d)
{
	m = d;
}

bool CStrategy::GetIsOnAir()
{
	return isOnAir;
}

void CStrategy::SetIsOnAir(bool b)
{
	isOnAir = b;
}