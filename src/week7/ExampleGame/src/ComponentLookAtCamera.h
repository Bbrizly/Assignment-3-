#pragma once
#include "ComponentBase.h"
#include "SceneManager.h"
#include "SceneCamera.h"
#include "tinyxml.h"
#include <glm/glm.hpp>
#include <string>

namespace Common {

    // A component that controls a "LookAt" camera
    class ComponentLookAtCamera : public ComponentBase
    {
    public:
        ComponentLookAtCamera()
            : m_camera(nullptr), m_fov(45.0f), m_aspect(1.77f),
            m_near(0.1f), m_far(1000.0f),
            m_position(0.0f, 65.0f, 70.0f),
            m_lookAt(0.0f, 0.0f, 0.0f),
            m_up(0.0f, 1.0f, 0.0f)
        {
        }

        virtual ~ComponentLookAtCamera()
        {
            if (Common::SceneManager::Instance()->GetCamera() == m_camera)
            {
                Common::SceneManager::Instance()->AttachCamera(nullptr);
            }
            delete m_camera;
        }

        static ComponentBase* CreateComponent(TiXmlNode* pNode)
        {
            if (std::strcmp(pNode->Value(), "LookAtCamera") != 0 &&
                std::strcmp(pNode->Value(), "GOC_LookAtCamera") != 0)
            {
                return nullptr;
            }

            ComponentLookAtCamera* comp = new ComponentLookAtCamera();

            TiXmlNode* pChild = pNode->FirstChild();
            while (pChild)
            {
                if (std::strcmp(pChild->Value(), "Param") == 0)
                {
                    TiXmlElement* elem = pChild->ToElement();
                    const char* szName = elem->Attribute("name");
                    const char* szValue = elem->Attribute("value");
                    if (szName && szValue)
                    {
                        if (strcmp(szName, "fov") == 0) { comp->m_fov = (float)atof(szValue); }
                        else if (strcmp(szName, "aspect") == 0) { comp->m_aspect = (float)atof(szValue); }
                        else if (strcmp(szName, "nearPlane") == 0) { comp->m_near = (float)atof(szValue); }
                        else if (strcmp(szName, "farPlane") == 0) { comp->m_far = (float)atof(szValue); }
                        else if (strcmp(szName, "position") == 0)
                        {
                            float x, y, z;
                            sscanf(szValue, "%f %f %f", &x, &y, &z);
                            comp->m_position = glm::vec3(x, y, z);
                        }
                        else if (strcmp(szName, "lookAt") == 0)
                        {
                            float x, y, z;
                            sscanf(szValue, "%f %f %f", &x, &y, &z);
                            comp->m_lookAt = glm::vec3(x, y, z);
                        }
                        else if (strcmp(szName, "up") == 0)
                        {
                            float x, y, z;
                            sscanf(szValue, "%f %f %f", &x, &y, &z);
                            comp->m_up = glm::vec3(x, y, z);
                        }
                        else if (strcmp(szName, "playerObjectName") == 0)
                        {
                            //std::cout << "Lookat\n-----------\n\n\n\n" << szValue << "\n\n\n-----";
                            comp->m_playerName = szValue;
                        }
                    }
                }
                pChild = pChild->NextSibling();
            }
            //comp->CreatCamerea();

            return comp;
        }

        /*void CreatCamerea()
        {
            m_camera = new SceneCamera(
                m_fov, m_aspect, m_near, m_far,
                m_position, m_lookAt, m_up
            );
        }*/

        virtual void Update(float deltaTime) override
        {
            if (!m_camera)
            {
                m_camera = new SceneCamera(
                    m_fov, m_aspect, m_near, m_far,
                    m_position, m_lookAt, m_up
                );

            }

            //std::cout << "\n\Qaa\n\n";
            if (Common::SceneManager::Instance()->GetCamera() == m_camera)
            {
                //std::cout << "\n\ZZZZZ\n\n";
                if (GetGameObject() && GetGameObject()->GetManager())
                {
                    GameObject* playerGO = GetGameObject()->GetManager()->GetGameObject(m_playerName);
                    //std::cout << m_playerName << " - - WHAT\n\n";
                    if (playerGO)
                    {
                        //std::cout << "\n\FUCK\n\n";
                        glm::vec3 playerPos = playerGO->GetTransform().GetTranslation();
                        m_camera->SetTarget(playerPos);
                    }
                    
                }
            }
        }

        void MakeActive()
        {
            std::cout << "\n\ 1A\n\n";
            if (m_camera)
            {
                Common::SceneManager::Instance()->AttachCamera(m_camera);
            }
        }

        SceneCamera* GetCamera() { return m_camera; }

        virtual const std::string FamilyID()    override { return "GOC_LookAtCamera"; }
        virtual const std::string ComponentID() override { return "GOC_LookAtCamera"; }

    private:
        SceneCamera* m_camera;
        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
        glm::vec3 m_position;
        glm::vec3 m_lookAt;
        glm::vec3 m_up;
        std::string m_playerName;
    };

} // namespace Common
