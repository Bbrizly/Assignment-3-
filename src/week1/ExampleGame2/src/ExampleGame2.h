//------------------------------------------------------------------------
// ExampleGame2
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// Main game class. Derives from Common::Game.
//------------------------------------------------------------------------

#ifndef EXAMPLEGAME2_H
#define EXAMPLEGAME2_H

#include "Game.h"
#include "SceneCamera.h"
#include "SceneManager.h"

namespace week1
{
	class ExampleGame2 : public week1::Game
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ExampleGame2();
		virtual ~ExampleGame2();

	protected:	
		//------------------------------------------------------------------------------
		// Protected methods.
		//------------------------------------------------------------------------------
		virtual bool Init();
		virtual bool Update();
		virtual void Render();
		virtual void Shutdown();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// A camera for the scene
		SceneCamera* m_pSceneCamera;

		// A model
		wolf::Model* m_pModel;
	};

} // namespace test

#endif // EXAMPLEGAME2_H
