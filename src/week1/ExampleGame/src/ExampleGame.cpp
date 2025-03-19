//------------------------------------------------------------------------
// ExampleGame
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// Main game class. Derives from week1::Game.
//------------------------------------------------------------------------

#include "ExampleGame.h"
#include "W_Model.h"

using namespace week1;


struct DirectionalLight
{
	glm::vec3		m_vDirection;
	wolf::Color4    m_ambient;
	wolf::Color4	m_diffuse;
	wolf::Color4	m_specular;
	DirectionalLight() : m_diffuse(0,0,0,0), m_specular(0,0,0,0), m_ambient(0,0,0,0) {}    
};

static DirectionalLight g_light1;

//------------------------------------------------------------------------------
// Method:    ExampleGame
// Parameter: void
// Returns:   
// 
// Constructor
//------------------------------------------------------------------------------
ExampleGame::ExampleGame()
	:
	m_pModel(NULL)
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
	assert(!m_pModel);
}

//------------------------------------------------------------------------------
// Method:    Init
// Returns:   bool
// 
// Overridden from week1::Game::Init(). Called when it's time to initialize our
// game. Returns true on successful initialization, false otherwise. If false is
// returned, the game will shut down.
//------------------------------------------------------------------------------
bool ExampleGame::Init()
{ 
	// Initialize the light parameters
	g_light1.m_diffuse = wolf::Color4(1.0f,1.0f,1.0f,1.0f);
	g_light1.m_ambient = wolf::Color4(0.1f,0.1f,0.1f,1.0f);
	g_light1.m_specular = wolf::Color4(1.0f,1.0f,1.0f,1.0f);
	g_light1.m_vDirection = glm::vec3(0.0f,0.0f,-1.0f);

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
	return true;
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: none
// Returns:   bool
// 
// Overridden from week1::Game::Update(). Called once per frame during the main
// loop of the game. The game state is updated here. Returns true on success, 
// false otherwise. If false is returned, the game will shutdown.
//------------------------------------------------------------------------------
bool ExampleGame::Update()
{
	m_pModel->Update(0.016f /* Week 1 Exercise: Make this a delta time */);
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
void ExampleGame::Render()
{
	for (wolf::Material* pMaterial : m_pModel->GetMaterials())
	{
		// Set the light parameters
		pMaterial->SetUniform("ViewDir", glm::vec3(0.0f, 0.0f, 1.0f));
		pMaterial->SetUniform("LightAmbient", g_light1.m_ambient);
		pMaterial->SetUniform("LightDiffuse", g_light1.m_diffuse);
		pMaterial->SetUniform("LightSpecular", g_light1.m_specular);
		pMaterial->SetUniform("LightDir", g_light1.m_vDirection);
	}


	// Set the world/view/proj matrices
	glm::mat4 mProj = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	glm::mat4 mView = glm::lookAt(glm::vec3(0.0f, 90.0f, 500.0f), glm::vec3(0.0f, 90.0f, 300.0f) - glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 mWorld = glm::rotate(0.0f, 0.0f, 1.0f, 0.0f);

	// Render the model
	m_pModel->SetTransform(mWorld);
	m_pModel->Render(mView,mProj);
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
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = NULL;
	}
}