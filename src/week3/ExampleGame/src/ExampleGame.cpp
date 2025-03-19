//------------------------------------------------------------------------
// ExampleGame
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// Main game class. Derives from Common::Game.
//------------------------------------------------------------------------

#include "ComponentAnimController.h"
#include "ComponentCharacterController.h"
#include "ComponentRenderableMesh.h"
#include "ExampleGame.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "W_Model.h"

using namespace week2;

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
	// Initialize our Scene Manager
	Common::SceneManager::CreateInstance();

	// Initialize our Scene Camera and attach it to the Scene Manager
	m_pSceneCamera = new Common::SceneCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3(0.0f,5.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	Common::SceneManager::Instance()->AttachCamera(m_pSceneCamera);

	// Initialize our GameObjectManager
	m_pGameObjectManager = new Common::GameObjectManager();

	// Create an empty GameObject
	Common::GameObject* character = m_pGameObjectManager->CreateGameObject();
	character->GetTransform().Scale(glm::vec3(0.05f, 0.05, 0.05f));

	// Create a renderable component for it
	ComponentRenderableMesh* pRenderableComponent = new ComponentRenderableMesh();
	pRenderableComponent->Init("data/characters/swat/Swat.pod", "data/characters/swat/", "data/shaders/skinned.vsh", "data/shaders/skinned.fsh"); 
	character->AddComponent(pRenderableComponent);

	// Create a controller component for it
	ComponentCharacterController* pCharacterControllerComponent = new ComponentCharacterController();
	character->AddComponent(pCharacterControllerComponent);

	// Create an animation controller component for it
	ComponentAnimController* pAnimControllerComponent = new ComponentAnimController();
	pAnimControllerComponent->AddAnim("idle", "data/characters/swat/anim/idle.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("walking", "data/characters/swat/anim/walking.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("right_strafe", "data/characters/swat/anim/right_strafe.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("right_strafe_walking", "data/characters/swat/anim/right_strafe_walking.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("jump", "data/characters/swat/anim/jump.pod", 0, 31, false);
	pAnimControllerComponent->AddAnim("left_strafe", "data/characters/swat/anim/left_strafe.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("left_strafe_walking", "data/characters/swat/anim/left_strafe_walking.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("left_turn_90", "data/characters/swat/anim/left_turn_90.pod", 0, 31, false);
	pAnimControllerComponent->AddAnim("right_turn_90", "data/characters/swat/anim/right_turn_90.pod", 0, 31, false);
	pAnimControllerComponent->AddAnim("standard_run", "data/characters/swat/anim/standard_run.pod", 0, 22, true);
	pAnimControllerComponent->SetAnim("idle");
	character->AddComponent(pAnimControllerComponent); 
	pAnimControllerComponent->InitDelegates();

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