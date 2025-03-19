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

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentCharacterController::ComponentCharacterController()
	:
	m_bWalking(false)
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

	// 'w' for walk
	if ((m_bKeysDown['w'] || m_bKeysDown['W']))
	{
		if (!m_bWalking)
		{
			if (m_fWalkingDelegate)
			{
				m_fWalkingDelegate(5 /* Number is just to demonstrate using std::function with params */);
			}
		}
		m_bWalking = true;
	}
	else
	{
		if (m_bWalking)
		{
			if (m_fStopWalkingDelegate)
			{
				m_fStopWalkingDelegate(2 /* Number is just to demonstrate using std::function with params */);
			}
		}
		m_bWalking = false;
	}
}
