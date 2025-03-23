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

    BulletPhysicsManager::Instance()->ToggleDebugRendering();

    // Create environment
    //CreateWalls();
    //CreateCrateStacks();
    //CreateLamppost();
    //CreateTeeterTotter();

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

//-----------------------------------------------
// Helper to create bounding walls
void ExampleGame::CreateWalls()
{
    // 4 walls around center
    struct WallData {
        vec3 position;
        vec3 scale;
    };
    vector<WallData> walls = {
        { vec3(0.0f, 5.0f, -50.0f), vec3(50.0f, 10.0f, 0.1f) },
        { vec3(0.0f, 5.0f, 50.0f),  vec3(50.0f, 10.0f, 0.1f) },
        { vec3(-50.0f, 5.0f, 0.0f), vec3(0.1f, 10.0f, 50.0f) },
        { vec3(50.0f, 5.0f, 0.0f),  vec3(0.1f, 10.0f, 50.0f) }
    };
    for (auto& wall : walls) {
        GameObject* pWall = m_pGameObjectManager->CreateGameObject();
        // just reusing crate mesh for a quick "brick wall"
        ComponentRenderableMesh* pWallMesh = new ComponentRenderableMesh();
        pWallMesh->Init("data/As1/props/crate.pod",
            "data/As1/props/",
            "data/As1/shaders/textured.vsh",
            "data/As1/shaders/textured.fsh");
        pWall->AddComponent(pWallMesh);
        pWall->GetTransform().SetTranslation(wall.position);
        pWall->GetTransform().SetScale(wall.scale);

        ComponentRigidBody* pWallRB = new ComponentRigidBody();
        pWall->AddComponent(pWallRB);
        btVector3 halfExtents(wall.scale.x * 0.5f, wall.scale.y * 0.5f, wall.scale.z * 0.5f);
        pWallRB->Init(new btBoxShape(halfExtents), "Wall", 0.0f, vec3(0.0f), false);
    }
}

void ExampleGame::CreateCrateStacks()
{
    vector<vec3> stackPositions = {
        vec3(-10.0f, 0.0f, -10.0f),
        vec3(10.0f, 0.0f, -10.0f),
        vec3(-10.0f, 0.0f,  10.0f),
        vec3(10.0f, 0.0f,  10.0f),
    };
    const int cratesPerStack = 9;
    const float crateHeight = 2.f;

    for (auto& pos : stackPositions) {
        for (int i = 0; i < cratesPerStack; ++i) {
            GameObject* pCrate = m_pGameObjectManager->CreateGameObject();
            vec3 cratePos = pos + vec3(0.f, i * crateHeight, 0.f);
            pCrate->GetTransform().SetTranslation(cratePos);

            ComponentRenderableMesh* pCrateMesh = new ComponentRenderableMesh();
            pCrateMesh->Init("data/As1/props/crate.pod",
                "data/As1/props/",
                "data/As1/shaders/textured.vsh",
                "data/As1/shaders/textured.fsh");
            pCrate->AddComponent(pCrateMesh);

            ComponentRigidBody* pCrateRB = new ComponentRigidBody();
            pCrate->AddComponent(pCrateRB);
            btVector3 halfExtents(1.5f, 1.5f, 1.5f);
            pCrateRB->Init(new btBoxShape(halfExtents), "Crate", 2.0f, vec3(0.f), false);
        }
    }
}

void ExampleGame::CreateTeeterTotter()
{
    // Example of a hinged constraint bonus
    GameObject* pTeeter = m_pGameObjectManager->CreateGameObject();
    ComponentRenderableMesh* pPlankMesh = new ComponentRenderableMesh();

    pPlankMesh->Init("data/As1/props/ground.pod",
        "data/As1/bonus/",
        "data/shaders/textured.vsh",
        "data/shaders/textured.fsh");
    pTeeter->AddComponent(pPlankMesh);
    pTeeter->GetTransform().SetTranslation(vec3(0.0f, 0.5f, 30.0f));
    pTeeter->GetTransform().SetScale(vec3(4.0f, 0.5f, 1.0f));
    ComponentRigidBody* pPlankRB = new ComponentRigidBody();
    pTeeter->AddComponent(pPlankRB);
    btVector3 halfExtents(2.0f, 0.25f, 0.5f);
    pPlankRB->Init(new btBoxShape(halfExtents), "TeeterPlank", 5.0f, vec3(0.0f), false);

    GameObject* pFulcrum = m_pGameObjectManager->CreateGameObject();
    pFulcrum->GetTransform().SetTranslation(vec3(0.0f, 0.5f, 30.0f));
    ComponentRigidBody* pFulcrumRB = new ComponentRigidBody();
    pFulcrum->AddComponent(pFulcrumRB);
    pFulcrumRB->Init(new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)), "Fulcrum", 0.0f, vec3(0.f), false);

    btRigidBody* bodyA = pPlankRB->GetRigidBody();
    btRigidBody* bodyB = pFulcrumRB->GetRigidBody();
    vec3 pivotInA(0.f, 0.f, 0.f), pivotInB(0.f, 0.f, 0.f);
    vec3 axisInA(0.f, 0.f, 1.f);

    btHingeConstraint* hinge = new btHingeConstraint(
        *bodyA, *bodyB,
        btVector3(pivotInA.x, pivotInA.y, pivotInA.z),
        btVector3(pivotInB.x, pivotInB.y, pivotInB.z),
        btVector3(axisInA.x, axisInA.y, axisInA.z),
        btVector3(axisInA.x, axisInA.y, axisInA.z)
    );
    BulletPhysicsManager::Instance()->GetWorld()->addConstraint(hinge, true);
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
