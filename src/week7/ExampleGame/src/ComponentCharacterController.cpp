//------------------------------------------------------------------------
// ComponentCharacterController
// 
// Movement now sets Bullet rigid body velocity so we cannot pass through walls.
//
// NOTE: The player GameObject must also have a "GOC_RigidBody" with e.g.:
//     <Param name="shape" value="capsule" />
// or a box shape, or a sphere, etc. in your XML so that collisions are handled.
//------------------------------------------------------------------------

#include "ComponentCharacterController.h"
#include "iostream"
#include "glfw/include/GL/glfw.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;
using namespace std;
using namespace week7;
using namespace Common;

ComponentCharacterController::ComponentCharacterController()
{
    memset(m_bKeysDown, 0, sizeof(m_bKeysDown));
    memset(m_bKeysDownLast, 0, sizeof(m_bKeysDownLast));
}

ComponentCharacterController::~ComponentCharacterController()
{
}



Common::ComponentBase* ComponentCharacterController::CreateComponent(TiXmlNode* p_pNode)
{
    // minimal check
    if (strcmp(p_pNode->Value(), "GOC_CharacterController") != 0) {
        return nullptr;
    }
    return new ComponentCharacterController();
}

void ComponentCharacterController::Update(float p_fDelta)
{
    if (!m_pRigidBody) {
        std::cout << "\n\nNO rigidbody\n\n";
        m_pRigidBody = dynamic_cast<Common::ComponentRigidBody*>(GetGameObject()->GetComponent("GOC_RigidBody"));
        if (!m_pRigidBody) {
            std::cout << "\n\Still no rigidbody?\n\n";
            return;
        }
    }
    m_pRigidBody->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0));
    
    if (!m_pAnimComponent) {
        m_pAnimComponent = dynamic_cast<ComponentAnimController*>(
            GetGameObject()->GetComponent("GOC_AnimController"));
        if (m_pAnimComponent) {
            m_pAnimComponent->SetAnim("idle");
        }
    }

    for (int i = 0; i < 256; ++i) {
        m_bKeysDownLast[i] = m_bKeysDown[i];
        m_bKeysDown[i] = (glfwGetKey(i) == GLFW_PRESS);
    }

    isRunning = (glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS);
    float moveSpeed = (isRunning ? m_fplayerSprintSpeed : m_fplayerWalkSpeed);

    SceneCamera* cam = SceneManager::Instance()->GetCamera();
    if (!cam) return;

    mat4 viewMatrix = cam->GetViewMatrix();
    
    vec3 camForward = normalize(vec3(viewMatrix[2].x, 0.0f, -viewMatrix[2].z));
    vec3 camRight = normalize(vec3(-viewMatrix[0].x, 0.0f, viewMatrix[0].z));

    camForward = normalize(camForward);
    camRight = normalize(-camRight);

    vec3 moveDir(0.f);

    if (m_bKeysDown['w'] || m_bKeysDown['W']) {
        moveDir += camForward;
    }
    if (m_bKeysDown['s'] || m_bKeysDown['S']) {
        moveDir -= camForward;
    }
    if (m_bKeysDown['a'] || m_bKeysDown['A']) {
        moveDir -= camRight;
    }
    if (m_bKeysDown['d'] || m_bKeysDown['D']) {
        moveDir += camRight;
    }

    static bool bLastMouseDown = false;
    bool bCurrentMouseDown = (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    if (bCurrentMouseDown && !bLastMouseDown) {
        CreateProjectile();
    }
    bLastMouseDown = bCurrentMouseDown;

    btRigidBody* body = m_pRigidBody->GetRigidBody();

    float lenMove = length(moveDir);
    if (lenMove > 0.001f) {
        moveDir = normalize(moveDir) * moveSpeed;

        btVector3 newVel(moveDir.x, 0, moveDir.z);
        body->setLinearVelocity(newVel);

        if (m_pAnimComponent) {
            if (isRunning && !isRunningAnim) {
                m_pAnimComponent->SetAnim("standard_run");
                isRunningAnim = true;
                isWalkingAnim = false;
            }
            else if (!isRunning && !isWalkingAnim) {
                m_pAnimComponent->SetAnim("walking");
                isRunningAnim = false;
                isWalkingAnim = true;
            }
        }
    }
    else {

        btVector3 velocity = body->getLinearVelocity();
        velocity.setX(0.f);
        velocity.setZ(0.f);
        body->setLinearVelocity(velocity);

        if (m_pAnimComponent && (isRunningAnim || isWalkingAnim)) {
            m_pAnimComponent->SetAnim("idle");
            isRunningAnim = false;
            isWalkingAnim = false;
        }
    }
}

void ComponentCharacterController::CreateProjectile()
{




    GameObject* pProjectile = GetGameObject()->GetManager()->CreateGameObject();
    // spawn at camera pos
    SceneCamera* cam = SceneManager::Instance()->GetCamera();
    if (!cam) return;



    //GameObject* pCrate = GetGameObject()->GetManager()->CreateGameObject();
    ////vec3 cratePos = pos + vec3(0.f, i * crateHeight, 0.f);
    //pCrate->GetTransform().SetTranslation(cam->GetPos());

    //ComponentRenderableMesh* pCrateMesh = new ComponentRenderableMesh();
    //pCrateMesh->Init("data/As1/props/crate.pod",
    //    "data/As1/props/",
    //    "data/As1/shaders/textured.vsh",
    //    "data/As1/shaders/textured.fsh");
    //pCrate->AddComponent(pCrateMesh);

    //ComponentRigidBody* pCrateRB = new ComponentRigidBody();
    //pCrate->AddComponent(pCrateRB);
    //btVector3 halfExtents(1.5f, 1.5f, 1.5f);
    //pCrateRB->Init(new btBoxShape(halfExtents), "Crate", 2.0f, vec3(0.f), false);


    pProjectile->GetTransform().SetTranslation(cam->GetPos());

    ComponentRenderableMesh* pProjMesh = new ComponentRenderableMesh();
    pProjMesh->Init("data/As1/props/ball.pod",
        "data/As1/props/",
        "data/shaders/textured.vsh",
        "data/shaders/textured.fsh");
    pProjectile->AddComponent(pProjMesh);

    ComponentRigidBody* pProjRB = new ComponentRigidBody();
    pProjectile->AddComponent(pProjRB);
    pProjRB->Init(new btSphereShape(1.0f), "Projectile", 1.0f, vec3(0.0f), false);

    vec3 lookDir = cam->GetLookDirection();
    float projectileSpeed = 50.f;
    btVector3 velocity(lookDir.x * projectileSpeed,
        lookDir.y * projectileSpeed,
        lookDir.z * projectileSpeed);
    pProjRB->GetRigidBody()->setLinearVelocity(velocity);
}
