//------------------------------------------------------------------------
// ExampleGame2
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// Main game class. Derives from week1::Game.
//------------------------------------------------------------------------

#include "ExampleGame2.h"
#include "W_Model.h"

using namespace week1;

//------------------------------------------------------------------------------
// Method:    ExampleGame2
// Parameter: void
// Returns:   
// 
// Constructor
//------------------------------------------------------------------------------
ExampleGame2::ExampleGame2()
	:
	m_pSceneCamera(NULL),
	m_pModel(NULL)
{
}

//------------------------------------------------------------------------------
// Method:    ~ExampleGame2
// Parameter: void
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ExampleGame2::~ExampleGame2()
{
	assert(!m_pModel);
	assert(!m_pSceneCamera);
}

//------------------------------------------------------------------------------
// Method:    Init
// Returns:   bool
// 
// Overridden from week1::Game::Init(). Called when it's time to initialize our
// game. Returns true on successful initialization, false otherwise. If false is
// returned, the game will shut down.
//------------------------------------------------------------------------------
bool ExampleGame2::Init()
{ 
	// Initialize our Scene Manager
	SceneManager::CreateInstance();

	// Initialize our Scene Camera and attach it to the Scene Manager
	m_pSceneCamera = new SceneCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 90.0f, 300.0f), glm::vec3(0.0f,90.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	SceneManager::Instance()->AttachCamera(m_pSceneCamera);

	// Initialize the model and add it the Scene Manager
	// Initialize the model	
	m_pModel = new wolf::Model("data/characters/zombie/Zombie.pod", "data/characters/zombie/", "data/shaders/skinned.vsh", "data/shaders/skinned.fsh");
	m_pModel->AddAnimation("anim", "data/characters/zombie/anim/zombie_anims.pod");
	m_pModel->SetAnim("anim");
	/*
	m_pModel = new wolf::Model("data/characters/swat/Swat.pod", "data/characters/swat/", "data/shaders/skinned.vsh", "data/shaders/skinned.fsh");
	m_pModel->AddAnimation("idle", "data/characters/swat/anim/idle.pod");
	m_pModel->AddAnimation("jump", "data/characters/swat/anim/jump.pod");
	m_pModel->AddAnimation("left_strafe", "data/characters/swat/anim/left_strafe.pod");
	m_pModel->AddAnimation("left_strafe_walking", "data/characters/swat/anim/left_strafe_walking.pod");
	m_pModel->AddAnimation("left_turn_90", "data/characters/swat/anim/left_turn_90.pod");
	m_pModel->AddAnimation("right_strafe", "data/characters/swat/anim/right_strafe.pod");
	m_pModel->AddAnimation("right_strafe_walking", "data/characters/swat/anim/right_strafe_walking.pod");
	m_pModel->AddAnimation("right_turn_90", "data/characters/swat/anim/right_turn_90.pod");
	m_pModel->AddAnimation("standard_run", "data/characters/swat/anim/standard_run.pod");
	m_pModel->AddAnimation("walking", "data/characters/swat/anim/walking.pod");
	m_pModel->SetAnim("walking");
	*/
	SceneManager::Instance()->AddModel(m_pModel);

	return true;
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   bool
// 
// Overridden from week1::Game::Update(). Called once per frame during the main
// loop of the game. The game state is updated here. Returns true on success, 
// false otherwise. If false is returned, the game will shutdown.
//------------------------------------------------------------------------------
bool ExampleGame2::Update()
{
	m_pModel->Update(0.016 /* Week 1 Exercise: Make this a delta time */);
	return true;
}

//------------------------------------------------------------------------------
// Method:    Render
// Returns:   void
// 
// Overridden from week1::Game::Render(). Called once per frame during the main
// loop of the game. Any objects in the game that need to be drawn are rendered
// here.
//------------------------------------------------------------------------------
void ExampleGame2::Render()
{
	SceneManager::Instance()->Render();
}

//------------------------------------------------------------------------------
// Method:    Shutdown
// Returns:   void
// 
// Called when the game is shutting down. All dynamic memory needs to be cleaned
// up.
//------------------------------------------------------------------------------
void ExampleGame2::Shutdown()
{
	if (m_pModel)
	{
		SceneManager::Instance()->RemoveModel(m_pModel);
		delete m_pModel;
		m_pModel = NULL;
	}

	if (m_pSceneCamera)
	{
		delete m_pSceneCamera;
		m_pSceneCamera = NULL;
	}

	SceneManager::DestroyInstance();
}