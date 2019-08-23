#pragma once
#include "Vector3.h"
#include "Map.h"
#include "Animation.h"

class CPlayerInfo2D : public CAnimation
{
protected:
	static CPlayerInfo2D *s_instance;
	CPlayerInfo2D(void);

public:
	static CPlayerInfo2D *GetInstance()
	{
		if (!s_instance)
			s_instance = new CPlayerInfo2D;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	~CPlayerInfo2D(void);

	// Initialise this class instance
	void Init(void);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the tile size
	void SetTileSize(const int x, const int y);
	// Set the map pointer to this class
	void SetMap(CMap* m_cMap);
	// Get the map pointer to this class
	CMap* GetMap(void);

	// Returns true if the player is on ground
	bool isOnGround(void);
	// Returns true if the player is jumping upwards
	bool isJumpUpwards(void);
	// Returns true if the player is on freefall
	bool isFreeFall(void);
	// Set the player's status to free fall mode
	void SetOnFreeFall(bool isOnFreeFall);
	// Set the player to jumping upwards
	void SetToJumpUpwards(bool isOnJumpUpwards);
	// Stop the player's vertical movement
	void StopVerticalMovement(void);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set Hp
	void SetHp(const int hp);
	// Set Max Hp
	void SetMaxHp(const int hp);
	// Take damage
	void TakeDamage(void);
	// Set m_dJumpSpeed of the player
	void SetJumpSpeed(const double m_dJumpSpeed);
	// Set m_dJumpAcceleration of the player
	void SetJumpAcceleration(const double m_dJumpAcceleration);
	// Set Fall Speed of the player
	void SetFallSpeed(const double m_dFallSpeed);
	// Set Fall Acceleration of the player
	void SetFallAcceleration(const double m_dFallAcceleration);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get Hp
	int GetHp(void) const;
	// Get Max Hp
	int GetMaxHp(void) const;
	// Get Jump Speed of the player
	double GetJumpSpeed(void) const;
	// Get Jump Acceleration of the player
	double GetJumpAcceleration(void) const;
	// Get Fall Speed of the player
	double GetFallSpeed(void) const;
	// Get Fall Acceleration of the player
	double GetFallAcceleration(void) const;
	// Get Stamina of player
	float GetStamina(void) const;
	// Get XP of player
	double GetXP(void) const;
	void AddXP(double xp);
	void XPLevelUp(void);
	// Get lifesteal of player
	double GetLifesteal(void) const;
	void AddLifesteal(int lifesteal);
	void LifestealLifeUp(void);
	// Get Level of player
	double GetLevel(void) const;
	// Get Tile Offset for x-axis
	int GetTileOffset_x(void) const;

	int GetMapOffset_x(void) const;
	// Get Max Boundary
	Vector3 GetMaxBoundary() const;
	// Get Min Boundary
	Vector3 GetMinBoundary() const;

	// Update Jump Upwards
	void UpdateJumpUpwards(double dt = 0.0333f);
	// Update FreeFall
	void UpdateFreeFall(double dt = 0.0333f);
	// Update
	void Update(double dt = 0.0333f);
	// Update tile offsets
	void UpdateTileOffset(void);
	// Update side movements
	void UpdateSideMovements(void);

	// Player movement
	void MoveUpDown(const bool mode, const float timeDiff);
	void MoveLeftRight(const bool mode, const float timeDiff);

	void Attack(const bool mode, const float timeDiff);
	void Die();
	void Heal(bool sound = true, int life = 3);

	bool isCharged(void) const;

	// Check if the player is standing on air
	bool isOnAir(void);

	// Constrain the position within the borders
	void Constrain(void);

	// Get/Set mapFineOffset_x values
	void UpdateMapFineOffset(void);
	int  GetMapFineOffset_x(void) const;
	int checkPosition_X, checkPosition_Y;
	Vector3 position;
	int mapOffset_x, mapOffset_y;

	/* 
		For Parallax Scrolling
	*/
	// Set the rear map pointer to this class
	void SetRearMap(CMap* m_cRearMap);
	// Update the rear map
	void UpdateRearMap(void);
	// Get Tile Offset for x-axis
	int GetRearTileOffset_x(void) const;
	// Get the rear map fine offset X value
	int GetRearMapFineOffset_x(void) const;
	
	// Load this class
	bool Load(const string saveFileName = ".//Image//DM2231.sav");
	// Save this class
	bool Save(const string saveFileName = ".//Image//DM2231.sav");
	// Methods to tokenize a string into a specific data type variable
	Vector3 Token2Vector(const string token);
	double Token2Double(const string token);
	bool Token2Bool(const string token);

	// Check for collision with CGoodies instances
	void UpdateGoodies(const int tileIndex_Column, const int tileIndex_Row);
	// Sound
	void AttackSound(void) const;
	void StepSound(void) const;
	void DoorSound(void) const;
	void DeflectSound(void) const;
	void KillSound(int type) const;

	bool getSecondAttack(void) const;

	bool getSkill(int skill) const;

	enum Skills
	{
		SK_DOUBLE_JUMP,
		SK_DASH_ATTACK,
		SK_DEFLECT,
		SK_DOUBLE_ATTACK,
		SK_CHARGE_ATTACK,
		SK_FAST_ROLL,
		SK_HIGH_JUMP,
		SK_TRIPLE_JUMP,
		SK_AIR_ROLL,
		SK_HEART2,
		SK_ROLL_COST,
		SK_HEART3,
		SK_LIFESTEAL,
		SK_TOTAL
	};

private:
	void InitSound(void) const;

	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	int tileSize_Width, tileSize_Height;
	int hp, maxhp;

	double m_dSpeed, m_dMoveSpeed, m_dRollSpeed;
	double m_dAcceleration;

	bool m_bJumpUpwards;
	bool m_bJumped;
	double m_dJumpSpeed;
	double m_dJumpAcceleration;

	bool m_bJumpKeyHeld;
	bool m_bDoubleJump;
	bool m_bDoubleJumped;
	
	bool m_bFallDownwards;
	double m_dFallSpeed;
	double m_dFallAcceleration;

	double m_dElapsedTime;

	CMap* theMapReference;

	// Codes for Scrolling
	int tileOffset_x, tileOffset_y;
	int mapFineOffset_x, mapFineOffset_y;

	// For Parallax Scrolling
	CMap* theRearMapReference;
	int rearTileOffset_x, rearTileOffset_y;
	int rearMapOffset_x, rearMapOffset_y;
	int rearMapFineOffset_x, rearMapFineOffset_y;

	// BounceTime
	double attackBounceTime;
	double rollBounceTime;
	double dashBounceTime;
	double damageBounceTime;

	// BounceTime Limit
	double attackBounceTimeLimit;
	double rollBounceTimeLimit;
	double rollBounceTimeLimit2;
	double dashBounceTimeLimit;
	double damageBounceTimeLimit;

	bool Roll();

	float stamina;
	bool StaminaDecrease(float decrease);
	void StaminaRegen(float regen, double dt);

	float dashPower;

	float chargeAttack;
	float chargeTime;

	int lifesteal;
	int lifestealLimit;

	// Second Attack
	bool secondAttack;

	// Experience
	double XP;
	double maxXP;
	double level;

	// Skill Tree
	bool skill[SK_TOTAL];
	void InitSkill(void);
};
