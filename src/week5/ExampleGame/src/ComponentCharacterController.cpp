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

using namespace week5;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentCharacterController::ComponentCharacterController()
	:
	m_eActionState(eActionState_None),
	m_fActionTimer(0.0f),
	m_fDuration(0.0f)
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

	// Check action state
	switch(m_eActionState)
	{
		case eActionState_Idle:
		{
			m_fActionTimer += p_fDelta;
			if (m_fActionTimer > m_fDuration)
			{
				m_eActionState = eActionState_None;
			}
			break;
		}
		case eActionState_Walking:
		{
			glm::vec3 vDiff = m_vDestination - transform.GetTranslation();
			float length = glm::length(vDiff);

			if (glm::length(vDiff) < 0.1f) 
			{
				m_eActionState = eActionState_None;
			}
			else
			{
				vDiff = glm::normalize(vDiff);
				vDiff *= p_fDelta;
				transform.Translate(vDiff);
			}
			break;
		}
		case eActionState_None:
		default:
		{
			break;
		}
	}
}


//------------------------------------------------------------------------------
// Method:    DoAction_Walk
// Parameter: const glm::vec3 & p_vDestination
// Returns:   void
// 
// Goes into the walk state and stays there until the destination is reached.
//------------------------------------------------------------------------------
void ComponentCharacterController::DoAction_Walk(const glm::vec3& p_vDestination)
{
	m_eActionState = eActionState_Walking;
	m_vDestination = p_vDestination;
	m_fActionTimer = 0.0f;

	// Play idle animation
	ComponentBase *pComponent = this->GetGameObject()->GetComponent("GOC_AnimController");
	if (pComponent)
	{
		ComponentAnimController *pAnimController = static_cast<ComponentAnimController*>(pComponent);
		pAnimController->SetAnim("walking");
	}
}

//------------------------------------------------------------------------------
// Method:    DoAction_Idle
// Parameter: const float p_fDuration
// Returns:   void
// 
// Goes into the idle state and stays there until the duration is expired.
//------------------------------------------------------------------------------
void ComponentCharacterController::DoAction_Idle(const float p_fDuration)
{
	m_eActionState = eActionState_Idle;
	m_fDuration = p_fDuration;
	m_fActionTimer = 0.0f;

	// Play idle animation
	ComponentBase *pComponent = this->GetGameObject()->GetComponent("GOC_AnimController");
	if (pComponent)
	{
		ComponentAnimController *pAnimController = static_cast<ComponentAnimController*>(pComponent);
		pAnimController->SetAnim("idle");
	}
}

//------------------------------------------------------------------------------
// Method:    IsActionComplete
// Returns:   bool
// 
// Returns true if the last action is complete.
//------------------------------------------------------------------------------
bool ComponentCharacterController::IsActionComplete()
{
	return m_eActionState == eActionState_None;
}

#if defined(_WIN32)
//------------------------------------------------------------------------------
// Method:    ExportToLua
// Returns:   void
// 
// Export interface to Lua.
//------------------------------------------------------------------------------
void ComponentCharacterController::ExportToLua()
{
	LuaPlus::LuaObject metaTable = Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().CreateTable("ComponentCharacterControllerMetaTable");
	metaTable.SetObject("__index", metaTable);

	// Register methods
	Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().RegisterDirect("CreateComponentCharacterController", &ComponentCharacterController::LuaNew);
	metaTable.RegisterObjectDirect("DoAction_Walk", (ComponentCharacterController*) 0, &ComponentCharacterController::LuaDoAction_Walk);
	metaTable.RegisterObjectDirect("DoAction_Idle", (ComponentCharacterController*) 0, &ComponentCharacterController::DoAction_Idle);
	metaTable.RegisterObjectDirect("IsActionComplete", (ComponentCharacterController*) 0, &ComponentCharacterController::IsActionComplete);
}

//------------------------------------------------------------------------------
// Method:    LuaNew
// Returns:   LuaPlus::LuaObject
// 
// Construction of ComponentCharacterController to be exported to Lua.
//------------------------------------------------------------------------------
LuaPlus::LuaObject ComponentCharacterController::LuaNew()
{
	ComponentCharacterController* pComponent = new ComponentCharacterController();
	LuaPlus::LuaObject luaInstance;
	luaInstance.AssignNewTable(Common::LuaScriptManager::Instance()->GetLuaState());

	luaInstance.SetLightUserData("__object", pComponent);

	LuaPlus::LuaObject metaTable = Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().GetByName("ComponentCharacterControllerMetaTable");
	luaInstance.SetMetaTable(metaTable);

	return luaInstance;
}

//------------------------------------------------------------------------------
// Method:    LuaDoAction_Walk
// Parameter: float x
// Parameter: float y
// Parameter: float z
// Returns:   void
// 
// Lua version of the walk action that doesn't require a vector parameter.
//------------------------------------------------------------------------------
void ComponentCharacterController::LuaDoAction_Walk(float x, float y, float z)
{
	this->DoAction_Walk(glm::vec3(x, y, z));
}
#endif
