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
    // Grab sibling RigidBody if not done yet
    if (!m_pRigidBody) {
        m_pRigidBody = dynamic_cast<Common::ComponentRigidBody*>(
            GetGameObject()->GetComponent("GOC_RigidBody"));
        if (!m_pRigidBody) {
            // can't do anything
            return;
        }
    }

    // Optionally grab anim
    if (!m_pAnimComponent) {
        m_pAnimComponent = dynamic_cast<ComponentAnimController*>(
            GetGameObject()->GetComponent("GOC_AnimController"));
        if (m_pAnimComponent) {
            m_pAnimComponent->SetAnim("idle");
        }
    }

    // gather key states
    for (int i = 0; i < 256; ++i) {
        m_bKeysDownLast[i] = m_bKeysDown[i];
        m_bKeysDown[i] = (glfwGetKey(i) == GLFW_PRESS);
    }

    // Decide walk or run
    isRunning = (glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS);
    float moveSpeed = (isRunning ? m_fplayerSprintSpeed : m_fplayerWalkSpeed);

    // We need the camera forward/right
    SceneCamera* cam = SceneManager::Instance()->GetCamera();
    if (!cam) return;

    mat4 viewMatrix = cam->GetViewMatrix();
    // Flatten
    vec3 camForward(viewMatrix[2][0], 0.0f, viewMatrix[2][2] * -1.f);
    vec3 camRight(-viewMatrix[0][0], 0.0f, viewMatrix[0][2]);

    camForward = normalize(camForward);
    camRight = normalize(camRight);

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

    float lenMove = length(moveDir);
    if (lenMove > 0.001f) {
        moveDir = normalize(moveDir) * moveSpeed;

        // Turn the character to face the movement direction if you want
        // We'll do a quick hack: yaw from movement direction
        float angle = atan2(moveDir.x, moveDir.z);
        GetGameObject()->GetTransform().SetRotation(vec3(0, degrees(angle), 0));

        // set linear velocity on the bullet rigid body
        btRigidBody* body = m_pRigidBody->GetRigidBody();
        //btVector3 velocity(moveDir.x, body->getLinearVelocity().y, moveDir.z);
        btVector3 velocity(moveDir.x, body->getLinearVelocity().getY(), moveDir.z);

        body->setLinearVelocity(velocity);

        // set animation
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
        // no movement
        btRigidBody* body = m_pRigidBody->GetRigidBody();
        // keep y velocity for gravity, zero out XZ
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
