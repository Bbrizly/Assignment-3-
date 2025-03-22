// ExampleGame.h
// Finalized header for Assignment #3: Physics Playground
// Created: 2025/03/18
// Author: Your Name
//
// This class implements the main game. It creates:
//   • 4 stacks of 9 crates each,
//   • Brick walls enclosing the world,
//   • A lamppost as an extra rigid body obstacle,
//   • Projectile spawning on mouse click,
//   • Collision detection via Bullet for coins, character, walls, lamppost, crates, ground, etc.,
//   • A pause state (toggled via an on-screen button or by key 'P') where physics are frozen and the scene still draws in the background,
//   • A minimal teeter‐totter bonus prop using a hinge constraint (or anything else interesting).

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

    // A simple enumerated game state if you do not want to push/pop states
    // but the assignment wants you to have a push/pop. Here we keep both ways for illustration.
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
        static ExampleGame* s_pInstance;  // for Lua bridging

        // Managers & scene
        Common::GameObjectManager* m_pGameObjectManager;
        GameState m_state;
        Common::GameObject* m_pCharacter; // pointer to player char

        // Optional: main camera pointer if you want to keep it around
        Common::SceneCamera* m_pSceneCamera;

        // Basic helper functions
        void CreateWalls();
        void CreateCrateStacks();
        void CreateLamppost();                // Extra obstacle
        void CreateTeeterTotter();            // Bonus constraint example
        void CreateProjectile();
        void TogglePause();
        void RenderPauseOverlay();            // "PAUSED" text or background

        // We store time just for minimal illusions
        float m_timeSinceLastUpdate = 0.0f;

        // For demonstration, we do not push/pop states with the StateMachine,
        // but you can do so if you prefer. The assignment wants a push/pop approach
        // for the paused overlay. That is left flexible.

    };

} // namespace week7

#endif // WEEK7_EXAMPLEGAME_H
