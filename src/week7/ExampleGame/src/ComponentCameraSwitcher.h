#pragma once
#include "ComponentBase.h"
#include "ComponentLookAtCamera.h"
#include "ComponentThirdPersonCamera.h"
#include "tinyxml.h"
#include <string>
#include <GL/glfw.h> 

namespace Common {

    class ComponentCameraSwitcher : public ComponentBase
    {
    public:
        ComponentCameraSwitcher()
            : m_pLookAt(nullptr), m_pThirdPerson(nullptr), m_bUsingThirdPersonCam(true)
        {}

        virtual ~ComponentCameraSwitcher() {}

        static ComponentBase* CreateComponent(TiXmlNode* pNode)
        {
            if (std::strcmp(pNode->Value(), "CameraSwitcher") != 0 &&
                std::strcmp(pNode->Value(), "GOC_CameraSwitcher") != 0)
            {
                return nullptr;
            }
            return new ComponentCameraSwitcher();
        }

        virtual void Update(float deltaTime) override {
            if (GetGameObject()) {
                if (!m_pLookAt)
                    m_pLookAt = dynamic_cast<ComponentLookAtCamera*>(GetGameObject()->GetComponent("GOC_LookAtCamera"));
                if (!m_pThirdPerson)
                    m_pThirdPerson = dynamic_cast<ComponentThirdPersonCamera*>(GetGameObject()->GetComponent("GOC_ThirdPersonCamera"));
            }



            if (!m_pLookAt && !m_pThirdPerson)
                return;

            static bool bLast = false;
             bool pressed = (glfwGetKey('C') == GLFW_PRESS || glfwGetKey('c') == GLFW_PRESS);
            if (pressed && !bLast) {
                Toggle();
            }
            bLast = pressed;
            //return;



            if (m_bUsingThirdPersonCam && m_pThirdPerson) {
                m_pThirdPerson->Update(deltaTime);
            }
            else if (!m_bUsingThirdPersonCam && m_pLookAt) {
                m_pLookAt->Update(deltaTime);
            }
        }



        void Toggle()
        {

            m_bUsingThirdPersonCam = !m_bUsingThirdPersonCam;
            if (m_bUsingThirdPersonCam)
            {
                if (m_pThirdPerson)
                {
                SceneManager::Instance()->AttachCamera(m_pThirdPerson->GetCamera());
                }

            }
            else
            {
                if (m_pLookAt) {
                    SceneManager::Instance()->AttachCamera(m_pLookAt->GetCamera());

                }
            }
        }

        virtual const std::string FamilyID()    override { return "GOC_CameraSwitcher"; }
        virtual const std::string ComponentID() override { return "GOC_CameraSwitcher"; }

    private:
        ComponentLookAtCamera* m_pLookAt;
        ComponentThirdPersonCamera* m_pThirdPerson;
        bool m_bUsingThirdPersonCam;
    };

} // namespace Common
