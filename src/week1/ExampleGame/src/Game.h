//------------------------------------------------------------------------
// Game
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// This class serves as a base Game class. Derive from this class and
// override the pure virtuals to setup the Init/Update/Render hooks for
// the main game logic.
//------------------------------------------------------------------------

#ifndef WEEK1_GAME_H
#define WEEK1_GAME_H

namespace week1
{
	class Game
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		Game();
		virtual ~Game();

		int Run(const char* p_strAppName = "week1::Game",
				int p_iWindowWidth = 1280,
				int p_iWindowHeight = 720);

	protected:
		//------------------------------------------------------------------------------
		// Protected methods.
		//------------------------------------------------------------------------------
		virtual bool Init() = 0;
		virtual bool Update() = 0;
		virtual void Render() = 0;
		virtual void Shutdown() = 0;

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
	};

} // namespace week1

#endif // WEEK1_GAME_H
