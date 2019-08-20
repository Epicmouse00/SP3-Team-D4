#pragma once
#include "Vector3.h"
#include "Map.h"

class CStrategy
{
public:
	CStrategy(void);
	~CStrategy(void);

	virtual void Update(Vector3& theDestination, Vector3& theEnemyPosition) = 0;

	virtual void SetDestination(const float x, const float y) = 0;

	virtual int GetDestination_x(void) = 0;
	virtual int GetDestination_y(void) = 0;
	virtual Vector3 GetDestination(void) = 0;

	int GetLR();
	int GetUD();

	void Init(CMap* m_cMap);

protected:
	int CalculateDistance(Vector3 theDestination, Vector3 theEnemyPosition);

	// The Destination for this Strategy
	Vector3 theDestination;

	CMap* theMapReference;

	int bounce;
	int m; // Up Down
	int n; // Left Right
};
