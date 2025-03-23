#ifndef COINCOMPONENT_H
#define COINCOMPONENT_H

#include "ComponentBase.h"
#include "GameObject.h"
#include "Transform.h"
#include "EventManager.h"
#include "BoundingVolumeComponent.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include <string>
#include <cmath>
#include "PlayerScore.h"

namespace Common {

    class CoinLogic : public ComponentBase {
    public:
        CoinLogic(float lifespan)
            : m_lifespan(lifespan), m_elapsedTime(0.0f) {}
        virtual void Update(float deltaTime) override;
        virtual const std::string FamilyID() override { return "GOC_CoinLogic"; }
        virtual const std::string ComponentID() override { return "GOC_CoinLogic"; }
    private:
        float m_lifespan;     // seconds
        float m_elapsedTime;
    };

    class CoinMovement : public ComponentBase {
    public:
        CoinMovement(float amplitude, float frequency, float rotationSpeed)
            : m_amplitude(amplitude), m_frequency(frequency), m_rotationSpeed(rotationSpeed), m_time(0.0f) {}
        virtual void Update(float deltaTime) override;
        virtual const std::string FamilyID() override { return "GOC_CoinMovement"; }
        virtual const std::string ComponentID() override { return "GOC_CoinMovement"; }
    private:
        float m_amplitude;     // vertical offset
        float m_frequency;     // how fast it bobs
        float m_rotationSpeed; // degrees per second
        float m_time;
    };

    class CoinScore : public ComponentBase {
    public:
        CoinScore(int value) : m_value(value) {}
        int GetValue() const { return m_value; }
        virtual void Update(float deltaTime) override {}
        virtual const std::string FamilyID() override { return "GOC_CoinScore"; }
        virtual const std::string ComponentID() override { return "GOC_CoinScore"; }
    private:
        int m_value;
    };

    class CoinCollision : public ComponentBase {
    public:
        CoinCollision(GameObject* playerGameObject) : g_playerGameObject(playerGameObject)
        {
            EventManager::Instance().AddListener(EventType::GameObjectCollision,
                [this](const Event& e) { OnBulletCollision(e); });
        }

        virtual void Update(float /*deltaTime*/) override {}
        virtual const std::string FamilyID()    override { return "GOC_CoinCollision"; }
        virtual const std::string ComponentID() override { return "GOC_CoinCollision"; }

    private:
        void OnBulletCollision(const Event& e);
        GameObject* g_playerGameObject = nullptr;
    };


} // namespace Common

#endif // COINCOMPONENT_H
