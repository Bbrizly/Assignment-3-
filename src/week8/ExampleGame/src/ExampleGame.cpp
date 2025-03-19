//------------------------------------------------------------------------
// ExampleGame
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// Main game class. Derives from Common::Game.
//------------------------------------------------------------------------

#include "ComponentAIController.h"
#include "ComponentAnimController.h"
#include "ComponentCharacterController.h"
#include "ComponentRenderableMesh.h"
#include "ExampleGame.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "W_Model.h"
#include <time.h>

using namespace week8;

//------------------------------------------------------------------------------
// Method:    ExampleGame
// Parameter: void
// Returns:   
// 
// Constructor
//------------------------------------------------------------------------------
ExampleGame::ExampleGame()
	: 
	m_pSceneCamera(NULL),
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
	assert(!m_pSceneCamera);
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
	m_pSceneCamera = new Common::SceneCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 25.0f, 55.0f), glm::vec3(0.0f,5.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	Common::SceneManager::Instance()->AttachCamera(m_pSceneCamera);

	// Initialize our GameObjectManager
	m_pGameObjectManager = new Common::GameObjectManager();

	// Register factory methods
	m_pGameObjectManager->RegisterComponentFactory("GOC_AnimController", ComponentAnimController::CreateComponent);

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
	pAIControllerComponent->Init("week8/ExampleGame/data/AIDecisionTree.xml");
	pZombie->AddComponent(pAIControllerComponent);

	// - - - - - - - - - - -
	// Ground Game Object

	Common::GameObject* pGround = m_pGameObjectManager->CreateGameObject();
	m_pGameObjectManager->SetGameObjectGUID(pGround, "Ground");
	pRenderableComponent = new ComponentRenderableMesh();
	pRenderableComponent->Init("week8/ExampleGame/data/ground.pod", "week8/ExampleGame/data/", "week8/ExampleGame/data/textured.vsh", "week8/ExampleGame/data/textured.fsh");
	pGround->AddComponent(pRenderableComponent);

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
	// Clear our Game Objects
	m_pGameObjectManager->DestroyAllGameObjects();
	delete m_pGameObjectManager;
	m_pGameObjectManager = NULL;

	// Delete our camera
	if (m_pSceneCamera)
	{
		delete m_pSceneCamera;
		m_pSceneCamera = NULL;
	}

	// Destroy the Scene Manager
	Common::SceneManager::DestroyInstance();
}