#include "Scene2D.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "../Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "../EntityManager.h"

#include "../GenericEntity.h"
#include "../TextEntity.h"
#include "../SpriteEntity.h"
#include "../Light.h"
#include "Strategy_Kill.h"
#include "Strategy_Shoot.h"

#include "GoodiesFactory.h"

#include <iostream>
using namespace std;

CScene2D* CScene2D::sInstance = new CScene2D(SceneManager::GetInstance());

CScene2D::CScene2D()
	: m_cMap(NULL)
	, m_cRearMap(NULL)
	, thePlayerInfo(NULL)
	, theSlashInfo(NULL)
	, theButtonInfo(NULL)
	, theEnemy(NULL)
	, theAxeEnemy(NULL)
	, doorIsOpen(false)
{
}

CScene2D::CScene2D(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("Scene2D", this);
}

void CScene2D::createWorld(size_t Difficulty, size_t Repeat)
{
	for (size_t i = 0; i < Repeat; ++i)
	{
		int initialWidth = m_cMap->getNumOfTiles_MapWidth();
		std::ostringstream ss;
		switch (Difficulty)
		{
		case 100: // 100 is for testing
			{
				ss << "Levels//Test.csv";
			}
			break;
		case D_EASY:
			{
				ss << "Levels//Easy" << Math::RandIntMinMax(1, 3) << ".csv";
			}
			break;
		case D_NORMAL:
			{
				ss << "Levels//Normal" << Math::RandIntMinMax(1, 5) << ".csv";
			}
			break;
		case D_HARD:
			{
				ss << "Levels//Hard" << Math::RandIntMinMax(1, 5) << ".csv";
			}
			break;
		case D_EXPERT:
			{
				ss << "Levels//MapDesign" << Math::RandIntMinMax(1, 2) << ".csv";
			}
			break;
		default:
			break;
		}
		m_cMap->LoadMap(ss.str());
		// unlimited??? (limited by max vector size?)
		for (int width = initialWidth; width < m_cMap->getNumOfTiles_MapWidth(); ++width)
		{
			for (int height = 0; height < m_cMap->getNumOfTiles_MapHeight(); ++height)
			{
				switch (m_cMap->theScreenMap[height][width])
				{
				case 101:
					theEnemy.push_back(Create::EnemyEntity(m_cMap, new CStrategy_Shoot(), false
						, Vector3(static_cast<float>(width * m_cMap->GetTileSize_Width() + (m_cMap->GetTileSize_Width() >> 1)), static_cast<float>(232 - height * m_cMap->GetTileSize_Height()))));
					break;
				case 102:
					theAxeEnemy.push_back(Create::AxeEnemyEntity(m_cMap, new CStrategy_Kill(), false
						, Vector3(static_cast<float>(width * m_cMap->GetTileSize_Width() + (m_cMap->GetTileSize_Width() >> 1)), static_cast<float>(232 - height * m_cMap->GetTileSize_Height()))));
					break;
				default:
					break;
				}
			}
		}
		
	}
}

CScene2D::~CScene2D()
{
	//delete spritesheet;
	//spritesheet = NULL;
	// Delete the sprites
	//delete Scene2D_Goodies_TreasureChest;
	//Scene2D_Goodies_TreasureChest = NULL;
	delete Scene2D_Background;
	Scene2D_Background = NULL;
	delete Scene2D_TileGround;
	Scene2D_TileGround = NULL;
	delete Scene2D_TileDoor;
	Scene2D_TileDoor = NULL;
	delete Scene2D_TileDoor2;
	Scene2D_TileDoor2 = NULL;
	delete Scene2D_TilePillar_1;
	Scene2D_TilePillar_1 = NULL;
	delete Scene2D_TilePillar_2;
	Scene2D_TilePillar_2 = NULL;
	delete Scene2D_TilePillar_3;
	Scene2D_TilePillar_3 = NULL;
	delete Scene2D_RearStructure;
	Scene2D_RearStructure = NULL;
	delete Scene2D_SpikeL;
	Scene2D_SpikeL = NULL;
	delete Scene2D_SpikeR;
	Scene2D_SpikeR = NULL;
	delete Scene2D_SpikeU;
	Scene2D_SpikeU = NULL;
	delete Scene2D_SpikeD;
	Scene2D_SpikeD = NULL;
	delete Scene2D_Error;
	Scene2D_Error = NULL;
	delete Scene2D_Error2;
	Scene2D_Error2 = NULL;
	delete Scene2D_LevelUp;
	Scene2D_LevelUp = NULL;
	delete Scene2D_EnemyHpBar;
	Scene2D_EnemyHpBar = NULL;


	for (int i = 0; i < theButtonInfo->GetFrameTotal(); ++i)
	{
		delete Scene2D_E[i];
		Scene2D_E[i] = NULL;
	}
	delete Scene2D_E;
	Scene2D_E = NULL;

	delete theButtonInfo;
	theButtonInfo = NULL;

	for (int i = 0; i < CAnimationCrystal::C_TOTAL; ++i)
	{
		delete Scene2D_Enemy[i];
		Scene2D_Enemy[i] = NULL;
	}
	delete Scene2D_Enemy;
	Scene2D_Enemy = NULL;

	for (int i = 0; i < CAnimationAxe::A_TOTAL; ++i)
	{
		delete Scene2D_AxeEnemy[i];
		Scene2D_AxeEnemy[i] = NULL;
	}
	delete Scene2D_AxeEnemy;
	Scene2D_AxeEnemy = NULL;

	for (int i = 0; i < thePlayerInfo->GetFrameTotal(); ++i)
	{
		delete Scene2D_Hero_Animated[i];
		Scene2D_Hero_Animated[i] = NULL;
	}
	delete Scene2D_Hero_Animated;
	Scene2D_Hero_Animated = NULL;

	for (int i = 0; i < theSlashInfo->GetFrameTotal(); ++i)
	{
		delete Scene2D_Slash_Animated[i];
		Scene2D_Slash_Animated[i] = NULL;
	}
	delete Scene2D_Slash_Animated;
	Scene2D_Slash_Animated = NULL;

	if (thePlayerInfo->DropInstance() == false)
	{
		cout << "CScene2D: Unable to drop CPlayerInfo2D class" << endl;
	}
	if(ui)
	{
		delete ui;
		ui = NULL;
	}
	if (theSlashInfo->DropInstance() == false)
	{
		cout << "CScene2D: Unable to drop Slash class" << endl;
	}

	if (m_cRearMap)
	{
		delete m_cRearMap;
		m_cRearMap = NULL;
	}
	if (m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}
}

