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

#include "ExampleGame.h"

namespace Common {

    class HUD : public ComponentBase {
    public:
        HUD(PlayerScore* scoreComponent = nullptr);
        virtual ~HUD();
        
        static Common::ComponentBase* CreateComponent(TiXmlNode* pNode);
        
        virtual void Update(float deltaTime) override;

        virtual const std::string FamilyID()    override { return "GOC_HUD"; }
        virtual const std::string ComponentID() override { return "GOC_HUD"; }

        void setPlayerName(const std::string& x) { m_playerName = x; }

    private:
        void OnCoinCollected(const struct Event& e);

        PlayerScore* m_scoreComponent = nullptr;

        TextBox* m_textBox = nullptr;
        TextBox* m_pauseBtn = nullptr;

        bool m_bIsPaused = false;

        float m_pauseBtnX = 520.f;
        float m_pauseBtnY = 350.f;

        float m_pauseBtnBoundingX = 1160.f;
        float m_pauseBtnBoundingY = 669.f;

        float m_pauseBtnW = 90.f;
        float m_pauseBtnH = 40.f;

        std::string m_playerName;
    };

} // namespace Common

#endif // HUD_H