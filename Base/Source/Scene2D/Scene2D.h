#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "../FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../GenericEntity.h"
#include "../SpriteEntity.h"
#include "Map.h"
#include "../UserInterface.h"
#include "PlayerInfo2D.h"
#include "Slash.h"
#include "E_Button.h"
#include "Enemy.h"
#include "AxeEnemy.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class CScene2D : public Scene
{	
public:
	CScene2D();
	~CScene2D();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void temporopPush(void);

private:
	CScene2D(SceneManager* _sceneMgr); // This is used to register to SceneManager

	enum difficulty
	{
		D_EASY,
		D_NORMAL,
		D_HARD,
		D_EXPERT
	};
	void createWorld(size_t Difficulty, size_t Repeat);

	ShaderProgram* currProg;
	FPSCamera camera;

	static CScene2D* sInstance; // The pointer to the object that gets registered

	// Tile information
	const int kiHalfTileWidth = 8;
	const int kiHalfTileHeight = 8;

	// Scene2D images
	SpriteEntity* Scene2D_Background;
	SpriteEntity* Scene2D_TileGround;
	SpriteEntity* Scene2D_TileDoor;
	SpriteEntity* Scene2D_TileDoor2;
	SpriteEntity* Scene2D_TilePillar_1;
	SpriteEntity* Scene2D_TilePillar_2;
	SpriteEntity* Scene2D_TilePillar_3;
	SpriteEntity* Scene2D_RearStructure;
	SpriteEntity* Scene2D_SpikeL;
	SpriteEntity* Scene2D_SpikeR;
	SpriteEntity* Scene2D_SpikeU;
	SpriteEntity* Scene2D_SpikeD;
	SpriteEntity* Scene2D_Error;
	SpriteEntity* Scene2D_Error2;
	SpriteEntity* Scene2D_LevelUp;
	SpriteEntity** Scene2D_Hero_Animated;
	SpriteEntity** Scene2D_Slash_Animated;
	SpriteEntity* Scene2D_Goodies_TreasureChest;
	SpriteEntity* Scene2D_EnemyHpBar;
	SpriteEntity** Scene2D_E;

	UserInterface* ui;
	// Handle to the tilemaps
	CMap* m_cMap;
	CMap* m_cRearMap;	// Pointer to the rear map

	// This class stores all information about the player / avatar
	CPlayerInfo2D* thePlayerInfo;
	Slash* theSlashInfo;
	E_Button* theButtonInfo;

	// The enemies
	vector<CEnemy*> theEnemy;
	vector<CAxeEnemy*> theAxeEnemy;

	SpriteEntity** Scene2D_Enemy;
	SpriteEntity** Scene2D_AxeEnemy;
	CProjectile* temporop;

	// Render the tile map
	void RenderTileMap(void);
	// Render the rear tile map
	void RenderRearTileMap(void);
	// Render the player
	void RenderPlayer(void);
	// Render the enemy
	void RenderEnemy(void);
	// Create the Goodies
	void CreateGoodies(void);

	void LoadMeshes(void);

	bool doorIsOpen;
};

#endif