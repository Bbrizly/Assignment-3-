#ifndef HUD_H
#define HUD_H

#include "ComponentBase.h"
#include "PlayerScore.h"
#include "wolf/TextRendering/TextRenderer.h"
#include "wolf/TextRendering/TextBox.h"
#include "tinyxml.h"
#include "GameObjectManager.h"
#include "EventManager.h"
#include <glm/glm.hpp>
#include <string>

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

        PlayerScore* m_scoreComponent;

        TextRenderer* m_textRenderer;
        TextBox* m_textBox;

        // Extra text box for the "Pause" button
        TextBox* m_pauseBtn = nullptr;

        std::string playerName = "";
        bool m_bIsPaused = false;
    };

} // namespace Common

#endif // HUD_H
