//------------------------------------------------------------------------
// AIStateWander
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Wander behaviour state.
//------------------------------------------------------------------------

#include "AIStateWander.h"
#include "ComponentBase.h"
#include "ComponentAIController.h"
#include "ComponentAnimController.h"
#include "GameObject.h"
#include <glm/gtx/vector_angle.hpp>

using namespace week9;

//------------------------------------------------------------------------------
// Method:    AIStateWander
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
AIStateWander::AIStateWander()
{
}

//------------------------------------------------------------------------------
// Method:    ~AIStateWander
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
AIStateWander::~AIStateWander()
{
}

//------------------------------------------------------------------------------
// Method:    Enter
// Returns:   void
// 
// Called when this state becomes active.
//------------------------------------------------------------------------------
void AIStateWander::Enter()
{
	// Trigger the injured walk animation
	ComponentAIController* pController = static_cast<ComponentAIController*>(m_pStateMachine->GetStateMachineOwner());
	Common::ComponentBase* pComponent = pController->GetGameObject()->GetComponent("GOC_AnimController");
	if (pComponent)
	{

		week7::ComponentAnimController* pAnimController = static_cast<week7::ComponentAnimController*>(pComponent);
		pAnimController->SetAnim("walk_injured");
	}

	// - - - - - - -- - 
	// Choose a destination

	const glm::vec3& vPosition = pController->GetGameObject()->GetTransform().GetTranslation();

	// Random offset between (5,0,0) and (10,0,0)
	glm::vec4 vDestOffset = glm::vec4(5.0f + (rand() % 5), 0.0f, 0.0f, 0.0f);

	// Rotate our offset by a random amount
	float fRotation = (float)(rand() % 360);
	glm::mat4 mRotation;
	mRotation = glm::rotate(mRotation, fRotation, glm::vec3(0.0, 1.0, 0.0));
	vDestOffset = mRotation * vDestOffset;

	// Add our rotated offset to our position to get our random destination
	m_vDestination = vPosition + glm::vec3(vDestOffset.x, vDestOffset.y, vDestOffset.z);
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Called each from when this state is active.
//------------------------------------------------------------------------------
void AIStateWander::Update(float p_fDelta)
{
	ComponentAIController* pController = static_cast<ComponentAIController*>(m_pStateMachine->GetStateMachineOwner());
	Common::Transform& transform = pController->GetGameObject()->GetTransform();

	// Check how close we are to our destination
	glm::vec3 vDiff = m_vDestination - transform.GetTranslation();
	if (glm::length(vDiff) < 0.5f)
	{
		// Close enough; we're finished in this state. Go to idle
		m_pStateMachine->GoToState(ComponentAIController::eAIState_Idle);
		return;
	}

	// Move towards our destination
	vDiff = glm::normalize(vDiff);
	float fVelocity = 1.0f;
	transform.Translate(vDiff * fVelocity * p_fDelta);

	// Rotate facing direction
	float fAngle = glm::orientedAngle(glm::vec3(0.0f, 0.0f, 1.0f), vDiff, glm::vec3(0.0f, 1.0f, 0.0f));
	transform.SetRotationXYZ(0.0f, fAngle, 0.0f);
}

//------------------------------------------------------------------------------
// Method:    Exit
// Returns:   void
// 
// Called when this state becomes inactive.
//------------------------------------------------------------------------------
void AIStateWander::Exit()
{	
	// Nothing to do when exiting this state
}