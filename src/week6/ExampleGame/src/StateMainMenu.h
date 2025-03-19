//------------------------------------------------------------------------
// StateMainMenu
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Main menu state.
//------------------------------------------------------------------------

#ifndef STATEMAINMENU_H
#define STATEMAINMENU_H

#include "GameObject.h"
#include "StateBase.h"

namespace week6
{
	class StateMainMenu : public Common::StateBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		StateMainMenu();
		virtual ~StateMainMenu();

		// Overridden from StateBase
		virtual void Enter();
		virtual void Update(float p_fDelta);
		virtual void Exit();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

	};
} // namespace week6

#endif

