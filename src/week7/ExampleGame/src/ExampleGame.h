#ifndef WEEK7_EXAMPLEGAME_H
#define WEEK7_EXAMPLEGAME_H

#include "common/Game.h"
#include "common/GameObjectManager.h"
#include "common/SceneManager.h"
#include "common/SceneCamera.h"
#include "ThirdPersonCamera.h"
#include "ComponentLookAtCamera.h"
#include "ComponentThirdPersonCamera.h"
#include "ComponentCameraSwitcher.h"
#include "ComponentRigidBody.h"
#include "HUD.h"
#include "CoinSpawner.h"
#include "PlayerScore.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "StateMachine.h"        // For the optional push/pop states

#ifdef _WIN32
#include "LuaPlus.h"
#include "LuaScriptManager.h"
#endif

namespace week7 {

    enum GameState { GAMEPLAY, PAUSED };

    class ExampleGame : public Common::Game {
    public:
        ExampleGame();
        virtual ~ExampleGame();

    protected:
        virtual bool Init();
        virtual bool Update(float p_fDelta);
        virtual void Render();
        virtual void Shutdown();

#if defined(_WIN32)
        static void ExportToLua();
        static LuaPlus::LuaObject GetGame();
        static void LuaPrint(const char* p_strDebugString);
        LuaPlus::LuaObject GetGameObjectManager();
#endif

    private:
        static ExampleGame* s_pInstance;

        Common::GameObjectManager* m_pGameObjectManager;
        GameState m_state;
        Common::GameObject* m_pCharacter;

        Common::SceneCamera* m_pSceneCamera;

        void CreateWalls();
        void CreateCrateStacks();
        void CreateLamppost();
        void CreateTeeterTotter();
        void CreateProjectile();
        void TogglePause();
        void RenderPauseOverlay();

        float m_timeSinceLastUpdate = 0.0f;

    };

} // namespace week7

#endif // WEEK7_EXAMPLEGAME_H
