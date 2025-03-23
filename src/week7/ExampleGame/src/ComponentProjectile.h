// ComponentProjectile.h

#pragma once
#include "ComponentBase.h"
#include "tinyxml.h"

namespace week7
{
    class ComponentProjectile : public Common::ComponentBase
    {
    public:
        ComponentProjectile() {}
        virtual ~ComponentProjectile() {}

        static Common::ComponentBase* CreateComponent(TiXmlNode* pNode)
        {
            // Minimal check
            if (!pNode) return nullptr;
            if (strcmp(pNode->Value(), "GOC_Projectile") != 0) return nullptr;
            return new ComponentProjectile();
        }

        virtual void Update(float /*deltaTime*/) override {}
        virtual const std::string FamilyID()    override { return "GOC_Projectile"; }
        virtual const std::string ComponentID() override { return "GOC_Projectile"; }
    };
}
