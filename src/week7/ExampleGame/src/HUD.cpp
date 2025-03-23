#include "HUD.h"
#include <iostream>
#include "EventManager.h"
#include "SceneManager.h"
#include "PlayerScore.h"
#include <GL/glew.h>
#include "glfw/include/GL/glfw.h"

using namespace std;
using namespace Common;

HUD::HUD(PlayerScore* scoreComponent)
    : m_scoreComponent(scoreComponent)
{
    EventManager::Instance().AddListener(EventType::CoinCollected, [this](const Event& e) {
        OnCoinCollected(e);
        });
}

HUD::~HUD()
{
    m_textBox = nullptr;
    m_pauseBtn = nullptr;
}

void HUD::OnCoinCollected(const Event& e)
{
    //ADD MONEY
}

Common::ComponentBase* HUD::CreateComponent(TiXmlNode* pNode)
{
    if (!pNode) return nullptr;
    if (std::strcmp(pNode->Value(), "HUD") != 0 &&
        std::strcmp(pNode->Value(), "GOC_HUD") != 0)
    {
        return nullptr;
    }

    std::string playerName;
    // parse any params
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
                if (!std::strcmp(szName, "playerObjectName"))
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
    if (!m_textBox)
    {
        m_textBox = SceneManager::Instance()->AddTextBox("Score: 0", 0.f, 350.f, 190.f, 50.f);
        if (m_textBox) {
            m_textBox->SetColor(255, 255, 255, 255);
        }

        m_pauseBtn = SceneManager::Instance()->AddTextBox("Pause", m_pauseBtnX, m_pauseBtnY, m_pauseBtnW, m_pauseBtnH);
        if (m_pauseBtn) {
            m_pauseBtn->SetColor(255, 255, 255, 255);
        }
    }

    if (!m_scoreComponent)
    {
        GameObject* pOwner = GetGameObject();
        if (pOwner)
        {
            GameObject* pPlayer = pOwner->GetManager()->GetGameObject(m_playerName);
            if (pPlayer) {
                m_scoreComponent = dynamic_cast<PlayerScore*>(pPlayer->GetComponent("GOC_PlayerScore"));
            }
        }
    }

    if (m_scoreComponent && m_textBox)
    {
        int val = m_scoreComponent->GetScore();
        m_textBox->SetText("Score: " + std::to_string(val));
    }

    // Check if user clicked on "Pause" or "Resume"
    static bool bLastMouseDown = false;
    bool bCurrentMouseDown = (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    if (bCurrentMouseDown && !bLastMouseDown)
    {
        // get the mouse
        int mx, my;
        glfwGetMousePos(&mx, &my);

        std::cout << "Mouse: " << mx << ", " << my <<
            "\nPaBtn: " << m_pauseBtn->getPosX() << ", " << m_pauseBtn->getPosY() <<
            "\Basee: " << m_pauseBtn->getBPosX() << ", " << m_pauseBtn->getBPosY() <<
            "\n";

        // invert if your coordinate system is 0 at top or bottom
        my = 720 - my; // if 0,0 is bottom-left in ortho
        std::cout << "\n\nMouse: " << mx << ", " << my <<
            "\nPaBtn: " << m_pauseBtn->getPosX() << ", " << m_pauseBtn->getPosY() <<
            "\Basee: " << m_pauseBtn->getBPosX() << ", " << m_pauseBtn->getBPosY() <<
            "\n";
        // Check bounds for the pause button
        if (m_pauseBtn)
        {
            float x = m_pauseBtnBoundingX;
            float y = m_pauseBtnBoundingY;

            float w = m_pauseBtn->getWidth();
            float h = m_pauseBtn->getHeight();

            if (mx >= x && mx <= x + w && my >= y && my <= y + h)
            {
                std::cout << "\n\nPASUED??\n\n";
                //// Toggle pause
                
                week7::ExampleGame* theGame = week7::ExampleGame::GetInstance();
                if (theGame)
                {
                    theGame->TogglePause();
                }

                m_bIsPaused = !m_bIsPaused;
                if (m_pauseBtn)
                {
                    m_pauseBtn->SetText(m_bIsPaused ? "Resume" : "Pause");
                }
            }
        }
    }
    bLastMouseDown = bCurrentMouseDown;
}
