//------------------------------------------------------------------------
// StateGameplay
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Gameplay state.
//------------------------------------------------------------------------

#ifndef STATEGAMEPLAY_H
#define STATEGAMEPLAY_H

#include "StateBase.h"

namespace week6
{
	class StateGameplay : public Common::StateBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		StateGameplay();
		virtual ~StateGameplay();

		// Overridden from StateBase
		virtual void Enter();
		virtual void Update(float p_fDelta);
		virtual void Exit();
	};
} // namespace week6

#endif

