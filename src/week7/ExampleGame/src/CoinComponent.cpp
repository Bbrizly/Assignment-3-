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
    if (deltaTime == 0) return;
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
    bool isProjectile = (other->GetComponent("GOC_Projectile") != nullptr);

    if (!isPlayer && !isProjectile)
        return;

    CoinScore* cScore = dynamic_cast<CoinScore*>(coinGO->GetComponent("GOC_CoinScore"));

    Event e2;
    e2.type = EventType::CoinCollected;
    e2.sender = coinGO;
    if (cScore) {
        e2.extra = reinterpret_cast<void*>(static_cast<intptr_t>(cScore->GetValue()));
    }
    EventManager::Instance().TriggerEvent(e2);

    coinGO->MarkForDestruction();

     if (isPlayer && cScore) {
         PlayerScore* pScore = dynamic_cast<PlayerScore*>(g_playerGameObject->GetComponent("GOC_PlayerScore"));
         if (pScore) {
             pScore->AddScore( cScore->GetValue() );
         }
     }
}