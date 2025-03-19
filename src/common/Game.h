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

#ifndef COMMON_GAME_H
#define COMMON_GAME_H

namespace Common
{
	class Game
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		Game();
		virtual ~Game();

		int Run(const char* p_strAppName = "Common::Game",
				int p_iWindowWidth = 1280,
				int p_iWindowHeight = 720);

	protected:
		//------------------------------------------------------------------------------
		// Protected methods.
		//------------------------------------------------------------------------------
		virtual bool Init() = 0;
		virtual bool Update(float p_fDelta) = 0;
		virtual void Render() = 0;
		virtual void Shutdown() = 0;

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------
		float m_fFrameTime;
	};

} // namespace Common

#endif // COMMON_GAME_H
