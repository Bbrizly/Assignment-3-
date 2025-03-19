//------------------------------------------------------------------------
// ExampleGame
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// Main game class. Derives from Common::Game.
//------------------------------------------------------------------------

#include "AIPathfinder.h"
#include "ComponentAIController.h"
#include "ComponentAnimController.h"
#include "ComponentCharacterController.h"
#include "ComponentRenderableMesh.h"
#include "ExampleGame.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "W_Model.h"
#include <time.h>

using namespace week9;

//------------------------------------------------------------------------------
// Method:    ExampleGame
// Parameter: void
// Returns:   
// 
// Constructor
//------------------------------------------------------------------------------
ExampleGame::ExampleGame()
	: 
	m_pFrontCamera(NULL),
	m_pTopDownCamera(NULL),
	m_pGameObjectManager(NULL)
{
}

//------------------------------------------------------------------------------
// Method:    ~ExampleGame
// Parameter: void
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ExampleGame::~ExampleGame()
{
	assert(!m_pGameObjectManager);
	assert(!m_pFrontCamera);
	assert(!m_pTopDownCamera);
}

//------------------------------------------------------------------------------
// Method:    Init
// Returns:   bool
// 
// Overridden from Common::Game::Init(). Called when it's time to initialize our
// game. Returns true on successful initialization, false otherwise. If false is
// returned, the game will shut down.
//------------------------------------------------------------------------------
bool ExampleGame::Init()
{
	// Seed rand for any AI behaviour
	srand(time(NULL));

	// Initialize our Scene Manager
	Common::SceneManager::CreateInstance();

	// Initialize our Scene Camera and attach it to the Scene Manager
	m_pTopDownCamera = new Common::SceneCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 65.0f, 0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f));
	m_pFrontCamera = new Common::SceneCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 25.0f, 55.0f), glm::vec3(0.0f,5.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	Common::SceneManager::Instance()->AttachCamera(m_pFrontCamera);

	// Initialize our GameObjectManager
	m_pGameObjectManager = new Common::GameObjectManager();

	// Register factory methods
	m_pGameObjectManager->RegisterComponentFactory("GOC_AnimController", ComponentAnimController::CreateComponent);

	// Initialize our AIPathfindingManager
	AIPathfinder::CreateInstance();

	// - - - - - - - - - - - 
	// Player Game Object

	// Create the player GameObject
	Common::GameObject* pCharacter = m_pGameObjectManager->CreateGameObject();
	pCharacter->GetTransform().Scale(glm::vec3(0.05f, 0.05, 0.05f));
	pCharacter->GetTransform().SetTranslation(glm::vec3(-15.0f, 0.0f, 0.0f));
	m_pGameObjectManager->SetGameObjectGUID(pCharacter, "PlayerCharacter");

	// Create a renderable component for it
	ComponentRenderableMesh* pRenderableComponent = new ComponentRenderableMesh();
	pRenderableComponent->Init("data/characters/swat/Swat.pod", "data/characters/swat/", "data/shaders/skinned.vsh", "data/shaders/skinned.fsh");
	pCharacter->AddComponent(pRenderableComponent);

	// Create an animation controller component for it
	ComponentAnimController* pAnimControllerComponent = new ComponentAnimController();
	pAnimControllerComponent->AddAnim("idle", "data/characters/swat/anim/idle.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("walking", "data/characters/swat/anim/walking.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("right_strafe", "data/characters/swat/anim/right_strafe.pod", 0, 20, true);
	pAnimControllerComponent->AddAnim("right_strafe_walking", "data/characters/swat/anim/right_strafe_walking.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("jump", "data/characters/swat/anim/jump.pod", 0, 31, false);
	pAnimControllerComponent->AddAnim("left_strafe", "data/characters/swat/anim/left_strafe.pod", 0, 20, true);
	pAnimControllerComponent->AddAnim("left_strafe_walking", "data/characters/swat/anim/left_strafe_walking.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("left_turn_90", "data/characters/swat/anim/left_turn_90.pod", 0, 31, false);
	pAnimControllerComponent->AddAnim("right_turn_90", "data/characters/swat/anim/right_turn_90.pod", 0, 31, false);
	pAnimControllerComponent->AddAnim("standard_run", "data/characters/swat/anim/standard_run.pod", 0, 22, true);
	pAnimControllerComponent->SetAnim("idle");
	pCharacter->AddComponent(pAnimControllerComponent); 

	// Create a controller component for it
	ComponentCharacterController* pCharacterControllerComponent = new ComponentCharacterController();
	pCharacter->AddComponent(pCharacterControllerComponent);

	// - - - - - - - - - - - 
	// AI Game Object

	Common::GameObject* pZombie = m_pGameObjectManager->CreateGameObject();
	pZombie->GetTransform().Scale(glm::vec3(0.05f, 0.05, 0.05f));
	pZombie->GetTransform().SetTranslation(glm::vec3(15.0f, 0.0f, 0.0f));
	m_pGameObjectManager->SetGameObjectGUID(pZombie, "AICharacter");

	// Create a renderable component for it
	pRenderableComponent = new ComponentRenderableMesh();
	pRenderableComponent->Init("data/characters/zombie/Zombie.pod", "data/characters/zombie/", "data/shaders/skinned.vsh", "data/shaders/skinned.fsh");
	pZombie->AddComponent(pRenderableComponent);

	// Create an animation controller component for it
	pAnimControllerComponent = new ComponentAnimController();

	// Animation controller
	pAnimControllerComponent->AddAnim("run", "data/characters/zombie/anim/zombie_anims.pod", 12, 36, true);
	pAnimControllerComponent->AddAnim("walk", "data/characters/zombie/anim/zombie_anims.pod", 108, 229, true);
	pAnimControllerComponent->AddAnim("idle", "data/characters/zombie/anim/zombie_anims.pod", 350, 470, true);
	pAnimControllerComponent->AddAnim("walk_injured", "data/characters/zombie/anim/zombie_anims.pod", 547, 582, true);
	pZombie->AddComponent(pAnimControllerComponent); 

	// Create a controller component for it
	ComponentAIController* pAIControllerComponent = new ComponentAIController();
	pAIControllerComponent->Init("week9/ExampleGame/data/AIDecisionTree.xml");
	pZombie->AddComponent(pAIControllerComponent);

	// - - - - - - - - - - -
	// Ground Game Object

	Common::GameObject* pGround = m_pGameObjectManager->CreateGameObject();
	m_pGameObjectManager->SetGameObjectGUID(pGround, "Ground");
	pRenderableComponent = new ComponentRenderableMesh();
	pRenderableComponent->Init("week9/ExampleGame/data/ground.pod", "week9/ExampleGame/data/", "week9/ExampleGame/data/textured.vsh", "week9/ExampleGame/data/textured.fsh");
	pGround->AddComponent(pRenderableComponent);

	// Different data for different path finding data
	static int iSample = 1;
	switch(iSample)
	{
		case 1:
		{
			InitSample1Level();
			break;
		}
		case 2:
		{
			InitSample2Level();
			break;
		}
		case 3:
		{
			InitSample3Level();
			break;
		}
	}

	// Everything initialized OK.
	return true;
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   bool
// 
// Overridden from Common::Game::Update(). Called once per frame during the main
// loop of the game. The game state is updated here. Returns true on success, 
// false otherwise. If false is returned, the game will shutdown.
//------------------------------------------------------------------------------
bool ExampleGame::Update(float p_fDelta)
{
	m_pGameObjectManager->Update(p_fDelta);

	// Toggle pathfinding debug rendering
	static bool bLastKeyDown = false;
	bool bCurrentKeyDown = glfwGetKey('Z');
	if (bCurrentKeyDown && !bLastKeyDown)
	{
		AIPathfinder::Instance()->ToggleDebugRendering(m_pGameObjectManager);
	}
	bLastKeyDown = bCurrentKeyDown;

	// Toggle cameras
	static bool bIsFrontView = true;
	static bool bLastCamKeyDown = false;
	bool bCamKeyDown = glfwGetKey('C');
	if (bCamKeyDown && !bLastCamKeyDown)
	{
		bIsFrontView = !bIsFrontView;
		Common::SceneManager::Instance()->AttachCamera(bIsFrontView ? m_pFrontCamera : m_pTopDownCamera);
	}
	bLastCamKeyDown = bCamKeyDown;

	return true;
}

//------------------------------------------------------------------------------
// Method:    Render
// Returns:   void
// 
// Overridden from Common::Game::Render(). Called once per frame during the main
// loop of the game. Any objects in the game that need to be drawn are rendered
// here.
//------------------------------------------------------------------------------
void ExampleGame::Render()
{
	// Sync transforms to render components
	m_pGameObjectManager->SyncTransforms();

	// Render the scene
	Common::SceneManager::Instance()->Render();

	// Debug pathfinding render
	AIPathfinder::Instance()->Render(Common::SceneManager::Instance()->GetCamera()->GetProjectionMatrix(),
									 Common::SceneManager::Instance()->GetCamera()->GetViewMatrix());
}

//------------------------------------------------------------------------------
// Method:    Shutdown
// Returns:   void
// 
// Called when the game is shutting down. All dynamic memory needs to be cleaned
// up.
//------------------------------------------------------------------------------
void ExampleGame::Shutdown()
{
	// Destroy our AIPathfindingManager
	AIPathfinder::DestroyInstance();

	// Clear our Game Objects
	m_pGameObjectManager->DestroyAllGameObjects();
	delete m_pGameObjectManager;
	m_pGameObjectManager = NULL;

	// Delete our camera
	if (m_pFrontCamera)
	{
		delete m_pFrontCamera;
		m_pFrontCamera = NULL;
	}

	// Delete our top down camera
	if (m_pTopDownCamera)
	{
		delete m_pTopDownCamera;
		m_pTopDownCamera = NULL;
	}

	// Destroy the Scene Manager
	Common::SceneManager::DestroyInstance();
}

//------------------------------------------------------------------------------
// Method:    InitSample1Level
// Returns:   void
//
// Initialize sample 1 level.
//------------------------------------------------------------------------------
void ExampleGame::InitSample1Level()
{
	AIPathfinder::Instance()->Load("week9/ExampleGame/data/AIPathfindingData.xml");
}

//------------------------------------------------------------------------------
// Method:    InitSample2Level
// Returns:   void
//
// Initialize sample 2 level.
//------------------------------------------------------------------------------
void ExampleGame::InitSample2Level()
{
	AIPathfinder::Instance()->Load("week9/ExampleGame/data/AIPathfindingData2.xml");

	for (int i = 0; i < 9; ++i)
	{
		Common::GameObject* pCrate = m_pGameObjectManager->CreateGameObject();
		ComponentRenderableMesh* pRenderableComponent = new ComponentRenderableMesh();
		pRenderableComponent->Init("week9/ExampleGame/data/crate.pod", "week9/ExampleGame/data/", "week9/ExampleGame/data/textured.vsh", "week9/ExampleGame/data/textured.fsh");
		pCrate->AddComponent(pRenderableComponent);
		pCrate->GetTransform().SetTranslation(glm::vec3(0.0f, 0.0f, ((float)i - 4.0f) * 3.0f));
	}
}

//------------------------------------------------------------------------------
// Method:    InitSample3Level
// Returns:   void
//
// Initialize sample 3 level.
//------------------------------------------------------------------------------
void ExampleGame::InitSample3Level()
{
	AIPathfinder::Instance()->Load("week9/ExampleGame/data/AIPathfindingData3.xml");

	TiXmlDocument doc("week9/ExampleGame/data/crate_positions.xml");
	if (doc.LoadFile() == true)
	{
		TiXmlNode* pRoot = doc.FirstChild("crate_positions");
		if (pRoot)
		{
			TiXmlNode* pPosition = pRoot->FirstChild("position");
			while(pPosition)
			{
				float x, y, z;
				TiXmlElement* pPositionElement = pPosition->ToElement();
				pPositionElement->QueryFloatAttribute("x", &x);
				pPositionElement->QueryFloatAttribute("y", &y);
				pPositionElement->QueryFloatAttribute("z", &z);

				Common::GameObject* pCrate = m_pGameObjectManager->CreateGameObject();
				ComponentRenderableMesh* pRenderableComponent = new ComponentRenderableMesh();
				pRenderableComponent->Init("week9/ExampleGame/data/crate.pod", "week9/ExampleGame/data/", "week9/ExampleGame/data/textured.vsh", "week9/ExampleGame/data/textured.fsh");
				pCrate->AddComponent(pRenderableComponent);
				pCrate->GetTransform().SetTranslation(glm::vec3(x, y, z));

				pPosition = pPosition->NextSibling();
			}
		}
	}
}