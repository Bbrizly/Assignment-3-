#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <queue>
#include <unordered_map>
#include <vector>
#include <functional>
#include "iostream"

namespace Common {

    enum class EventType {
        CoinSpawned,
        CoinDisappeared,
        GameObjectCollision,
        CoinCollected
    };

    // Generic event structure.
    struct Event {
        EventType type;
        void* sender;   // pointer to the GameObject that sent the event
        void* extra;    // additional data (if needed)
    };

    class EventManager {
    public:
        using Listener = std::function<void(const Event&)>;

        static EventManager& Instance() {
            static EventManager instance;
            return instance;
        }

        void AddListener(EventType type, Listener listener) {
            m_listeners[type].push_back(listener);
        }

        void QueueEvent(const Event& e) {
            m_eventQueue.push(e);
        }

        void TriggerEvent(const Event& e) {
            std::cout << "Event Type: " << static_cast<int>(e.type) << "\n";
            auto it = m_listeners.find(e.type);
            if (it != m_listeners.end()) {
                for (auto& listener : it->second) {
                    listener(e);
                }
            }
        }

        void Update() {
            while (!m_eventQueue.empty()) {
                Event e = m_eventQueue.front();
                m_eventQueue.pop();
                TriggerEvent(e);
            }
        }

    private:
        EventManager() {}
        std::queue<Event> m_eventQueue;
        std::unordered_map<EventType, std::vector<Listener>> m_listeners;
    };

} // namespace Common

#endif // EVENTMANAGER_H
