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

#if defined(_WIN32)
#include "windows.h"
#include "LuaScriptManager.h"
#endif

using namespace week5;

ExampleGame* ExampleGame::s_pInstance = NULL;

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

#if defined(_WIN32)
//------------------------------------------------------------------------------
// Method:    ExportToLua
// Returns:   void
// 
// Export interface to Lua.
//------------------------------------------------------------------------------
void ExampleGame::ExportToLua()
{
	LuaPlus::LuaObject metaTable = Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().CreateTable("GameMetaTable");
	metaTable.SetObject("__index", metaTable);
	Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().RegisterDirect("GetGame", &ExampleGame::GetGame);
	Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().RegisterDirect("LuaPrint", &ExampleGame::LuaPrint);
	metaTable.RegisterObjectDirect("GetGameObjectManager", (ExampleGame*)0, &ExampleGame::GetGameObjectManager);
}

//------------------------------------------------------------------------------
// Method:    GetGame
// Returns:   LuaPlus::LuaObject
// 
// Returns an instance of the Game.
//------------------------------------------------------------------------------
LuaPlus::LuaObject ExampleGame::GetGame()
{
	LuaPlus::LuaObject luaInstance;
	luaInstance.AssignNewTable(Common::LuaScriptManager::Instance()->GetLuaState());

	luaInstance.SetLightUserData("__object", s_pInstance);

	LuaPlus::LuaObject metaTable = Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().GetByName("GameMetaTable");
	luaInstance.SetMetaTable(metaTable);

	return luaInstance;
}

//------------------------------------------------------------------------------
// Method:    LuaPrint
// Parameter: const char * p_strDebugString
// Returns:   void
// 
// Prints a debug string to the console.
//------------------------------------------------------------------------------
void ExampleGame::LuaPrint(const char* p_strDebugString)
{
	static char buffer[1024];
	sprintf_s(buffer, "LUA PRINT[%s]\n", p_strDebugString);
	OutputDebugString(buffer);
}

//------------------------------------------------------------------------------
// Method:    GetGameObjectManager
// Returns:   LuaPlus::LuaObject
// 
// Returns the GameObjectManager as a LuaObject instance for use in Lua.
//------------------------------------------------------------------------------
LuaPlus::LuaObject ExampleGame::GetGameObjectManager()
{
	LuaPlus::LuaObject luaInstance;
	luaInstance.AssignNewTable(Common::LuaScriptManager::Instance()->GetLuaState());

	luaInstance.SetLightUserData("__object", m_pGameObjectManager);

	LuaPlus::LuaObject metaTable = Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().GetByName("GameObjectManagerMetaTable");
	luaInstance.SetMetaTable(metaTable);

	return luaInstance;
}
#endif

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
	// Initialize the static instance
	s_pInstance = this;

#if defined(_WIN32)
	// Initialize LuaPlus
	Common::LuaScriptManager::CreateInstance();
#endif

	// Initialize our Scene Manager
	Common::SceneManager::CreateInstance();

	// Initialize our Scene Camera and attach it to the Scene Manager
	m_pSceneCamera = new Common::SceneCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3(0.0f,5.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	Common::SceneManager::Instance()->AttachCamera(m_pSceneCamera);

	// Initialize our GameObjectManager
	m_pGameObjectManager = new Common::GameObjectManager();

	// Register factory methods
	m_pGameObjectManager->RegisterComponentFactory("GOC_AnimController", ComponentAnimController::CreateComponent);

#if defined(_WIN32)
	// Export class interfaces to Lua
	week5::ExampleGame::ExportToLua();
	week5::ComponentRenderableMesh::ExportToLua();
	week5::ComponentCharacterController::ExportToLua();
	Common::GameObjectManager::ExportToLua();
	Common::ComponentBase::ExportToLua();
	Common::GameObject::ExportToLua();
	Common::Transform::ExportToLua();

	// Call the main lua script
	int status = Common::LuaScriptManager::Instance()->GetLuaState()->DoFile("week5/ExampleGame/data/scripts/main_example1.lua");

	// Call the init method
	LuaPlus::LuaFunction<bool> ScriptInit = Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobal("init");
	bool result = ScriptInit();
#endif
    
	// Below is how we initialize the same scene in C++

	//// Create an initialized GameObject
	//Common::GameObject* pCharacter = m_pGameObjectManager->CreateGameObject("week5/ExampleGame/data/character.xml");
	//pCharacter->GetTransform().Scale(glm::vec3(0.05f, 0.05, 0.05f));

	//// Create a renderable component for it
	//ComponentRenderableMesh* pRenderableComponent = new ComponentRenderableMesh();
	//pRenderableComponent->Init("data/characters/swat/Swat.pod", "data/characters/swat/", "data/shaders/skinned.vsh", "data/shaders/skinned.fsh"); 
	//pCharacter->AddComponent(pRenderableComponent);

	// NOTE: this is just here to demonstrate how we would programmatically add this component. It's now handled inside character.xml
	// Create an animation controller component for it
	//ComponentAnimController* pAnimControllerComponent = new ComponentAnimController();
	//pAnimControllerComponent->AddAnim("idle", "data/characters/swat/anim/idle.pod", 0, 31, true);
	//pAnimControllerComponent->AddAnim("walking", "data/characters/swat/anim/walking.pod", 0, 31, true);
	//pAnimControllerComponent->AddAnim("right_strafe", "data/characters/swat/anim/right_strafe.pod", 0, 31, true);
	//pAnimControllerComponent->AddAnim("right_strafe_walking", "data/characters/swat/anim/right_strafe_walking.pod", 0, 31, true);
	//pAnimControllerComponent->AddAnim("jump", "data/characters/swat/anim/jump.pod", 0, 31, false);
	//pAnimControllerComponent->AddAnim("left_strafe", "data/characters/swat/anim/left_strafe.pod", 0, 31, true);
	//pAnimControllerComponent->AddAnim("left_strafe_walking", "data/characters/swat/anim/left_strafe_walking.pod", 0, 31, true);
	//pAnimControllerComponent->AddAnim("left_turn_90", "data/characters/swat/anim/left_turn_90.pod", 0, 31, false);
	//pAnimControllerComponent->AddAnim("right_turn_90", "data/characters/swat/anim/right_turn_90.pod", 0, 31, false);
	//pAnimControllerComponent->AddAnim("standard_run", "data/characters/swat/anim/standard_run.pod", 0, 22, true);
	//pAnimControllerComponent->SetAnim("idle");
	//pCharacter->AddComponent(pAnimControllerComponent);

	//// Create a controller component for it
	//ComponentCharacterController* pCharacterControllerComponent = new ComponentCharacterController();
	//pCharacter->AddComponent(pCharacterControllerComponent);

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
    bool result = true;
    
#if defined(_WIN32)
	// Tell Lua to update
	LuaPlus::LuaFunction<bool> ScriptUpdate = Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobal("update");
	result = ScriptUpdate(p_fDelta);
#endif
    
	m_pGameObjectManager->Update(p_fDelta);
	return result;
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

#if defined(_WIN32)
	// Destroy the Lua script manager
	Common::LuaScriptManager::DestroyInstance();
#endif
}