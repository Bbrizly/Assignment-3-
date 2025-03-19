#ifndef COINSPAWNER_H
#define COINSPAWNER_H

#include "ComponentBase.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "CoinComponent.h"
#include "EventManager.h"
#include <cstdlib>
#include <glm/glm.hpp>
#include "tinyxml.h"

namespace Common {

    class CoinSpawner : public ComponentBase {
    public:
        CoinSpawner(float spawnInterval, int maxCoins, int coinValue)
            : m_spawnInterval(spawnInterval), m_maxCoins(maxCoins), m_timeSinceLastSpawn(0.0f), coinValue(coinValue) {}
        virtual void Update(float deltaTime) override;

        static Common::ComponentBase* CreateComponent(TiXmlNode* pNode);

        void SetSpawnInterval(float interval)
            {m_spawnInterval = interval;    }

        void SetPlayer(GameObject* player) { g_playerGameObject = player; }
        void SetPlayerString(std::string in) { m_playerName = in; }

        virtual const std::string FamilyID() override { return "GOC_CoinSpawner"; }
        virtual const std::string ComponentID() override { return "GOC_CoinSpawner"; }
    private:

        GameObject* g_playerGameObject = nullptr;

        float m_spawnInterval;
        int m_maxCoins;
        float m_timeSinceLastSpawn;
        int coinValue;// = 100;
        int CountActiveCoins();
        void SpawnCoin();
        std::string m_playerName;

    };

} // namespace Common

#endif // COINSPAWNER_H
