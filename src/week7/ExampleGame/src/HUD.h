#ifndef HUD_H
#define HUD_H

#include "ComponentBase.h"
#include "PlayerScore.h"

#include "wolf/TextRendering/TextRenderer.h"
#include "wolf/TextRendering/TextBox.h"
#include "tinyxml.h"

#include "ComponentBase.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "CoinComponent.h"
#include "EventManager.h"
#include <cstdlib>
#include <glm/glm.hpp>
#include "tinyxml.h"

namespace Common {

    class HUD : public ComponentBase {
    public:
        HUD(PlayerScore* scoreComponent = nullptr);
        virtual ~HUD();

        static Common::ComponentBase* CreateComponent(TiXmlNode* pNode);

        virtual void Update(float deltaTime) override;

        void Render();

        virtual const std::string FamilyID() override { return "GOC_HUD"; }
        virtual const std::string ComponentID() override { return "GOC_HUD"; }

        void setPlayerName(std::string x) { playerName = x; }

    private:

        void OnCoinCollected(const struct Event& e);

        HUD* thiz()
        {
            return this;
        }

        PlayerScore* m_scoreComponent;

        TextRenderer* m_textRenderer;
        TextBox* m_textBox;
        std::string playerName = "";
    };

} // namespace Common

#endif // HUD_H
