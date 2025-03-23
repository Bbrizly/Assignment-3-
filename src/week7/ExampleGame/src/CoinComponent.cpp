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

void CoinCollision::OnBulletCollision(const Event& e)
{
    GameObject* coinGO = GetGameObject();
    if (!coinGO) return;

    GameObject* A = static_cast<GameObject*>(e.sender);
    GameObject* B = static_cast<GameObject*>(e.extra);
    if (!A || !B) return;

    if (A != coinGO && B != coinGO)
        return; // coin not in this collision

    GameObject* other = (A == coinGO) ? B : A;

    bool isPlayer = (other == g_playerGameObject);
    bool isProjectile = false;

    std::string otherName = other->GetGUID();
    if (otherName.find("Projectile") != std::string::npos) {
        isProjectile = true;
    }

    if (!isPlayer && !isProjectile)
        return;

    // Ok, so coin + (player or projectile). We'll "collect" or destroy coin
    // Possibly also do score logic if it was a player
    CoinScore* cScore = dynamic_cast<CoinScore*>(coinGO->GetComponent("GOC_CoinScore"));

    // If you want to send a "CoinCollected" event with the coin's value:
    Event e2;
    e2.type = EventType::CoinCollected;
    e2.sender = coinGO;
    if (cScore) {
        e2.extra = reinterpret_cast<void*>(static_cast<intptr_t>(cScore->GetValue()));
    }
    EventManager::Instance().TriggerEvent(e2);

    // Mark coin for destruction
    coinGO->MarkForDestruction();

    // Optionally if a player is colliding, we add to the player's score directly
    // if (isPlayer && cScore) {
    //     PlayerScore* pScore = dynamic_cast<PlayerScore*>(other->GetComponent("GOC_PlayerScore"));
    //     if (pScore) {
    //         pScore->AddScore( cScore->GetValue() );
    //     }
    // }
    // Then coin destroyed
}