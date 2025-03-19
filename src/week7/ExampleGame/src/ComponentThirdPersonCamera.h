#pragma once
#include "ComponentBase.h"
#include "tinyxml.h"
#include <glm/glm.hpp>
#include <string>
#include "SceneCamera.h"
#include "ThirdPersonCamera.h"
#include "GameObjectManager.h"
#include "SceneManager.h"

namespace Common {

    class ComponentThirdPersonCamera : public ComponentBase
    {
    public:
        ComponentThirdPersonCamera()
            : m_camera(nullptr),
            m_fov(45.f),
            m_aspect(1.7778f),
            m_near(0.1f),
            m_far(1000.f),
            m_distance(30.0f),
            m_targetOffset(0.0f, 0.0f, 0.0f),
            m_playerName(""),          
            m_playerGameObject(nullptr)
        {
        }

        virtual ~ComponentThirdPersonCamera()
        {
            if (SceneManager::Instance() &&
                SceneManager::Instance()->GetCamera() == m_camera)
            {
                SceneManager::Instance()->AttachCamera(nullptr);
            }
            delete m_camera;
            m_camera = nullptr;
        }

        static ComponentBase* CreateComponent(TiXmlNode* pNode)
        {
            if (std::strcmp(pNode->Value(), "GOC_ThirdPersonCamera") != 0)
            {
                return nullptr;
            }

            ComponentThirdPersonCamera* comp = new ComponentThirdPersonCamera();

            //cout << "\n\nCREATING THIRD PERSON CAMERA\n\n";


            TiXmlNode* child = pNode->FirstChild();
            while (child)
            {
                if (std::strcmp(child->Value(), "Param") == 0)
                {
                    TiXmlElement* elem = child->ToElement();
                    const char* szName = elem->Attribute("name");
                    const char* szValue = elem->Attribute("value");
                    if (szName && szValue)
                    {
                        if (strcmp(szName, "fov") == 0) { comp->m_fov = (float)atof(szValue); }
                        else if (strcmp(szName, "aspect") == 0) { comp->m_aspect = (float)atof(szValue); }
                        else if (strcmp(szName, "nearPlane") == 0) { comp->m_near = (float)atof(szValue); }
                        else if (strcmp(szName, "farPlane") == 0) { comp->m_far = (float)atof(szValue); }
                        else if (strcmp(szName, "distance") == 0) { comp->m_distance = (float)atof(szValue); }
                        else if (strcmp(szName, "targetOffset") == 0)
                        {
                            float x, y, z;
                            sscanf(szValue, "%f %f %f", &x, &y, &z);
                            comp->m_targetOffset = glm::vec3(x, y, z);
                        }
                        else if (strcmp(szName, "playerObjectName") == 0)
                        {
                            comp->m_playerName = szValue;
                        }
                    }
                }
                child = child->NextSibling();
            }
            
            return comp;
        }

        virtual void Update(float deltaTime) override
        {
            if (!SceneManager::Instance())
                return;

            

            if (!m_camera)
            {
                m_camera = new week2::ThirdPersonCamera(
                    m_fov, m_aspect, m_near, m_far,
                    glm::vec3(0.0f),
                    m_distance
                );
                SceneManager::Instance()->AttachCamera(m_camera);
            }

            if (!m_playerGameObject && GetGameObject() && GetGameObject()->GetManager())
            {
                m_playerGameObject = GetGameObject()->GetManager()->GetGameObject(m_playerName);
            }

            if (m_playerGameObject && m_camera)
            {
                glm::vec3 playerPos = m_playerGameObject->GetTransform().GetTranslation();

                m_camera->SetTarget(playerPos + m_targetOffset);

                m_camera->Update(deltaTime);
            }
        }

        void MakeActive()
        {
            if (m_camera && SceneManager::Instance())
            {
                SceneManager::Instance()->AttachCamera(m_camera);
            }
        }

        week2::ThirdPersonCamera* GetCamera() { return m_camera; }

        virtual const std::string FamilyID()    override { return "GOC_ThirdPersonCamera"; }
        virtual const std::string ComponentID() override { return "GOC_ThirdPersonCamera"; }

    private:
        week2::ThirdPersonCamera* m_camera;

        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
        float m_distance;
        glm::vec3 m_targetOffset;

        std::string m_playerName;

        GameObject* m_playerGameObject;
    };

} // namespace Common
