#include "HUD.h"
#include <iostream>
#include "EventManager.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <GL/glew.h>
#include "SceneManager.h"
#include "ExampleGame.h"

using namespace std;
using namespace Common;

HUD::HUD(PlayerScore* scoreComponent)
    : m_scoreComponent(scoreComponent)
    , m_textRenderer(nullptr)
    , m_textBox(nullptr)
    , m_pauseBtn(nullptr)
{
    // Listen for coin collisions if you still want
    EventManager::Instance().AddListener(EventType::CoinCollected, [this](const Event& e) {
        OnCoinCollected(e);
        });
}

HUD::~HUD()
{
    m_textRenderer = nullptr;
    m_textBox = nullptr;
    m_pauseBtn = nullptr;
}

void HUD::OnCoinCollected(const Event& e)
{
    // If you were still doing that event
}

Common::ComponentBase* HUD::CreateComponent(TiXmlNode* pNode)
{
    if (std::strcmp(pNode->Value(), "HUD") != 0 &&
        std::strcmp(pNode->Value(), "GOC_HUD") != 0)
    {
        return nullptr;
    }

    std::string playerName;
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
                if (std::strcmp(szName, "playerObjectName") == 0)
                {
                    playerName = szValue;
                }
            }
        }
        pChild = pChild->NextSibling();
    }

    HUD* pHUD = new HUD(nullptr);
    pHUD->setPlayerName(playerName);
    return pHUD;
}

void HUD::Update(float deltaTime)
{
    // We create text boxes once (lazy init)
    if (!m_textRenderer) {
        m_textRenderer = new TextRenderer();
        m_textRenderer->init();

        Font* font = m_textRenderer->createFont("Arial");

        // Score box
        m_textBox = m_textRenderer->createTextBox(font, "Score: 0", 640 - 200, 10, 190, 50);
        m_textBox->SetColor(255, 255, 255, 255);

        // Pause Button box (top-right corner)
        m_pauseBtn = m_textRenderer->createTextBox(font, "Pause", 640 - 100, 60, 90, 40);
        m_pauseBtn->SetColor(255, 255, 255, 255);

        // Maybe a "PAUSED" display if you want 
        // ...
    }

    // If we have a player, get its score
    if (!m_scoreComponent) {
        if (GetGameObject()) {
            GameObject* pOwner = GetGameObject();
            GameObject* pPlayer = pOwner->GetManager()->GetGameObject(playerName);
            if (pPlayer) {
                m_scoreComponent = dynamic_cast<PlayerScore*>(pPlayer->GetComponent("GOC_PlayerScore"));
            }
        }
    }
    if (m_scoreComponent && m_textBox) {
        int scoreVal = m_scoreComponent->GetScore();
        m_textBox->SetText("Score: " + std::to_string(scoreVal));
    }

    // For a clickable approach, check if the user clicked the "Pause" box
    static bool lastMouseDown = false;
    bool curMouseDown = (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    //if (curMouseDown && !lastMouseDown) {
    //    // see if the click is within m_pauseBtn bounds
    //    int mx, my;
    //    glfwGetMousePos(&mx, &my);
    //    // text box is at (540,60) size(90,40) => quick bounding check
    //    if (m_pauseBtn) {
    //        float x = m_pauseBtn->GetX();
    //        float y = m_pauseBtn->GetY();
    //        float w = m_pauseBtn->GetWidth();
    //        float h = m_pauseBtn->GetHeight();
    //        if (mx >= x && mx <= x + w && my >= y && my <= y + h)
    //        {
    //            // toggle
    //            // We can get the ExampleGame instance and call TogglePause
    //            // Or set a flag
    //            ExampleGame* theGame = (ExampleGame*)ExampleGame::s_pInstance;
    //            if (theGame) {
    //                theGame->TogglePause();
    //            }

    //            // Also update label
    //            m_bIsPaused = !m_bIsPaused;
    //            if (m_pauseBtn) {
    //                m_pauseBtn->SetText(m_bIsPaused ? "Resume" : "Pause");
    //            }
    //        }
    //    }
    //}
    lastMouseDown = curMouseDown;
}

void HUD::Render()
{
    if (!m_textRenderer) return;

    // We rely on SceneManager's orthographic projection for text
    glm::mat4 ortho = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, 0.f, 1000.f);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_textRenderer->render(ortho, glm::mat4(1.f));

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
