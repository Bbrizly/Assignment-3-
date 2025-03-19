//------------------------------------------------------------------------
// ExampleGame
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// Main game class. Derives from Common::Game.
//------------------------------------------------------------------------

#ifndef WEEK2_EXAMPLEGAME_H
#define WEEK2_EXAMPLEGAME_H

#include "common/Game.h"
#include "common/GameObjectManager.h"
#include "common/SceneCamera.h"

#include <list>

namespace week2
{
	class ExampleGame : public Common::Game
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ExampleGame();
		virtual ~ExampleGame();

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

		// A camera for the scene
		Common::SceneCamera* m_pSceneCamera;
		
		// Game Object Manager
		Common::GameObjectManager* m_pGameObjectManager;
	};

} // namespace week2

#endif // WEEK2_EXAMPLEGAME_H
