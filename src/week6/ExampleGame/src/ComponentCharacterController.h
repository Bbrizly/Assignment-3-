//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#ifndef COMPNENTCHARACTERCONTROLLER_H
#define COMPNENTCHARACTERCONTROLLER_H

#include "ComponentBase.h"
#include <glm/glm.hpp>

namespace week6
{
	class ComponentCharacterController : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

		enum ActionState
		{
			eActionState_None = 0,
			eActionState_Idle,
			eActionState_Walking
		};

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentCharacterController();
		virtual ~ComponentCharacterController();

		virtual const std::string FamilyID() { return std::string("GOC_CharacterController"); }
		virtual const std::string ComponentID(){ return std::string("GOC_CharacterController"); }
		virtual void Update(float p_fDelta);

		// Control interface
		void DoAction_Walk(const glm::vec3& p_vDestination);
		void DoAction_Idle(const float p_fDuration);
		bool IsActionComplete();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Keys buffer
		bool m_bKeysDown[256];
		bool m_bKeysDownLast[256];

		// Action state
		ActionState m_eActionState;
		float m_fActionTimer;

		// Destination for eActionState_Walk
		glm::vec3 m_vDestination;

		// Duration for eActionState_Idle
		float m_fDuration;
	};
}

#endif // COMPNENTCHARACTERCONTROLLER_H

