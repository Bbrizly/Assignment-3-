#include "CoinSpawner.h"
//#include "../common/ComponentRenderable.h"
#include "ExampleGame.h"
#include "ComponentRenderableMesh.h"
#include "iostream"

using namespace std; using namespace glm; using namespace week7; using namespace Common;

int CoinSpawner::CountActiveCoins() {
    //count coints in gameobject manager
    return 0;
}

void CoinSpawner::SpawnCoin() {
    cout << "COIN SPAWNED\n";

    Common::GameObjectManager* g_gameObjectManager = GetGameObject()->GetManager();

    GameObject* coin = g_gameObjectManager->CreateGameObject();
    //m_playerName = "GameObject_2";
    SetPlayer(g_gameObjectManager->GetGameObject(m_playerName));



    float x = static_cast<float>((rand() % 100) - 50);
    float z = static_cast<float>((rand() % 100) - 50);

    float lifespan = 10.0f;       // seconds
    float amplitude = 0.1f;       // bob amplitude
    float frequency = 2.0f;       // bob frequency
    float rotationSpeed = 45.0f;  // degrees per second

    coin->AddComponent(new CoinLogic(lifespan));
    coin->AddComponent(new CoinMovement(amplitude, frequency, rotationSpeed));
    //coin->AddComponent(new CoinCollision(g_playerGameObject));
    coin->AddComponent(new CoinScore(coinValue));

    ComponentRenderableMesh* pCoinMesh = new ComponentRenderableMesh();
    pCoinMesh->Init("data/As2/props/coin.pod",
                    "data/As2/props/",
                    "data/As1/shaders/point_light.vsh",
                    "data/As1/shaders/point_light.fsh");

    coin->AddComponent(pCoinMesh);

    //COIN BOUNDING
    SphereVolume* coinSphere = new SphereVolume(glm::vec3(0.0f), 5.0f);
    coin->AddComponent(new BoundingVolumeComponent(coinSphere));

    coin->GetTransform().SetTranslation(vec3(x, 0.0f, z));

    Event e;
    e.type = EventType::CoinSpawned;
    e.sender = coin;
    EventManager::Instance().TriggerEvent(e);
}

void CoinSpawner::Update(float deltaTime) {
    m_timeSinceLastSpawn += deltaTime;
    if (m_timeSinceLastSpawn >= m_spawnInterval && CountActiveCoins() < m_maxCoins) {
        SpawnCoin();
        m_timeSinceLastSpawn = 0.0f;
    }
}

Common::ComponentBase* CoinSpawner::CreateComponent(TiXmlNode* pNode)
{
    if (std::strcmp(pNode->Value(), "CoinSpawner") != 0 &&
        std::strcmp(pNode->Value(), "GOC_CoinSpawner") != 0)
    {
        return nullptr;
    }

    float spawnInterval = 5.0f;
    int maxCoins = 10;
    int coinValue = 1;
    std::string m_playerName = "";

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
                if (std::strcmp(szName, "spawnInterval") == 0)
                {
                    spawnInterval = float(atof(szValue));
                }
                else if (std::strcmp(szName, "maxCoins") == 0)
                {
                    maxCoins = atoi(szValue);
                }
                else if (std::strcmp(szName, "coinValue") == 0)
                {
                    coinValue = atoi(szValue);
                }
                else if (strcmp(szName, "playerObjectName") == 0)
                {
                    m_playerName = szValue;
                }
            }
        }
        pChild = pChild->NextSibling();
    }

    CoinSpawner* pSpawner = new CoinSpawner(spawnInterval, maxCoins, coinValue);
    pSpawner->SetPlayerString(m_playerName);
    return pSpawner;

}

