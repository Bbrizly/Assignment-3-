#ifndef PLAYERSCORE_H
#define PLAYERSCORE_H

#include "ComponentBase.h"
#include <string>
#include "tinyxml.h"

#include "EventManager.h" 
#include <cstdint>           
#include <cstring>           

namespace Common {

    //PLAYER Scroee
    class PlayerScore : public ComponentBase {
    public:
        PlayerScore() : m_score(0)
        {

            EventManager::Instance().AddListener(EventType::CoinCollected, [this](const Event& e) {
                int coinVal = static_cast<int>(reinterpret_cast<intptr_t>(e.extra));
                this->AddScore(coinVal); //CoinCollected
                });

        }

        static Common::ComponentBase* CreateComponent(TiXmlNode* pNode)
        {
            if (std::strcmp(pNode->Value(), "PlayerScore") != 0 &&
                std::strcmp(pNode->Value(), "GOC_PlayerScore") != 0)
            {
                return nullptr;
            }

            PlayerScore* pScore = new PlayerScore();
            return pScore;
        }

        void AddScore(int value) { m_score += value; }
        int GetScore() const { return m_score; }
        virtual void Update(float deltaTime) override {}
        virtual const std::string FamilyID() override { return "GOC_PlayerScore"; }
        virtual const std::string ComponentID() override { return "GOC_PlayerScore"; }
    private:
        int m_score;
    };

} // namespace Common

#endif // PLAYERSCORE_H
