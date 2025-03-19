//------------------------------------------------------------------------
// ExampleGame
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// Main game class. Derives from Common::Game.
//------------------------------------------------------------------------

#ifndef WEEK5_EXAMPLEGAME_H
#define WEEK5_EXAMPLEGAME_H

#include "common/Game.h"
#include "common/GameObjectManager.h"
#include "common/SceneCamera.h"
#include "common/StateMachine.h"

#include <list>

namespace week6
{
	class ExampleGame : public Common::Game
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ExampleGame();
		virtual ~ExampleGame();

		// Static instance
		static ExampleGame* GetInstance() { return s_pInstance; }
		Common::GameObjectManager* GameObjectManager() { return m_pGameObjectManager; }

	protected:	
		//------------------------------------------------------------------------------
		// Protected methods.
		//------------------------------------------------------------------------------
		virtual bool Init();
		virtual bool Update(float p_fDelta);
		virtual void Render();
		virtual void Shutdown();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Static instance for easy export to Lua
		static ExampleGame* s_pInstance;

		// A camera for the scene
		Common::SceneCamera* m_pSceneCamera;
		
		// Game Object Manager
		Common::GameObjectManager* m_pGameObjectManager;

		// State Machine
		Common::StateMachine* m_pStateMachine;
	};

} // namespace week6

#endif // WEEK5_EXAMPLEGAME_H
