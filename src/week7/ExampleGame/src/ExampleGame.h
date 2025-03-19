// ExampleGame.h
// Finalized header for Assignment #3: Physics Playground
// Created: 2025/03/18
// Author: Your Name
//
// This class implements the main game. It creates:
//   • 4 stacks of 9 crates each,
//   • Brick walls enclosing the world,
//   • A ground and lamppost with collision geometry,
//   • Projectile spawning on mouse click in the camera’s look direction,
//   • Collision detection via Bullet (for coins, character, walls, etc.),
//   • A pause state (toggled via key 'P') where physics updates are frozen,
//   • BONUS: A teeter‐totter prop using a hinge constraint.
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

#ifdef _WIN32
#include "LuaPlus.h"
#include "LuaScriptManager.h"
#include "windows.h"
#endif

namespace week7 {

    // Game state enumeration.
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

        Common::GameObjectManager* GetGameObjectManagerCpp() { return m_pGameObjectManager; } //get GameObjectManager LUA
#endif

    private:
        // Static instance for Lua export.
        static ExampleGame* s_pInstance;

        // Managers
        Common::SceneManager* m_pSceneManager;
        Common::GameObjectManager* m_pGameObjectManager;

        // Scene camera (created manually here)
        Common::SceneCamera* m_pSceneCamera;

        // Game state (gameplay or paused)
        GameState m_state;

        // Pointer to the player character GameObject.
        Common::GameObject* m_pCharacter;

        // Helper functions to create scene objects.
        void CreateWalls();
        void CreateCrateStacks();
        void CreateCharacter();
        void CreateProjectile();
        void TogglePause();
        void RenderPauseOverlay();
        void CreateTeeterTotter(); // BONUS: Creates a teeter‐totter prop.
    };

} // namespace week7

#endif // WEEK7_EXAMPLEGAME_H
