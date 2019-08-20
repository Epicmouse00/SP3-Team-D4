#include "Strategy.h"
#include <iostream>

using namespace std;

CStrategy::CStrategy()
{
	

	bounce = 0;
	m = 0;
	n = 0;
}

CStrategy::~CStrategy()
{
}

void CStrategy::Init(CMap* m_cMap)
{
	theMapReference = m_cMap;
}

void CStrategy::Update(Vector3& theDestination, Vector3& theEnemyPosition)
{
	cout << "void CStrategy::Update()" << endl;
}

int CStrategy::CalculateDistance(Vector3 theDestination, Vector3 theEnemyPosition)
{
	return ((theDestination.x - theEnemyPosition.x)*(theDestination.x - theEnemyPosition.x) + 
			(theDestination.y - theEnemyPosition.y)*(theDestination.y - theEnemyPosition.y));
}

int CStrategy::GetLR()
{
	return n;
}

int CStrategy::GetUD()
{
	return m;
}