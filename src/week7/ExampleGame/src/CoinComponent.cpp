#include "CoinComponent.h"
#include "iostream"
using namespace glm;
using namespace Common;

void CoinLogic::Update(float deltaTime) {
    m_elapsedTime += deltaTime;
    if (m_elapsedTime >= m_lifespan) {

        Event e;
        e.type = EventType::CoinDisappeared;
        e.sender = GetGameObject();
        EventManager::Instance().TriggerEvent(e);

        GetGameObject()->MarkForDestruction();
    }
}

void CoinMovement::Update(float deltaTime) {
    m_time += deltaTime;
    Common::Transform& t = GetGameObject()->GetTransform();
    vec3 pos = t.GetTranslation();
    
    pos.y += m_amplitude * sin(m_frequency * m_time);
    t.SetTranslation(pos);
    
    t.Rotate(vec3(0.0f, m_rotationSpeed * deltaTime, 0.0f));
}

void CoinCollision::Update(float deltaTime) {
    if (CollidesWithPlayer()) {

        Event e;
        e.type = EventType::CoinCollected;
        e.sender = GetGameObject();

        CoinScore* coinScore = dynamic_cast<CoinScore*>(GetGameObject()->GetComponent("GOC_CoinScore"));
        if (!coinScore) return;
        
        e.extra = reinterpret_cast<void*>(static_cast<intptr_t>(coinScore->GetValue()));

        EventManager::Instance().TriggerEvent(e);

        //g_playerGameObject->GetComponent("GOC_PlayerScore")->Update(deltaTime);
        if(!GetGameObject()->IsMarkedForDestruction())
            GetGameObject()->MarkForDestruction();
    }
}

bool CoinCollision::CollidesWithPlayer() {

    BoundingVolumeComponent* coinBV = dynamic_cast<BoundingVolumeComponent*>(GetGameObject()->GetComponent("GOC_BoundingVolume"));
    if (!coinBV) return false;

    //GET player's vounding bolume
    if (!g_playerGameObject) return false;
    
    BoundingVolumeComponent* playerBV = dynamic_cast<BoundingVolumeComponent*>(g_playerGameObject->GetComponent("GOC_BoundingVolume"));
    if (!playerBV) return false;

    //Check for collision
    return coinBV->GetVolume()->Overlaps(*playerBV->GetVolume());
}
