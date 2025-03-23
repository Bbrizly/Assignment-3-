//------------------------------------------------------------------------
// ComponentCharacterController
//
// Updated to use bullet velocity for movement so that the character
// cannot clip through walls or lamppost.
//------------------------------------------------------------------------

#ifndef COMPNENTCHARACTERCONTROLLER_H
#define COMPNENTCHARACTERCONTROLLER_H

#include "ComponentBase.h"
#include "SceneManager.h"
#include "SceneCamera.h"
#include "ThirdPersonCamera.h"
#include "W_Model.h"
#include "ComponentAnimController.h"
#include "ComponentRenderableMesh.h"
#include "ComponentRigidBody.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "tinyxml.h"

namespace week7
{
    class ComponentCharacterController : public Common::ComponentBase
    {
    public:
        ComponentCharacterController();
        virtual ~ComponentCharacterController();

        static Common::ComponentBase* CreateComponent(TiXmlNode* p_pNode);

        void CreateProjectile();

        virtual const std::string FamilyID() override { return std::string("GOC_CharacterController"); }
        virtual const std::string ComponentID() override { return std::string("GOC_CharacterController"); }
        virtual void Update(float p_fDelta) override;

    private:
        Common::ComponentRigidBody* m_pRigidBody = nullptr;

        ComponentAnimController* m_pAnimComponent = nullptr;

        bool m_bKeysDown[256];
        bool m_bKeysDownLast[256];

        float m_fplayerWalkSpeed = 10.0f;
        float m_fplayerSprintSpeed = 25.0f;

        bool isWalkingAnim = false;
        bool isRunningAnim = false;
        bool isRunning = false;
    };

} // namespace week7

#endif // COMPNENTCHARACTERCONTROLLER_H
