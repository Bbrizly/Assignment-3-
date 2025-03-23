#include "ExampleGame.h"
#include "ComponentAnimController.h"
#include "ComponentCharacterController.h"
#include "ComponentRenderableMesh.h"
#include "ComponentRigidBody.h"
#include "BulletPhysicsManager.h"
#include "SceneManager.h"
#include "W_Model.h"
#include "CoinSpawner.h"
#include "PlayerScore.h"
#include "HUD.h"
#include "ComponentLookAtCamera.h"
#include "ComponentThirdPersonCamera.h"
#include "ComponentCameraSwitcher.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef _WIN32
#include "LuaScriptManager.h"
#include "LuaPlus.h"
#include "windows.h"
#endif

using namespace std;
using namespace glm;
using namespace week7;
using namespace Common;

ExampleGame* ExampleGame::s_pInstance = nullptr;

ExampleGame::ExampleGame()
    : m_pGameObjectManager(nullptr),
    m_state(GAMEPLAY),
    m_pCharacter(nullptr),
    m_pSceneCamera(nullptr)
{
}

ExampleGame::~ExampleGame() {
    // Ensure the managers are cleaned up in Shutdown()
    assert(m_pGameObjectManager == nullptr);
}

//-------------------------------------
// Lua bridging
#if defined(_WIN32)
void ExampleGame::ExportToLua()
{
    LuaPlus::LuaObject metaTable =
        Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().CreateTable("GameMetaTable");
    metaTable.SetObject("__index", metaTable);

    Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().RegisterDirect(
        "GetGame", &ExampleGame::GetGame);

    Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().RegisterDirect(
        "LuaPrint", &ExampleGame::LuaPrint);

    metaTable.RegisterObjectDirect("GetGameObjectManager", (ExampleGame*)0, &ExampleGame::GetGameObjectManager);
}

LuaPlus::LuaObject ExampleGame::GetGame()
{
    LuaPlus::LuaObject luaInstance;
    luaInstance.AssignNewTable(Common::LuaScriptManager::Instance()->GetLuaState());

    luaInstance.SetLightUserData("__object", s_pInstance);

    LuaPlus::LuaObject metaTable =
        Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().GetByName("GameMetaTable");
    luaInstance.SetMetaTable(metaTable);

    return luaInstance;
}

void ExampleGame::LuaPrint(const char* p_strDebugString)
{
#ifdef _WIN32
    char buffer[1024];
    sprintf_s(buffer, "LUA PRINT[%s]\n", p_strDebugString);
    OutputDebugStringA(buffer);
#else
    std::cout << "LUA PRINT[" << (p_strDebugString ? p_strDebugString : "") << "]\n";
#endif
}

LuaPlus::LuaObject ExampleGame::GetGameObjectManager()
{
    // Return the manager as a Lua object
    LuaPlus::LuaObject luaInstance;
    luaInstance.AssignNewTable(Common::LuaScriptManager::Instance()->GetLuaState());
    luaInstance.SetLightUserData("__object", m_pGameObjectManager);

    LuaPlus::LuaObject metaTable =
        Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().GetByName("GameObjectManagerMetaTable");
    luaInstance.SetMetaTable(metaTable);

    return luaInstance;
}
#endif
//-------------------------------------

