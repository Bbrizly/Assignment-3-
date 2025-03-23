#ifndef THIRDPERSONCAMERA_H
#define THIRDPERSONCAMERA_H

#include "common/SceneCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace week7 {

    class ThirdPersonCamera : public Common::SceneCamera
    {
    public:
        ThirdPersonCamera(float p_fFOV, float p_fAspectRatio, float p_fNearClip, float p_fFarClip,
            const glm::vec3& target, float distance);

        void Update(float deltaTime);
        void Rotate(float yaw, float pitch, const glm::vec2& mouseMovement);
        void SetTarget(const glm::vec3& target);
        void SwitchShoulder();

        bool lockMouse = true;

    private:
        glm::vec2 currentMousePos;
        glm::vec2 m_lastMousePos;
        
        glm::vec2 getMousePos() const;
        glm::vec3 m_target;
        mutable glm::mat4 m_mViewMatrix;
        float m_sensitivity = 0.1;
        float m_distance;
        float m_yaw = 0;
        float m_pitch = 0;
        glm::vec3 m_offsetLeft = glm::vec3(2.0f,0.0f,0.0f);
        glm::vec3 m_offsetRight = glm::vec3(0.0f, 0.0f, 2.0f);
        bool m_isLeftShoulder;
        void Rotate(float yaw, float pitch);
        void UpdateCameraPosition();
        void ProcessMouseInput();
        const glm::mat4& GetViewMatrix() const override;

    };

} // namespace week7

#endif // THIRDPERSONCAMERA_H