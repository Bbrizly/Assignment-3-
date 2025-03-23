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
        // We don't have a separate "Render()" now, because SceneManager does it automatically.

        virtual const std::string FamilyID()    override { return "GOC_HUD"; }
        virtual const std::string ComponentID() override { return "GOC_HUD"; }

        void setPlayerName(const std::string& x) { m_playerName = x; }

    private:
        void OnCoinCollected(const struct Event& e);

        PlayerScore* m_scoreComponent = nullptr;

        // We'll store pointers to the text boxes we create in SceneManager
        TextBox* m_textBox = nullptr; // shows "Score: X"
        TextBox* m_pauseBtn = nullptr; // "Pause"/"Resume" button

        bool m_bIsPaused = false;

        // We'll store bounding or position for the button
        float m_pauseBtnX = 10.f;
        float m_pauseBtnY = 60.f;
        float m_pauseBtnW = 90.f;
        float m_pauseBtnH = 40.f;

        // The player's name in case we need to find them
        std::string m_playerName;
    };

} // namespace Common

#endif // HUD_H