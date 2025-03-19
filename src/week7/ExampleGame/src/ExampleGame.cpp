// Finalized submission for Assignment #3: Physics Playground (including bonus)

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

ExampleGame* ExampleGame::s_pInstance = NULL;

ExampleGame::ExampleGame()
    : m_pSceneCamera(nullptr),
    m_pGameObjectManager(nullptr),
    m_state(GAMEPLAY),
    m_pCharacter(nullptr)
{
}

ExampleGame::~ExampleGame() {
    // Ensure that the managers are cleaned up in Shutdown()
    assert(m_pGameObjectManager == nullptr);
    assert(m_pSceneCamera == nullptr);
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

    Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().RegisterDirect(
        "GetGame", &ExampleGame::GetGame);

    Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().RegisterDirect(
        "LuaPrint", &ExampleGame::LuaPrint);

    metaTable.RegisterObjectDirect("GetGameObjectManager", (ExampleGame*)0, &ExampleGame::GetGameObjectManager);

    //metaTable.RegisterObjectDirect("GetGameObjectManager", this, &ExampleGame::GetGameObjectManager);
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


bool ExampleGame::Init() {
    s_pInstance = this;
    m_state = GAMEPLAY;  // Start in gameplay state

#if defined(_WIN32)
    LuaScriptManager::CreateInstance();
#endif
    SceneManager::CreateInstance();

#pragma region Camera

    // Create and attach a scene camera.
    /*  m_pSceneCamera = new Common::SceneCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f,
        glm::vec3(0.0f, 15.0f, 25.0f),
        glm::vec3(0.0f, 5.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    SceneManager::Instance()->AttachCamera(m_pSceneCamera);*/
#pragma endregion


    // Create GameObjectManager.
    m_pGameObjectManager = new GameObjectManager();

    // Register component factories.
    m_pGameObjectManager->RegisterComponentFactory("GOC_AnimController", ComponentAnimController::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_RenderableMesh", ComponentRenderableMesh::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_CharacterController", ComponentCharacterController::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_BoundingVolume", BoundingVolumeComponent::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_PlayerScore", PlayerScore::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_CoinSpawner", CoinSpawner::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_LookAtCamera", ComponentLookAtCamera::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_ThirdPersonCamera", ComponentThirdPersonCamera::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_CameraSwitcher", ComponentCameraSwitcher::CreateComponent);
    m_pGameObjectManager->RegisterComponentFactory("GOC_HUD", HUD::CreateComponent);
     //m_pGameObjectManager->RegisterComponentFactory("GOC_TeeterTotter", TeeterTotter::CreateComponent);

    // Initialize Bullet physics.
    BulletPhysicsManager::CreateInstance("data/As3/physics_materials.xml",
        "data/As3/shaders/lines.vsh",
        "data/As3/shaders/lines.fsh");

    CreateWalls();

    CreateCrateStacks();


    /*
    // --- Set up cameras and (optionally) HUD ---
    {
        // Create LookAt camera GameObject.
        GameObject* pLookAtCamGO = m_pGameObjectManager->CreateGameObject();
        ComponentLookAtCamera* m_pLookAtCam = static_cast<ComponentLookAtCamera*>(ComponentLookAtCamera::CreateComponent(nullptr));
        pLookAtCamGO->AddComponent(m_pLookAtCam);
        
       // Common::SceneCamera* m_pLookAtCam = new Common::SceneCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f,
         //   vec3(0.0f, 65.0f, 70.0f),
          //  vec3(0.0f, 0.0f, 0.0f),
         //   vec3(0.0f, 1.0f, 0.0f));
        
        //ThirdPersonCamera* m_pThirdPersonCam = new ThirdPersonCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f,
            //vec3(0.0f, 25.0f, 55.0f), 10.0f);


         //Create ThirdPerson camera GameObject.
        GameObject* pThirdPersonCamGO = m_pGameObjectManager->CreateGameObject();
        ComponentThirdPersonCamera* pThirdPersonCam = static_cast<ComponentThirdPersonCamera*>(ComponentThirdPersonCamera::CreateComponent(nullptr));
        pThirdPersonCamGO->AddComponent(pThirdPersonCam);

        // Create a camera switcher to toggle cameras (using key 'C').
        GameObject* pCamSwitcherGO = m_pGameObjectManager->CreateGameObject();
        ComponentCameraSwitcher* pCamSwitcher = static_cast<ComponentCameraSwitcher*>(ComponentCameraSwitcher::CreateComponent(nullptr));
        pCamSwitcherGO->AddComponent(pCamSwitcher);

        // (Optional) Create HUD with a pause/resume button.
        // If your HUD component supports a SetPauseCallback method, register it here.
        // GameObject* pHUD = m_pGameObjectManager->CreateGameObject();
        // HUD* pHUDComponent = new HUD(nullptr);
        // pHUDComponent->SetPauseCallback([this]() { TogglePause(); });
        // pHUD->AddComponent(pHUDComponent);
    }
    */
#if defined(_WIN32)

    GameObjectManager::ExportToLua();
    ExampleGame::ExportToLua();
    ComponentRenderableMesh::ExportToLua();
    CoinSpawner::ExportToLua();
    ComponentBase::ExportToLua();
    GameObject::ExportToLua();
    Transform::ExportToLua();

    // Call the main Lua script.
    int status = LuaScriptManager::Instance()->GetLuaState()->DoFile("data/scripts/mainXML.lua");
    if (status != 0) {
        const char* errorMsg = LuaScriptManager::Instance()->GetLuaState()->ToString(-1);
        std::cerr << "Lua script error: " << (errorMsg ? errorMsg : "") << std::endl;
        return false;
    }
    LuaPlus::LuaFunction<bool> ScriptInit = LuaScriptManager::Instance()->GetLuaState()->GetGlobal("init");
    bool result = ScriptInit();
    if (!result) {
        std::cerr << "Lua init failed" << std::endl;
        return false;
    }
#endif

    m_pCharacter = m_pGameObjectManager->GetGameObject("playerChar");
    if (!m_pCharacter) {
        std::cerr << "Warning: Character (player) not found from XML." << std::endl;
    }
    else
    {
        std::cout << "\n\nPLAYER EXITS\n";
    }

    return true;
}

bool ExampleGame::Update(float p_fDelta) {
#if defined(_WIN32)
    LuaPlus::LuaFunction<bool> ScriptUpdate = LuaScriptManager::Instance()->GetLuaState()->GetGlobal("update");
    bool luaResult = ScriptUpdate(p_fDelta);
    if (!luaResult) {
        std::cerr << "Lua update failed" << std::endl;
        return false;
    }
#endif

    // Toggle pause state on key 'P'.
    static bool bLastPKey = false;
    bool bCurrentPKey = (glfwGetKey('P') == GLFW_PRESS);
    if (bCurrentPKey && !bLastPKey) {
        TogglePause();
    }
    bLastPKey = bCurrentPKey;

    if (m_state == GAMEPLAY) {
        BulletPhysicsManager::Instance()->Update(p_fDelta);
        m_pGameObjectManager->Update(p_fDelta);
        EventManager::Instance().Update();

        // Spawn projectile on left mouse click.
        static bool bLastMouseDown = false;
        bool bCurrentMouseDown = (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        if (bCurrentMouseDown && !bLastMouseDown) {
            CreateProjectile();
        }
        bLastMouseDown = bCurrentMouseDown;
    }
    return true;
}

void ExampleGame::Render() {
    m_pGameObjectManager->SyncTransforms();
    SceneManager::Instance()->Render();
    BulletPhysicsManager::Instance()->Render(SceneManager::Instance()->GetCamera()->GetProjectionMatrix(),
        SceneManager::Instance()->GetCamera()->GetViewMatrix());
    if (m_state == PAUSED) {
        RenderPauseOverlay();
    }
}

void ExampleGame::Shutdown() {
    m_pGameObjectManager->DestroyAllGameObjects();
    delete m_pGameObjectManager;
    m_pGameObjectManager = nullptr;

    if (m_pSceneCamera) {
        delete m_pSceneCamera;
        m_pSceneCamera = nullptr;
    }

    SceneManager::DestroyInstance();
    BulletPhysicsManager::DestroyInstance();
#if defined(_WIN32)
    LuaScriptManager::DestroyInstance();
#endif
}

void ExampleGame::CreateWalls() {
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
        //btVector3 halfExtents(wall.scale.x , wall.scale.y , wall.scale.z );
        pWallRB->Init(new btBoxShape(halfExtents), "Wall", 0.0f, vec3(0.0f), false);
    }
}

void ExampleGame::CreateCrateStacks() {
    vector<vec3> stackPositions = {
        vec3(-10.0f, 0.0f, -10.0f),
        vec3(10.0f, 0.0f, -10.0f),
        vec3(-10.0f, 0.0f, 10.0f),
        vec3(10.0f, 0.0f, 10.0f)
    };
    const int cratesPerStack = 9;
    const float crateHeight = 2.0f;
    for (auto& pos : stackPositions) {
        for (int i = 0; i < cratesPerStack; ++i) {
            GameObject* pCrate = m_pGameObjectManager->CreateGameObject();
            vec3 cratePos = pos;
            cratePos.y += i * crateHeight;
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
            pCrateRB->Init(new btBoxShape(halfExtents), "Crate", 2.0f, vec3(0.0f, -1.5f, 0.0f), false);
        }
    }
}

void ExampleGame::CreateProjectile() {
    GameObject* pProjectile = m_pGameObjectManager->CreateGameObject();
    // Spawn projectile at camera position.
    pProjectile->GetTransform().SetTranslation(SceneManager::Instance()->GetCamera()->GetPosition());
    ComponentRenderableMesh* pProjMesh = new ComponentRenderableMesh();
    pProjMesh->Init("data/As1/props/ball.pod",
        "data/As1/props/",
        "data/shaders/textured.vsh",
        "data/shaders/textured.fsh");

    pProjectile->AddComponent(pProjMesh);
    ComponentRigidBody* pProjRB = new ComponentRigidBody();
    pProjectile->AddComponent(pProjRB);
    pProjRB->Init(new btSphereShape(0.5f), "Projectile", 1.0f, vec3(0.0f), false);
    // Fire projectile in the camera's look direction.
    vec3 lookDir = SceneManager::Instance()->GetCamera()->GetLookDirection();
    float projectileSpeed = 50.0f;
    btVector3 velocity(lookDir.x * projectileSpeed, lookDir.y * projectileSpeed, lookDir.z * projectileSpeed);
    pProjRB->GetRigidBody()->setLinearVelocity(velocity);
}

void ExampleGame::TogglePause() {
    m_state = (m_state == GAMEPLAY) ? PAUSED : GAMEPLAY;
    // Optionally: Update a HUD button to display "Resume" or "Pause" accordingly.
}

void ExampleGame::RenderPauseOverlay() {
    // BUTTONS??

}

void ExampleGame::CreateTeeterTotter() {
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
    pFulcrumRB->Init(new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)), "Fulcrum", 0.0f, vec3(0.0f), false);

    btRigidBody* bodyA = pPlankRB->GetRigidBody();
    btRigidBody* bodyB = pFulcrumRB->GetRigidBody();
    vec3 pivotInA(0.0f, 0.0f, 0.0f);
    vec3 pivotInB(0.0f, 0.0f, 0.0f);
    vec3 axisInA(0.0f, 0.0f, 1.0f);
    btHingeConstraint* hinge = new btHingeConstraint(*bodyA, *bodyB,
        btVector3(pivotInA.x, pivotInA.y, pivotInA.z),
        btVector3(pivotInB.x, pivotInB.y, pivotInB.z),
        btVector3(axisInA.x, axisInA.y, axisInA.z),
        btVector3(axisInA.x, axisInA.y, axisInA.z));
    BulletPhysicsManager::Instance()->GetWorld()->addConstraint(hinge, true);
}
