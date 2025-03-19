//------------------------------------------------------------------------
// ExampleGame
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// Main game class. Derives from Common::Game.
//------------------------------------------------------------------------

#ifndef week8_EXAMPLEGAME_H
#define week8_EXAMPLEGAME_H

#include "common/Game.h"
#include "common/GameObjectManager.h"
#include "common/SceneCamera.h"

#include <list>

namespace week9
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
		// Private methods
		//------------------------------------------------------------------------------

		void InitSample1Level();
		void InitSample2Level();
		void InitSample3Level();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// A camera for the scene
		Common::SceneCamera* m_pFrontCamera;
		Common::SceneCamera* m_pTopDownCamera;
		
		// Game Object Manager
		Common::GameObjectManager* m_pGameObjectManager;
	};

} // namespace week9

#endif // week8_EXAMPLEGAME_H
