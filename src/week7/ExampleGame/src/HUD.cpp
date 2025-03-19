#include "HUD.h"
#include <iostream>
#include "EventManager.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <GL/glew.h>


#include "SceneManager.h"

using namespace std;
using namespace Common;

HUD::HUD(PlayerScore* scoreComponent)
    : m_scoreComponent(scoreComponent)
    , m_textRenderer(nullptr)
    , m_textBox(nullptr)
{

    //SceneManager::Instance()->AddTextBox(
    //    "Score: 0",
    //    120.0f - 200,   360.0f,        //max 640
    //    200,             60);


    EventManager::Instance().AddListener(EventType::CoinCollected, [this](const Event& e) {
        this->OnCoinCollected(e); //CoinCollected

        });

}

HUD::~HUD()
{
    //delete m_textRenderer;
    m_textRenderer = nullptr;
    m_textBox = nullptr;
}

//If coin collected update HUD
void HUD::OnCoinCollected(const Event& e)
{   
    if (!m_scoreComponent) {

        GameObject* pOwner = GetGameObject();
        if (pOwner) {
            GameObject* pPlayerGO = pOwner->GetManager()->GetGameObject(playerName);
            if (pPlayerGO) {
                m_scoreComponent = static_cast<PlayerScore*>(pPlayerGO->GetComponent("GOC_PlayerScore"));
            }
        }
    }

    if (m_scoreComponent) {

        TextBox* txtBox = SceneManager::Instance()->GetTextBox(0);

        int scoreValue = m_scoreComponent->GetScore();
        txtBox->SetText("Score: " + std::to_string(scoreValue));
    }
}

void HUD::Update(float deltaTime)
{
    if (m_textBox && m_scoreComponent)
    {
        TextBox* txtBox = SceneManager::Instance()->GetTextBox(0);

        int scoreValue = m_scoreComponent->GetScore();
        txtBox->SetText("Score: " + std::to_string(scoreValue));
    }
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