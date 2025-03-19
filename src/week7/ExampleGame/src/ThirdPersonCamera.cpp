#include "ThirdPersonCamera.h"
#include "glfw/include/GL/glfw.h"
#include "iostream"

using namespace std;

using namespace week2;

ThirdPersonCamera::ThirdPersonCamera(float p_fFOV, float p_fAspectRatio, float p_fNearClip, float p_fFarClip,
    const glm::vec3& target, float distance)
    : SceneCamera(p_fFOV, p_fAspectRatio, p_fNearClip, p_fFarClip, glm::vec3(0.0f), target, glm::vec3(0.0f, 1.0f, 0.0f)),
    m_target(target), m_distance(distance), m_yaw(0.0f), m_pitch(0.0f),
    m_offsetLeft(glm::vec3(-0.5f, 0.3f, 0.0f)), m_offsetRight(glm::vec3(0.5f, 0.3f, 0.0f)),
    m_isLeftShoulder(true), m_sensitivity(0.1f)
{
    if (fabs(m_yaw) > 360.0f) m_yaw = fmod(m_yaw, 360.0f);
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;
    UpdateCameraPosition();
}

void ThirdPersonCamera::Update(float deltaTime) {
    ProcessMouseInput();
    UpdateCameraPosition();
}

void ThirdPersonCamera::ProcessMouseInput() {
    int xpos, ypos;
    glfwGetMousePos(&xpos, &ypos);
    
    glm::vec2 currentMousePos = glm::vec2(xpos, ypos);

    glm::vec2 mouseMovement = currentMousePos - m_lastMousePos;
    m_lastMousePos = currentMousePos;
    if (glm::length(mouseMovement) > 0.01f)
    {
        Rotate(mouseMovement.x * m_sensitivity, -mouseMovement.y * m_sensitivity);
        glfwSetMousePos(640, 360);
        m_lastMousePos = glm::vec2(640, 360);
    }
    static bool bLastShoulderKeyDown = false;
    bool bShoulderKeyDown = glfwGetKey('Q') || glfwGetKey('q');
    if (bShoulderKeyDown && !bLastShoulderKeyDown)
    {
        SwitchShoulder();
    }
    bLastShoulderKeyDown = bShoulderKeyDown;
}

void ThirdPersonCamera::Rotate(float yaw, float pitch) {
    m_yaw += yaw;
    m_pitch = glm::clamp(m_pitch + pitch, -89.0f, 89.0f);
}

void ThirdPersonCamera::SetTarget(const glm::vec3& target) {
    m_target = target;
}

void ThirdPersonCamera::SwitchShoulder() {
    m_isLeftShoulder = !m_isLeftShoulder;
    UpdateCameraPosition();
}

void ThirdPersonCamera::UpdateCameraPosition() {

    glm::vec3 rightVector = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f),
        glm::normalize(m_target - m_vPos)));

    // Adjust offset based on shoulder position
    glm::vec3 offset = (m_isLeftShoulder ? -rightVector : rightVector) * 4.0f;
    glm::vec3 direction = glm::normalize(glm::vec3(
        cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
        sin(glm::radians(m_pitch)),
        sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
    ));
    

    glm::vec3 position = m_target - direction * m_distance;
    SetPos(position);

    SetTarget(m_target + offset);
}

const glm::mat4& ThirdPersonCamera::GetViewMatrix() const {


    glm::vec3 forward = glm::normalize(m_target - m_vPos);
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward));
    glm::vec3 up = glm::cross(forward, right);

    m_mViewMatrix = glm::lookAt(m_vPos, m_target, up);
    return m_mViewMatrix;
}