//------------------------------------------------------------------------
// AIStateChasing
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Idle behaviour state.
//------------------------------------------------------------------------

#include "AIStateChasing.h"
#include "ComponentBase.h"
#include "ComponentAIController.h"
#include "ComponentAnimController.h"
#include "GameObject.h"
#include <glm/gtx/vector_angle.hpp>

using namespace week8;

//------------------------------------------------------------------------------
// Method:    AIStateChasing
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
AIStateChasing::AIStateChasing()
{
}

//------------------------------------------------------------------------------
// Method:    ~AIStateChasing
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
AIStateChasing::~AIStateChasing()
{
}

//------------------------------------------------------------------------------
// Method:    Enter
// Returns:   void
// 
// Called when this state becomes active.
//------------------------------------------------------------------------------
void AIStateChasing::Enter()
{
	// Trigger the walk animation
	ComponentAIController* pController = static_cast<ComponentAIController*>(m_pStateMachine->GetStateMachineOwner());
	Common::ComponentBase* pComponent = pController->GetGameObject()->GetComponent("GOC_AnimController");
	if (pComponent)
	{
		ComponentAnimController* pAnimController = static_cast<ComponentAnimController*>(pComponent);
		pAnimController->SetAnim("walk");
	}

	// Set our target
	// NOTE: this really should come into the state Enter() method as a context parameter so it's not hard coded.
	m_pTargetGameObject = pController->GetGameObject()->GetManager()->GetGameObject("PlayerCharacter");
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Called each from when this state is active.
//------------------------------------------------------------------------------
void AIStateChasing::Update(float p_fDelta)
{
	if (m_pTargetGameObject)
	{
		ComponentAIController* pController = static_cast<ComponentAIController*>(m_pStateMachine->GetStateMachineOwner());
		Common::Transform& transform = pController->GetGameObject()->GetTransform();
		Common::Transform& targetTransform = m_pTargetGameObject->GetTransform();

		// Move towards our destination
		glm::vec3 vDiff = targetTransform.GetTranslation() - transform.GetTranslation();
		vDiff = glm::normalize(vDiff);
		float fVelocity = 2.0f;
		transform.Translate(vDiff * fVelocity * p_fDelta);

		// Rotate facing direction
		float fAngle = glm::orientedAngle(glm::vec3(0.0f, 0.0f, 1.0f), vDiff, glm::vec3(0.0f, 1.0f, 0.0f));
		transform.SetRotationXYZ(0.0f, fAngle, 0.0f);
	}
	else
	{
		// If we don't have a target, we shouldn't be in a chase state; go to idle
		m_pStateMachine->GoToState(ComponentAIController::eAIState_Idle);
	}
}

//------------------------------------------------------------------------------
// Method:    Exit
// Returns:   void
// 
// Called when this state becomes inactive.
//------------------------------------------------------------------------------
void AIStateChasing::Exit()
{	
	m_pTargetGameObject = NULL;
}