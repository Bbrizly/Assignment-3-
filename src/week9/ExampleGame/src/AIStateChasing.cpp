//------------------------------------------------------------------------
// AIStateChasing
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Idle behaviour state.
//------------------------------------------------------------------------

#include "AIPathfinder.h"
#include "AIStateChasing.h"
#include "ComponentBase.h"
#include "ComponentAIController.h"
#include "ComponentAnimController.h"
#include "GameObject.h"
#include <glm/gtx/vector_angle.hpp>

using namespace week9;

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

		// In two cases we want to recalculate our path:
		//	1. If we don't have a path
		//  2. If our target position is different from the existing path we are following
		if (m_lPath.empty())
		{
			// We don't have a path yet
			m_lPath = AIPathfinder::Instance()->FindPath(transform.GetTranslation(), targetTransform.GetTranslation());
		}
		else
		{
			const AIPathfinder::PathNode* const pEndNode = AIPathfinder::Instance()->GetClosestNode(targetTransform.GetTranslation());
			glm::vec3 vTarget = m_lPath.back();
			if (pEndNode->m_vPosition != vTarget)
			{
				// Our path has changed
				m_lPath = AIPathfinder::Instance()->FindPath(transform.GetTranslation(), targetTransform.GetTranslation());
			}
		}

		// Find the next node we should move towards
		glm::vec3 vMoveTarget = transform.GetTranslation();
		if (m_lPath.size() > 0)
		{
			// Move towards the first node in the path
			AIPathfinder::PositionList::const_iterator it = m_lPath.begin();
			vMoveTarget = static_cast<glm::vec3>(*it);

			// If we're here then choose then next node instead
			if (m_lPath.size() > 1 && glm::length(vMoveTarget - transform.GetTranslation()) < 0.5f)
			{
				m_lPath.pop_front();
				it = m_lPath.begin();
				vMoveTarget = static_cast<glm::vec3>(*it);
			}
		}

		// Move towards our destination
		glm::vec3 vDiff = vMoveTarget - transform.GetTranslation();
		if (glm::length(vDiff) > 0.0f)
		{
			vDiff = glm::normalize(vDiff);
			float fVelocity = 4.0f;
			transform.Translate(vDiff * fVelocity * p_fDelta);

			// Rotate facing direction
			float fAngle = glm::orientedAngle(glm::vec3(0.0f, 0.0f, 1.0f), vDiff, glm::vec3(0.0f, 1.0f, 0.0f));
			transform.SetRotationXYZ(0.0f, fAngle, 0.0f);
		}
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