bool ExampleGame::Init()
{
    s_pInstance = this;
    m_state = GAMEPLAY;

#if defined(_WIN32)
    LuaScriptManager::CreateInstance();
#endif
    SceneManager::CreateInstance();

    // Create GameObjectManager
    m_pGameObjectManager = new GameObjectManager();

    // Register component factories
    m_pGameObjectManager->RegisterComponentFactory("GOC_AnimController", ComponentAnimController::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_RenderableMesh", ComponentRenderableMesh::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_CharacterController", ComponentCharacterController::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_BoundingVolume", BoundingVolumeComponent::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_PlayerScore", PlayerScore::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_CoinSpawner", CoinSpawner::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_LookAtCamera", ComponentLookAtCamera::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_ThirdPersonCamera", ComponentThirdPersonCamera::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_CameraSwitcher", ComponentCameraSwitcher::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_RigidBody", ComponentRigidBody::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_HUD", HUD::CreateComponent);

    // Initialize Bullet
    BulletPhysicsManager::CreateInstance(
        "data/As3/physics_materials.xml",
        "data/As3/shaders/lines.vsh",
        "data/As3/shaders/lines.fsh"
    );

    //BulletPhysicsManager::Instance()->ToggleDebugRendering();

    // Create environment
    //CreateWalls();
    //CreateCrateStacks();
    //CreateLamppost();
    CreateTeeterTotter();

#if defined(_WIN32)
    GameObjectManager::ExportToLua();
    ExampleGame::ExportToLua();
    ComponentRenderableMesh::ExportToLua();
    CoinSpawner::ExportToLua();
    ComponentBase::ExportToLua();
    GameObject::ExportToLua();
    Transform::ExportToLua();
    ComponentRigidBody::ExportToLua();

    int status =
        LuaScriptManager::Instance()->GetLuaState()->DoFile("data/scripts/mainXML.lua");
    if (status != 0) {
        const char* errorMsg =
            LuaScriptManager::Instance()->GetLuaState()->ToString(-1);
        std::cerr << "Lua script error: " << (errorMsg ? errorMsg : "") << std::endl;
        // We continue anyway
    }
    LuaPlus::LuaFunction<bool> ScriptInit =
        LuaScriptManager::Instance()->GetLuaState()->GetGlobal("init");
    bool result = ScriptInit();
    if (!result) {
        std::cerr << "Lua init failed" << std::endl;
    }
#endif

    m_pCharacter = m_pGameObjectManager->GetGameObject("playerChar");
    if (!m_pCharacter) {
        std::cerr << "[WARNING] Player character object not found in XML.\n";
    }

    // Done
    return true;
}

bool ExampleGame::Update(float p_fDelta)
{
    static bool bLastPKey = false;
    bool bCurrentPKey = (glfwGetKey('P') == GLFW_PRESS);
    if (bCurrentPKey && !bLastPKey) {
        TogglePause();
    }
    bLastPKey = bCurrentPKey;

    if (m_state == GAMEPLAY) {
        // normal updates
        BulletPhysicsManager::Instance()->Update(p_fDelta);
        m_pGameObjectManager->Update(p_fDelta);
        EventManager::Instance().Update();
    }
    else if (m_state == PAUSED) {
        m_pGameObjectManager->Update(0.0f);
    }

    return true;
}

void ExampleGame::Render()
{
    m_pGameObjectManager->SyncTransforms();
    SceneManager::Instance()->Render();

    BulletPhysicsManager::Instance()->Render(
        SceneManager::Instance()->GetCamera()->GetProjectionMatrix(),
        SceneManager::Instance()->GetCamera()->GetViewMatrix());

    if (m_state == PAUSED) {
        RenderPauseOverlay();
    }
}

void ExampleGame::Shutdown()
{
    m_pGameObjectManager->DestroyAllGameObjects();
    delete m_pGameObjectManager;
    m_pGameObjectManager = nullptr;

    SceneManager::DestroyInstance();
    BulletPhysicsManager::DestroyInstance();
#if defined(_WIN32)
    LuaScriptManager::DestroyInstance();
#endif
}