void CScene2D::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default",
		"Shader//Texture.vertexshader",
		"Shader//Texture.fragmentshader");

	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");

	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	glDisable(GL_DEPTH_TEST);
	// Enable blend mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load all the meshes
	LoadMeshes();

	//MeshBuilder::GetInstance()->GenerateSpriteAnimation("sprite", 16, 16);
	//MeshBuilder::GetInstance()->GetMesh("sprite")->textureID = LoadTGA("Image//spritesheet.tga");

	// Create entities into the scene
	//Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	//Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(Application::GetInstance().GetWindowHeight()-16, Application::GetInstance().GetWindowWidth(), 15, 64, 240, 1024);
	Math::InitRNG();
	//createWorld(); // For Test.csv
	createWorld(D_EASY, 5);
	createWorld(D_NORMAL, 5);
	createWorld(D_HARD, 10);
	createWorld(D_EXPERT, 10);
	
	// Create the Goodies
	CreateGoodies();
	{
		CMap* rearMapRef = new CMap();
		rearMapRef->Init(Application::GetInstance().GetWindowHeight(), Application::GetInstance().GetWindowWidth(), 15, 64, 240, 1024);
		rearMapRef->LoadMap("Levels//RearMapDesign.csv");
		m_cRearMap = new CMap();
		m_cRearMap->AddRearFile(m_cMap,rearMapRef);
		delete rearMapRef;
	}
	// Create the playerinfo instance, which manages all information about the player
	thePlayerInfo = CPlayerInfo2D::GetInstance();
	thePlayerInfo->Init();
	thePlayerInfo->SetPos(Vector3(64.0f + kiHalfTileWidth, 80.f + kiHalfTileHeight));
	thePlayerInfo->SetSpawn();
	//thePlayerInfo->SetBoundary(Vector3(210.f, 230.0f, 0.0f), Vector3(10.0f, 10.0f, 0.0f));
	thePlayerInfo->SetBoundary(Vector3(static_cast<float>(m_cMap->getScreenWidth()) / 2, static_cast<float>(m_cMap->getScreenHeight()), 0.0f)
		, Vector3(static_cast<float>(m_cMap->getScreenWidth()) / 2, 0, 0.0f));
	thePlayerInfo->SetTileSize(m_cMap->GetTileSize_Width(), m_cMap->GetTileSize_Height());
	thePlayerInfo->SetMap(m_cMap);
	thePlayerInfo->SetRearMap(m_cRearMap);

	Scene2D_corruption = Create::Projectile("Corrupt_temp",
		Vector3(static_cast<float>(thePlayerInfo->GetPos().x - m_cMap->getScreenWidth()), static_cast<float>(m_cMap->getScreenHeight()) / 2, 0),
		Vector3(static_cast<float>(m_cMap->getScreenWidth()),
			static_cast<float>(m_cMap->getScreenHeight())),
		Vector3(1, 0, 0),
		1.f, 30.f, EntityBase::ENTITY_TYPE::E_CORRUPTION);

	ui = new UserInterface(Scene2D_corruption);
	theSlashInfo = Slash::GetInstance();
	theSlashInfo->Init();

	theButtonInfo = E_Button::GetInstance();
	theButtonInfo->Init();

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;

	// Create the background image
	Scene2D_Background = Create::Sprite2DObject("SCENE2D_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(360.0f, 240.0f, 0.0f));
	Scene2D_TileGround = Create::Sprite2DObject("Tile_1111",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_TileDoor = Create::Sprite2DObject("Tile_Door_Close",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_TileDoor2 = Create::Sprite2DObject("Tile_Door_Open",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_TilePillar_1 = Create::Sprite2DObject("Tile_Pillar_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_TilePillar_2 = Create::Sprite2DObject("Tile_Pillar_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_TilePillar_3 = Create::Sprite2DObject("Tile_Pillar_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_RearStructure = Create::Sprite2DObject("Tile_BG",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_SpikeL = Create::Sprite2DObject("Tile_Spike_Left",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_SpikeR = Create::Sprite2DObject("Tile_Spike_Right",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_SpikeU = Create::Sprite2DObject("Tile_Spike_Up",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_SpikeD = Create::Sprite2DObject("Tile_Spike_Down",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Error = Create::Sprite2DObject("Tile_Null_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Error2 = Create::Sprite2DObject("Tile_Null_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_LevelUp = Create::Sprite2DObject("Tile_LevelUp",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_EnemyHpBar = Create::Sprite2DObject("Stamina_Amber",//temp
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(320.f, 240.f, 0.f));

	Scene2D_E = new SpriteEntity*[theButtonInfo->GetFrameTotal()];
	Scene2D_E[0] = Create::Sprite2DObject("E_Button_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_E[1] = Create::Sprite2DObject("E_Button_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_E[2] = Create::Sprite2DObject("X_Button_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_E[3] = Create::Sprite2DObject("X_Button_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	
	Scene2D_Hero_Animated = new SpriteEntity*[thePlayerInfo->GetFrameTotal()];
	Scene2D_Hero_Animated[0] = Create::Sprite2DObject("Lonin_Right_Idle_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[1] = Create::Sprite2DObject("Lonin_Right_Idle_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[2] = Create::Sprite2DObject("Lonin_Right_Charge",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[3] = Create::Sprite2DObject("Lonin_Left_Idle_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[4] = Create::Sprite2DObject("Lonin_Left_Idle_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[5] = Create::Sprite2DObject("Lonin_Left_Charge",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[6] = Create::Sprite2DObject("Lonin_Right_Run_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[7] = Create::Sprite2DObject("Lonin_Right_Run_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[8] = Create::Sprite2DObject("Lonin_Left_Run_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[9] = Create::Sprite2DObject("Lonin_Left_Run_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[10] = Create::Sprite2DObject("Lonin_Right_Attack_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[11] = Create::Sprite2DObject("Lonin_Right_Attack_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[12] = Create::Sprite2DObject("Lonin_Left_Attack_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[13] = Create::Sprite2DObject("Lonin_Left_Attack_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[14] = Create::Sprite2DObject("Lonin_Right_Charged_Attack_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[15] = Create::Sprite2DObject("Lonin_Right_Charged_Attack_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[16] = Create::Sprite2DObject("Lonin_Right_Charged_Attack_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[17] = Create::Sprite2DObject("Lonin_Left_Charged_Attack_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[18] = Create::Sprite2DObject("Lonin_Left_Charged_Attack_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[19] = Create::Sprite2DObject("Lonin_Left_Charged_Attack_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[20] = Create::Sprite2DObject("Lonin_Right_Jump_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[21] = Create::Sprite2DObject("Lonin_Right_Jump_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[22] = Create::Sprite2DObject("Lonin_Left_Jump_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[23] = Create::Sprite2DObject("Lonin_Left_Jump_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[24] = Create::Sprite2DObject("Lonin_Right_Fall_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[25] = Create::Sprite2DObject("Lonin_Right_Fall_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[26] = Create::Sprite2DObject("Lonin_Left_Fall_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[27] = Create::Sprite2DObject("Lonin_Left_Fall_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[28] = Create::Sprite2DObject("Lonin_Right_Pogo_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[29] = Create::Sprite2DObject("Lonin_Right_Pogo_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[30] = Create::Sprite2DObject("Lonin_Left_Pogo_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[31] = Create::Sprite2DObject("Lonin_Left_Pogo_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[32] = Create::Sprite2DObject("Lonin_Right_Roll_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[33] = Create::Sprite2DObject("Lonin_Right_Roll_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[34] = Create::Sprite2DObject("Lonin_Right_Roll_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[35] = Create::Sprite2DObject("Lonin_Right_Roll_4",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[36] = Create::Sprite2DObject("Lonin_Left_Roll_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[37] = Create::Sprite2DObject("Lonin_Left_Roll_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[38] = Create::Sprite2DObject("Lonin_Left_Roll_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[39] = Create::Sprite2DObject("Lonin_Left_Roll_4",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[40] = Create::Sprite2DObject("Lonin_Right_Roll_4",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[41] = Create::Sprite2DObject("Lonin_Right_Roll_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[42] = Create::Sprite2DObject("Lonin_Left_Roll_4",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Hero_Animated[43] = Create::Sprite2DObject("Lonin_Left_Roll_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));

	Scene2D_Slash_Animated = new SpriteEntity*[theSlashInfo->GetFrameTotal()];
	Scene2D_Slash_Animated[0] = Create::Sprite2DObject("Slash_Up_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[1] = Create::Sprite2DObject("Slash_Up_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[2] = Create::Sprite2DObject("Slash_Down_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[3] = Create::Sprite2DObject("Slash_Down_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[4] = Create::Sprite2DObject("Slash_Left_1_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[5] = Create::Sprite2DObject("Slash_Left_1_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[6] = Create::Sprite2DObject("Slash_Left_2_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[7] = Create::Sprite2DObject("Slash_Left_2_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[8] = Create::Sprite2DObject("Slash_Left_3_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[9] = Create::Sprite2DObject("Slash_Left_3_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[10] = Create::Sprite2DObject("Slash_Left_3_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[11] = Create::Sprite2DObject("Slash_Right_1_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[12] = Create::Sprite2DObject("Slash_Right_1_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[13] = Create::Sprite2DObject("Slash_Right_2_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[14] = Create::Sprite2DObject("Slash_Right_2_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[15] = Create::Sprite2DObject("Slash_Right_3_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[16] = Create::Sprite2DObject("Slash_Right_3_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Slash_Animated[17] = Create::Sprite2DObject("Slash_Right_3_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));

	Scene2D_Enemy = new SpriteEntity*[CAnimationCrystal::C_TOTAL]; // Enemy stuff
	Scene2D_Enemy[0] = Create::Sprite2DObject("Crystal_Attack_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Enemy[1] = Create::Sprite2DObject("Crystal_Attack_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Enemy[2] = Create::Sprite2DObject("Crystal_Die_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Enemy[3] = Create::Sprite2DObject("Crystal_Die_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Enemy[4] = Create::Sprite2DObject("Crystal_Idle_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_Enemy[5] = Create::Sprite2DObject("Crystal_Idle_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));

	Scene2D_AxeEnemy = new SpriteEntity*[CAnimationAxe::A_TOTAL];
	Scene2D_AxeEnemy[0] = Create::Sprite2DObject("Axe_Attack_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_AxeEnemy[1] = Create::Sprite2DObject("Axe_Attack_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_AxeEnemy[2] = Create::Sprite2DObject("Axe_Attack_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_AxeEnemy[3] = Create::Sprite2DObject("Axe_Attack_4",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_AxeEnemy[4] = Create::Sprite2DObject("Axe_Attack_5",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_AxeEnemy[5] = Create::Sprite2DObject("Axe_Die_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_AxeEnemy[6] = Create::Sprite2DObject("Axe_Die_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_AxeEnemy[7] = Create::Sprite2DObject("Axe_Idle_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_AxeEnemy[8] = Create::Sprite2DObject("Axe_Idle_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_AxeEnemy[9] = Create::Sprite2DObject("Axe_Run_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
	Scene2D_AxeEnemy[10] = Create::Sprite2DObject("Axe_Run_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(16.0f, 16.0f, 0.0f));
}

void CScene2D::Update(double dt)
{
	if (ui->Update(dt))
	{
		// Update our entities
		EntityManager::GetInstance()->Update(dt);
		

		// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
		if (KeyboardController::GetInstance()->IsKeyDown('1'))
			glEnable(GL_CULL_FACE);
		if (KeyboardController::GetInstance()->IsKeyDown('2'))
			glDisable(GL_CULL_FACE);
		if (KeyboardController::GetInstance()->IsKeyDown('3'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (KeyboardController::GetInstance()->IsKeyDown('4'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// if the left mouse button was released
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
		{
			cout << "Left Mouse Button was released!" << endl;
		}
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
		{
			cout << "Right Mouse Button was released!" << endl;
		}
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
		{
			cout << "Middle Mouse Button was released!" << endl;
		}
		if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
		{
			cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
		}
		if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
		{
			cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
		}
		// <THERE>

		// Update the thePlayerInfo
		thePlayerInfo->Update(dt);
		theSlashInfo->Update(dt);
		theButtonInfo->Update(dt);
		// Update the enemies
		for (size_t i = 0; i < theEnemy.size(); ++i)
		{
			if (theEnemy[i]->IsDead())
			{
				delete theEnemy[i];
				theEnemy[i] = nullptr;
				theEnemy.erase(theEnemy.begin() + i);
				--i;
				continue;
			}
			if (!theEnemy[i]->IsDone())
				theEnemy[i]->SetDestination(Vector3(thePlayerInfo->GetPos().x,
					thePlayerInfo->GetPos().y,
					0));
			theEnemy[i]->Update();
		}
		for (size_t i = 0; i < theAxeEnemy.size(); ++i)
		{
			if (theAxeEnemy[i]->IsDead())
			{
				delete theAxeEnemy[i];
				theAxeEnemy[i] = nullptr;
				theAxeEnemy.erase(theAxeEnemy.begin() + i);
				--i;
				continue;
			}
			if (!theAxeEnemy[i]->IsDone())
				theAxeEnemy[i]->SetDestination(Vector3(thePlayerInfo->GetPos().x,
					thePlayerInfo->GetPos().y,
					0));
			theAxeEnemy[i]->Update();
		}

		if (m_cMap->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X] == 3 ||
			(thePlayerInfo->checkPosition_X + 1 < m_cMap->GetNumOfTiles_Width() && m_cMap->theScreenMap[thePlayerInfo->checkPosition_Y][thePlayerInfo->checkPosition_X + 1] == 3))
		{
			if (doorIsOpen)
				thePlayerInfo->DoorSound();
			doorIsOpen = false;
		}
		else
		{
			if (!doorIsOpen)
				thePlayerInfo->DoorSound();
			doorIsOpen = true;
		}

		if (Scene2D_corruption->GetPosition().x > static_cast<float>(thePlayerInfo->GetMapOffset_x() + m_cMap->getScreenWidth() / 2))
			Scene2D_corruption->SetPosition(Vector3(static_cast<float>(thePlayerInfo->GetMapOffset_x() + m_cMap->getScreenWidth() / 2),Scene2D_corruption->GetPosition().y));

		if (ui->IsRespawned())
		{
			EntityManager::GetInstance()->ClearEntityList();
			while (theEnemy.size())
			{
				delete theEnemy[0];
				theEnemy[0] = nullptr;
				theEnemy.erase(theEnemy.begin());
			}
			while (theAxeEnemy.size())
			{
				delete theAxeEnemy[0];
				theAxeEnemy[0] = nullptr;
				theAxeEnemy.erase(theAxeEnemy.begin());
			}

			for (int width = thePlayerInfo->GetTileOffset_x(); width < m_cMap->getNumOfTiles_MapWidth(); ++width)
			{
				for (int height = 0; height < m_cMap->getNumOfTiles_MapHeight(); ++height)
				{
					switch (m_cMap->theScreenMap[height][width])
					{
					case 101:
						theEnemy.push_back(Create::EnemyEntity(m_cMap, new CStrategy_Shoot(), false
							, Vector3(static_cast<float>(width * m_cMap->GetTileSize_Width() + (m_cMap->GetTileSize_Width() >> 1)), static_cast<float>(232 - height * m_cMap->GetTileSize_Height()))));
						break;
					case 102:
						theAxeEnemy.push_back(Create::AxeEnemyEntity(m_cMap, new CStrategy_Kill(), false
							, Vector3(static_cast<float>(width * m_cMap->GetTileSize_Width() + (m_cMap->GetTileSize_Width() >> 1)), static_cast<float>(232 - height * m_cMap->GetTileSize_Height()))));
						break;
					default:
						break;
					}
				}
			}
		}

		GraphicsManager::GetInstance()->UpdateLights(dt);
	}
}

void CScene2D::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	//GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Render the required entities
	//EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0, 360,//temp
															  0, 240 - 16,
															  -10, 10);
	//GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(),
	//														  0, Application::GetInstance().GetWindowHeight() - 16,
	//														  -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	if (ui->GetScreenStatus()) {
		Scene2D_Background->RenderUI();
		// Render the rear tile map
		RenderRearTileMap();
		// Render the required entities
		EntityManager::GetInstance()->RenderUI();
		// Render the tile map
		RenderTileMap();
		// Render the Enemy
		RenderEnemy();
		// Render the player
		RenderPlayer();

		Scene2D_corruption->RenderUI();
	}
	ui->Render();
}

void CScene2D::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}

void CScene2D::RenderTileMap()
{
	int m = 0;
	thePlayerInfo->UpdateMapFineOffset();
	int NumOfTilesMapHeight = m_cMap->getNumOfTiles_MapHeight();
	int NumOfTilesMapWidth = m_cMap->getNumOfTiles_MapWidth();
	for (int i = 0; i < NumOfTilesMapHeight; ++i)
	{
		for (int k = 0; k < NumOfTilesMapWidth + 1; ++k)
		{
			m = thePlayerInfo->GetTileOffset_x() + k;

			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if (m >= m_cMap->getNumOfTiles_MapWidth()
				|| m > thePlayerInfo->GetTileOffset_x() + m_cMap->getScreenWidth()/m_cMap->GetTileSize_Width() + 1)
				break;

			Vector3 tempPos = Vector3(static_cast<float>(k*m_cMap->GetTileSize_Width() + kiHalfTileWidth
									- thePlayerInfo->GetMapFineOffset_x()),
									static_cast<float>(224 - i * m_cMap->GetTileSize_Height() + kiHalfTileHeight));

			if (m_cMap->theScreenMap[i][m] != 0 && m_cMap->theScreenMap[i][m] < 100
				&& (m*m_cMap->GetTileSize_Width() + kiHalfTileWidth < Scene2D_corruption->GetPosition().x + Scene2D_corruption->GetScale().x / 2 - kiHalfTileWidth))
			{
				switch (rand() % 5)
				{
				case 0:
					Scene2D_Error2->SetPosition(tempPos);
					Scene2D_Error2->RenderUI();
					break;
				default:
					Scene2D_Error->SetPosition(tempPos);
					Scene2D_Error->RenderUI();
					break;
				}
			}
			else
			{
				switch (m_cMap->theScreenMap[i][m])
				{
				case 1:
					Scene2D_TileGround->SetPosition(tempPos);
					Scene2D_TileGround->RenderUI();
					break;
				case 3:
				{
					SpriteEntity* Door = nullptr;
					if (thePlayerInfo->checkPosition_Y == i && thePlayerInfo->checkPosition_X == m ||
						(thePlayerInfo->checkPosition_X + 1 < m_cMap->GetNumOfTiles_Width() && thePlayerInfo->checkPosition_Y == i && thePlayerInfo->checkPosition_X + 1 == m))
						Door = Scene2D_TileDoor2;
					else
						Door = Scene2D_TileDoor;
					Door->SetPosition(tempPos);
					Door->RenderUI();
					break;
				}
				case 10:
					Scene2D_Goodies_TreasureChest->SetPosition(tempPos);
					Scene2D_Goodies_TreasureChest->RenderUI();
					break;
				case 20:
					Scene2D_SpikeL->SetPosition(tempPos);
					Scene2D_SpikeL->RenderUI();
					break;
				case 21:
					Scene2D_SpikeR->SetPosition(tempPos);
					Scene2D_SpikeR->RenderUI();
					break;
				case 22:
					Scene2D_SpikeU->SetPosition(tempPos);
					Scene2D_SpikeU->RenderUI();
					break;
				case 23:
					Scene2D_SpikeD->SetPosition(tempPos);
					Scene2D_SpikeD->RenderUI();
					break;
				case 30:
					Scene2D_LevelUp->SetPosition(tempPos);
					Scene2D_LevelUp->RenderUI();
					break;
				default:
					break;
				}
			}
		}
	}
}

// Render the rear tile map
void CScene2D::RenderRearTileMap(void)
{
	int m = 0;
	thePlayerInfo->UpdateRearMap();
	int NumOfTiles_Height = m_cRearMap->GetNumOfTiles_Height();
	int NumOfTiles_Width = m_cRearMap->GetNumOfTiles_Width();
	for (int i = 0; i < NumOfTiles_Height; ++i)
	{
		for (int k = 0; k < NumOfTiles_Width + 1; ++k)
		{
			m = thePlayerInfo->GetRearTileOffset_x() + k;

			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if (m >= m_cRearMap->getNumOfTiles_MapWidth()
				|| m > thePlayerInfo->GetRearTileOffset_x() + m_cRearMap->getScreenWidth() / m_cRearMap->GetTileSize_Width() + 1)
				break;

			Vector3 tempPosition = Vector3(static_cast<float>(k*m_cRearMap->GetTileSize_Width() + kiHalfTileWidth
				- thePlayerInfo->GetRearMapFineOffset_x()),
				static_cast<float>(224 - i * m_cRearMap->GetTileSize_Height() + kiHalfTileHeight));

			switch (m_cRearMap->theScreenMap[i][m])
			{
			case 1:
				Scene2D_TilePillar_1->SetPosition(tempPosition);
				Scene2D_TilePillar_1->RenderUI();
				break;
			case 2:
				Scene2D_TilePillar_2->SetPosition(tempPosition);
				Scene2D_TilePillar_2->RenderUI();
				break;
			case 3:
				Scene2D_TilePillar_3->SetPosition(tempPosition);
				Scene2D_TilePillar_3->RenderUI();
				break;
			default:
				break;
			}
		}
	}
}

void CScene2D::RenderPlayer()
{
	//	Scene2D_Hero->SetPosition(thePlayerInfo->GetPos());
	//	Scene2D_Hero->RenderUI();

	// Display the player
	int playerFrameState = thePlayerInfo->GetFrameState();
	int playerMapOffsetX = thePlayerInfo->GetMapOffset_x();
	Scene2D_Hero_Animated[playerFrameState]->SetPosition(Vector3(thePlayerInfo->GetPos().x - playerMapOffsetX, thePlayerInfo->GetPos().y));
	Scene2D_Hero_Animated[playerFrameState]->RenderUI();
	int slashFrameState = theSlashInfo->GetFrameState();
	if (slashFrameState != Slash::S_TOTAL)
	{
		Scene2D_Slash_Animated[slashFrameState]->SetPosition(Vector3(theSlashInfo->GetPos().x - playerMapOffsetX, theSlashInfo->GetPos().y));
		Scene2D_Slash_Animated[slashFrameState]->RenderUI();
	}
	int buttonFrameState = theButtonInfo->GetFrameState();
	if (buttonFrameState != E_Button::B_TOTAL)
	{
		Scene2D_E[buttonFrameState]->SetPosition(Vector3(theButtonInfo->GetPos().x - playerMapOffsetX, theButtonInfo->GetPos().y));
		Scene2D_E[buttonFrameState]->RenderUI();
	}
}

// Render the enemy
void CScene2D::RenderEnemy(void)
{
	// Render the enemies
	for (size_t i = 0; i < theEnemy.size(); ++i)
	{
		int theEnemy_x = theEnemy[i]->GetPos_x() - thePlayerInfo->mapOffset_x;
		int theEnemy_y = theEnemy[i]->GetPos_y();
		int enemyFrameState = theEnemy[i]->GetFrameState();

		if (((theEnemy_x >= 0) && (theEnemy_x < m_cMap->GetNumOfTiles_Width()*m_cMap->GetTileSize_Width())) &&
			((theEnemy_y >= 0) && (theEnemy_y < m_cMap->GetNumOfTiles_Height()*m_cMap->GetTileSize_Height())) &&
			enemyFrameState != CEnemy::C_TOTAL)
		{
			Scene2D_Enemy[enemyFrameState]->SetPosition(Vector3(static_cast<float>(theEnemy_x), static_cast<float>(theEnemy_y), 0));
			Scene2D_Enemy[enemyFrameState]->RenderUI();
			if (theEnemy[i]->GetHp() < theEnemy[i]->GetMaxHp())
			{
				float temporaryFloat = static_cast<float>(theEnemy[i]->GetHp()) / static_cast<float>(theEnemy[i]->GetMaxHp()) * m_cMap->GetTileSize_Width();
				Scene2D_EnemyHpBar->SetPosition(Vector3(static_cast<float>(theEnemy_x - temporaryFloat / 2), static_cast<float>(theEnemy_y + m_cMap->GetTileSize_Height() / 2), 0));
				Scene2D_EnemyHpBar->SetScale(Vector3(temporaryFloat, static_cast<float>(m_cMap->GetTileSize_Height() / 4), 0.f));
				Scene2D_EnemyHpBar->RenderUI();
			}
		}
	}
	for (size_t i = 0; i < theAxeEnemy.size(); ++i)
	{

		int theEnemy_x = theAxeEnemy[i]->GetPos_x() - thePlayerInfo->mapOffset_x;
		int theEnemy_y = theAxeEnemy[i]->GetPos_y();
		int enemyFrameState = theAxeEnemy[i]->GetFrameState();

		if (((theEnemy_x >= 0) && (theEnemy_x < m_cMap->GetNumOfTiles_Width()*m_cMap->GetTileSize_Width())) &&
			((theEnemy_y >= 0) && (theEnemy_y < m_cMap->GetNumOfTiles_Height()*m_cMap->GetTileSize_Height())) &&
			enemyFrameState != CAxeEnemy::A_TOTAL)
		{
			Scene2D_AxeEnemy[enemyFrameState]->SetPosition(Vector3(static_cast<float>(theEnemy_x), static_cast<float>(theEnemy_y), 0));
			Scene2D_AxeEnemy[enemyFrameState]->RenderUI();
			if (theAxeEnemy[i]->GetHp() < theAxeEnemy[i]->GetMaxHp())
			{
				float temporaryFloat = static_cast<float>(theAxeEnemy[i]->GetHp()) / static_cast<float>(theAxeEnemy[i]->GetMaxHp()) * m_cMap->GetTileSize_Width();
				Scene2D_EnemyHpBar->SetPosition(Vector3(static_cast<float>(theEnemy_x - temporaryFloat / 2), static_cast<float>(theEnemy_y + m_cMap->GetTileSize_Height() / 2), 0));
				Scene2D_EnemyHpBar->SetScale(Vector3(temporaryFloat, static_cast<float>(m_cMap->GetTileSize_Height() / 4), 0.f));
				Scene2D_EnemyHpBar->RenderUI();
			}
		}

	}
}

// Create the Goodies
void CScene2D::CreateGoodies(void)
{
	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			if (m_cMap->theScreenMap[i][k] == 10)
			{
				CGoodiesFactory::GetInstance()->Create(TREASURECHEST, k, i);
			}
		}
	}
}

void CScene2D::LoadMeshes(void)
{
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

	// Load background image
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_BKGROUND")->textureID = LoadTGA("Image//Scene2D_Background.tga");

	// Tiles
	MeshBuilder::GetInstance()->GenerateQuad("Tile_1111", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Tile_1111")->textureID = LoadTGA("Image//Sprites//Tile_1111.tga");
	{
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Anvil", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Anvil")->textureID = LoadTGA("Image//Sprites//Tile_Anvil.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_BG", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_BG")->textureID = LoadTGA("Image//Sprites//Tile_BG.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_BG_Safe", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_BG_Safe")->textureID = LoadTGA("Image//Sprites//Tile_BG_Safe.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Crate", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Crate")->textureID = LoadTGA("Image//Sprites//Tile_Crate.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Door_Close", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Door_Close")->textureID = LoadTGA("Image//Sprites//Tile_Door_Close.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Door_Open", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Door_Open")->textureID = LoadTGA("Image//Sprites//Tile_Door_Open.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_LevelUp", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_LevelUp")->textureID = LoadTGA("Image//Sprites//Tile_LevelUp.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Poison_Back", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Poison_Back")->textureID = LoadTGA("Image//Sprites//Tile_Poison_Back.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Poison_Front", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Poison_Front")->textureID = LoadTGA("Image//Sprites//Tile_Poison_Front.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Shield", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Shield")->textureID = LoadTGA("Image//Sprites//Tile_Shield.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Spike_Down", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Spike_Down")->textureID = LoadTGA("Image//Sprites//Tile_Spike_Down.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Spike_Left", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Spike_Left")->textureID = LoadTGA("Image//Sprites//Tile_Spike_Left.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Spike_Right", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Spike_Right")->textureID = LoadTGA("Image//Sprites//Tile_Spike_Right.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Spike_Up", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Spike_Up")->textureID = LoadTGA("Image//Sprites//Tile_Spike_Up.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Null_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Null_1")->textureID = LoadTGA("Image//Sprites//Tile_Null_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Null_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Null_2")->textureID = LoadTGA("Image//Sprites//Tile_Null_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Pillar_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Pillar_1")->textureID = LoadTGA("Image//Sprites//Tile_Pillar_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Pillar_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Pillar_2")->textureID = LoadTGA("Image//Sprites//Tile_Pillar_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Tile_Pillar_3", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Tile_Pillar_3")->textureID = LoadTGA("Image//Sprites//Tile_Pillar_3.tga");
	}
	// Lonin
	{
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Attack_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Attack_1")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Attack_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Attack_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Attack_2")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Attack_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Charged_Attack_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Charged_Attack_1")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Charged_Attack_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Charged_Attack_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Charged_Attack_2")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Charged_Attack_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Fall_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Fall_1")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Fall_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Fall_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Fall_2")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Fall_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Idle_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Idle_1")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Idle_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Idle_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Idle_2")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Idle_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Charge", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Charge")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Charge.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Jump_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Jump_1")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Jump_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Jump_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Jump_2")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Jump_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Pogo_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Pogo_1")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Pogo_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Pogo_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Pogo_2")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Pogo_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Roll_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Roll_1")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Roll_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Roll_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Roll_2")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Roll_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Roll_3", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Roll_3")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Roll_3.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Roll_4", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Roll_4")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Roll_4.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Run_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Run_1")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Run_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Left_Run_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Left_Run_2")->textureID = LoadTGA("Image//Sprites//Lonin_Left_Run_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Attack_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Attack_1")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Attack_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Attack_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Attack_2")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Attack_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Charged_Attack_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Charged_Attack_1")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Charged_Attack_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Charged_Attack_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Charged_Attack_2")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Charged_Attack_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Fall_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Fall_1")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Fall_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Fall_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Fall_2")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Fall_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Idle_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Idle_1")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Idle_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Idle_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Idle_2")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Idle_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Charge", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Charge")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Charge.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Jump_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Jump_1")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Jump_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Jump_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Jump_2")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Jump_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Pogo_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Pogo_1")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Pogo_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Pogo_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Pogo_2")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Pogo_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Roll_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Roll_1")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Roll_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Roll_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Roll_2")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Roll_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Roll_3", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Roll_3")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Roll_3.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Roll_4", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Roll_4")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Roll_4.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Run_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Run_1")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Run_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Lonin_Right_Run_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Lonin_Right_Run_2")->textureID = LoadTGA("Image//Sprites//Lonin_Right_Run_2.tga");
	}
	// Slashes
	{
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Down_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Down_1")->textureID = LoadTGA("Image//Sprites//Slash_Down_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Down_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Down_2")->textureID = LoadTGA("Image//Sprites//Slash_Down_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Left_1_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Left_1_1")->textureID = LoadTGA("Image//Sprites//Slash_Left_1_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Left_1_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Left_1_2")->textureID = LoadTGA("Image//Sprites//Slash_Left_1_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Left_2_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Left_2_1")->textureID = LoadTGA("Image//Sprites//Slash_Left_2_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Left_2_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Left_2_2")->textureID = LoadTGA("Image//Sprites//Slash_Left_2_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Left_3_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Left_3_1")->textureID = LoadTGA("Image//Sprites//Charged_Left_Slash_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Left_3_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Left_3_2")->textureID = LoadTGA("Image//Sprites//Charged_Left_Slash_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Left_3_3", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Left_3_3")->textureID = LoadTGA("Image//Sprites//Charged_Left_Slash_3.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Right_1_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Right_1_1")->textureID = LoadTGA("Image//Sprites//Slash_Right_1_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Right_1_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Right_1_2")->textureID = LoadTGA("Image//Sprites//Slash_Right_1_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Right_2_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Right_2_1")->textureID = LoadTGA("Image//Sprites//Slash_Right_2_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Right_2_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Right_2_2")->textureID = LoadTGA("Image//Sprites//Slash_Right_2_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Right_3_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Right_3_1")->textureID = LoadTGA("Image//Sprites//Charged_Right_Slash_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Right_3_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Right_3_2")->textureID = LoadTGA("Image//Sprites//Charged_Right_Slash_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Right_3_3", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Right_3_3")->textureID = LoadTGA("Image//Sprites//Charged_Right_Slash_3.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Up_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Up_1")->textureID = LoadTGA("Image//Sprites//Slash_Up_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Slash_Up_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Slash_Up_2")->textureID = LoadTGA("Image//Sprites//Slash_Up_2.tga");
	}
	// Enemies
	{
		MeshBuilder::GetInstance()->GenerateQuad("Axe_Attack_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Axe_Attack_1")->textureID = LoadTGA("Image//Sprites//Axe_Attack_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Axe_Attack_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Axe_Attack_2")->textureID = LoadTGA("Image//Sprites//Axe_Attack_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Axe_Attack_3", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Axe_Attack_3")->textureID = LoadTGA("Image//Sprites//Axe_Attack_3.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Axe_Attack_4", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Axe_Attack_4")->textureID = LoadTGA("Image//Sprites//Axe_Attack_4.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Axe_Attack_5", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Axe_Attack_5")->textureID = LoadTGA("Image//Sprites//Axe_Attack_5.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Axe_Die_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Axe_Die_1")->textureID = LoadTGA("Image//Sprites//Axe_Die_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Axe_Die_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Axe_Die_2")->textureID = LoadTGA("Image//Sprites//Axe_Die_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Axe_Idle_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Axe_Idle_1")->textureID = LoadTGA("Image//Sprites//Axe_Idle_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Axe_Idle_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Axe_Idle_2")->textureID = LoadTGA("Image//Sprites//Axe_Idle_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Axe_Run_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Axe_Run_1")->textureID = LoadTGA("Image//Sprites//Axe_Run_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Axe_Run_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Axe_Run_2")->textureID = LoadTGA("Image//Sprites//Axe_Run_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Crystal_Attack_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Crystal_Attack_1")->textureID = LoadTGA("Image//Sprites//Crystal_Attack_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Crystal_Attack_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Crystal_Attack_2")->textureID = LoadTGA("Image//Sprites//Crystal_Attack_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Crystal_Die_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Crystal_Die_1")->textureID = LoadTGA("Image//Sprites//Crystal_Die_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Crystal_Die_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Crystal_Die_2")->textureID = LoadTGA("Image//Sprites//Crystal_Die_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Crystal_Idle_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Crystal_Idle_1")->textureID = LoadTGA("Image//Sprites//Crystal_Idle_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Crystal_Idle_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Crystal_Idle_2")->textureID = LoadTGA("Image//Sprites//Crystal_Idle_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Crystal_Projectile_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Crystal_Projectile_1")->textureID = LoadTGA("Image//Sprites//Crystal_Projectile_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Crystal_Projectile_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Crystal_Projectile_2")->textureID = LoadTGA("Image//Sprites//Crystal_Projectile_2.tga");
	}
	// UI
	{
		MeshBuilder::GetInstance()->GenerateQuad("UI_BOX", Color(0.2f, 0.2f, 0.6f), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("Corrupt_temp", Color(1, 0, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Corrupt_temp")->textureID = LoadTGA("Image//corruptionAlpha.tga");
		MeshBuilder::GetInstance()->GenerateQuad("UI_BOX2", Color(0, 0, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("UI_Bar", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("UI_Bar")->textureID = LoadTGA("Image//Sprites//UI_Bar.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Stamina_Green", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Stamina_Green")->textureID = LoadTGA("Image//Sprites//Stamina_Green.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Stamina_Amber", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Stamina_Amber")->textureID = LoadTGA("Image//Sprites//Stamina_Amber.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Stamina_Red", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Stamina_Red")->textureID = LoadTGA("Image//Sprites//Stamina_Red.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Stamina_Bar", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Stamina_Bar")->textureID = LoadTGA("Image//Sprites//Stamina_Bar.tga");
		MeshBuilder::GetInstance()->GenerateQuad("XP_Bar", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("XP_Bar")->textureID = LoadTGA("Image//Sprites//XP_Bar.tga");
		MeshBuilder::GetInstance()->GenerateQuad("XP_Block", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("XP_Block")->textureID = LoadTGA("Image//Sprites//XP_Block.tga");
		MeshBuilder::GetInstance()->GenerateQuad("XP_Block2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("XP_Block2")->textureID = LoadTGA("Image//Sprites//XP_Block2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Level_Up_Screen", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Level_Up_Screen")->textureID = LoadTGA("Image//Level_Up_Screen.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Skill_Selected_Frame", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Skill_Selected_Frame")->textureID = LoadTGA("Image//Sprites//Skill_Selected_Frame.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Skill_Unlocked_Frame", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Skill_Unlocked_Frame")->textureID = LoadTGA("Image//Sprites//Skill_Unlocked_Frame.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Life_Block", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Life_Block")->textureID = LoadTGA("Image//Sprites//Life_Block.tga");

		MeshBuilder::GetInstance()->GenerateQuad("Heart_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Heart_1")->textureID = LoadTGA("Image//Sprites//Heart_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Heart_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Heart_2")->textureID = LoadTGA("Image//Sprites//Heart_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Heart_3", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Heart_3")->textureID = LoadTGA("Image//Sprites//Heart_3.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Heart_4", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Heart_4")->textureID = LoadTGA("Image//Sprites//Heart_4.tga");

		MeshBuilder::GetInstance()->GenerateQuad("E_Button_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("E_Button_1")->textureID = LoadTGA("Image//Sprites//E_Button_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("E_Button_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("E_Button_2")->textureID = LoadTGA("Image//Sprites//E_Button_2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("X_Button_1", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("X_Button_1")->textureID = LoadTGA("Image//Sprites//X_Button_1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("X_Button_2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("X_Button_2")->textureID = LoadTGA("Image//Sprites//X_Button_2.tga");
	}
	// Title Screen
	{
		MeshBuilder::GetInstance()->GenerateQuad("Title_Screen", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Title_Screen")->textureID = LoadTGA("Image//Title_Screen.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Title_Screen2", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GetMesh("Title_Screen2")->textureID = LoadTGA("Image//Title_Screen2.tga");
	}
}
