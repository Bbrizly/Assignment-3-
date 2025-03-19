//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#include "W_Model.h"
#include "ComponentAnimController.h"
#include "ComponentCharacterController.h"
#include "ComponentRenderableMesh.h"
#include "GameObject.h"

using namespace week9;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentCharacterController::ComponentCharacterController()
{
	memset(m_bKeysDown, 0, sizeof(bool) * 256);
	memset(m_bKeysDownLast, 0, sizeof(bool) * 256);
}

//------------------------------------------------------------------------------
// Method:    ~ComponentCharacterController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentCharacterController::~ComponentCharacterController()
{
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentCharacterController::Update(float p_fDelta)
{
	// Transfer previous frame key presses to "Last" array and get current keyboard state
	for (int i = 0; i < 256; ++i)
	{
		m_bKeysDownLast[i] = m_bKeysDown[i];
		m_bKeysDown[i] = (glfwGetKey(i) == GLFW_PRESS);
	}

	// Get the parent GameObject's Transform component.
	Common::Transform& transform = this->GetGameObject()->GetTransform();

	// Move forward on 'w' key
	if ((m_bKeysDown['w'] || m_bKeysDown['W']))
	{
		transform.Translate(glm::vec3(0.0f, 0.0f, -10.0f * p_fDelta));
	}
	// Move left on 'a' key
	if ((m_bKeysDown['a'] || m_bKeysDown['A']))
	{
		transform.Translate(glm::vec3(-10.0f * p_fDelta, 0.0f, 0.0f));
	}
	// Move right on 'd' key
	if ((m_bKeysDown['d'] || m_bKeysDown['D']))
	{
		transform.Translate(glm::vec3(10.0f * p_fDelta, 0.0f, 0.0f));
	}
	// Move backward on 's' key
	if ((m_bKeysDown['s'] || m_bKeysDown['S']))
	{
		transform.Translate(glm::vec3(0.0f, 0.0f, 10.0f * p_fDelta));
	}
}