void ExampleGame::CreateTeeterTotter()
{
    // Create the teeter totter plank.
    GameObject* pPlankObj = m_pGameObjectManager->CreateGameObject();

    // Use a crate mesh (or any suitable asset) to represent the plank.
    ComponentRenderableMesh* pPlankMesh = new ComponentRenderableMesh();
    pPlankMesh->Init("data/As1/props/crate.pod",
        "data/As1/props/",
        "data/As1/shaders/textured.vsh",
        "data/As1/shaders/textured.fsh");
    pPlankObj->AddComponent(pPlankMesh);

    // Set the plank's transform:
    // Position the plank at a height of 5 units above the ground and 30 units into the scene.
    pPlankObj->GetTransform().SetTranslation(vec3(0.0f, 5.0f, 30.0f));
    // Scale the plank to be long and wide: 20 units long, 0.5 units high, and 2 units wide.
    pPlankObj->GetTransform().SetScale(vec3(10.0f, 0.5f, 1.0f));

    // Add a rigid body to the plank.
    ComponentRigidBody* pPlankRB = new ComponentRigidBody();
    pPlankObj->AddComponent(pPlankRB);
    // Calculate half-extents for the box shape from the scale.
    btVector3 plankHalfExtents(10.0f, 0.25f, 1.0f);
    // Make the plank dynamic (mass > 0) so that it can swing.
    pPlankRB->Init(new btBoxShape(plankHalfExtents), "TeeterPlank", 5.0f, vec3(0.0f), false);

    // Create the fulcrum (pivot support) for the teeter totter.
    GameObject* pFulcrumObj = m_pGameObjectManager->CreateGameObject();

    // Position the fulcrum below the middle of the plank.
    // For example, place it 2 units below the plank's center.
    pFulcrumObj->GetTransform().SetTranslation(vec3(0.0f, 3.0f, 30.0f));
    // Scale the fulcrum to be a modest cube (2 units per side).
    pFulcrumObj->GetTransform().SetScale(vec3(2.0f, 2.0f, 2.0f));

    // Optionally, give the fulcrum a mesh (using the crate mesh here).
    ComponentRenderableMesh* pFulcrumMesh = new ComponentRenderableMesh();
    pFulcrumMesh->Init("data/As1/props/crate.pod",
        "data/As1/props/",
        "data/As1/shaders/textured.vsh",
        "data/As1/shaders/textured.fsh");
    pFulcrumObj->AddComponent(pFulcrumMesh);

    // Add a rigid body to the fulcrum.
    ComponentRigidBody* pFulcrumRB = new ComponentRigidBody();
    pFulcrumObj->AddComponent(pFulcrumRB);
    // Use a small static box for the fulcrum.
    btVector3 fulcrumHalfExtents(1.0f, 1.0f, 1.0f);
    pFulcrumRB->Init(new btBoxShape(fulcrumHalfExtents), "Fulcrum", 0.0f, vec3(0.0f), false);

    // Retrieve the rigid bodies.
    btRigidBody* bodyPlank = pPlankRB->GetRigidBody();
    btRigidBody* bodyFulcrum = pFulcrumRB->GetRigidBody();

    // Define pivot points in each body's local space.
    // For the plank, we set the pivot at its bottom center.
    btVector3 pivotPlank(0, -0.25f, 0);
    // For the fulcrum, set the pivot at its top center.
    btVector3 pivotFulcrum(0, 1.0f, 0);

    // Define the hinge axis (rotate around the Z axis, so the plank will tilt forward/back).
    btVector3 hingeAxis(0, 0, 1);

    // Create the hinge constraint linking the plank and the fulcrum.
    btHingeConstraint* hinge = new btHingeConstraint(*bodyPlank, *bodyFulcrum,
        pivotPlank, pivotFulcrum,
        hingeAxis, hingeAxis);
    // Optionally, set limits to restrict the swing (e.g., ±90 degrees).
    hinge->setLimit(-SIMD_HALF_PI, SIMD_HALF_PI);

    // Add the constraint to the Bullet world.
    BulletPhysicsManager::Instance()->GetWorld()->addConstraint(hinge, true);
    pPlankRB->AddConstraint(hinge);
}

void ExampleGame::TogglePause()
{
    if (m_state == GAMEPLAY)
        m_state = PAUSED;
    else
        m_state = GAMEPLAY;
}

void ExampleGame::RenderPauseOverlay()
{
    // If you want, draw a big "PAUSED" across the screen or a translucent overlay
    // We can do something minimal:
    // SceneManager::Instance()->AddTextBox("PAUSED", 400, 300, 400, 100);
    // or rely on the HUD to do it
}
