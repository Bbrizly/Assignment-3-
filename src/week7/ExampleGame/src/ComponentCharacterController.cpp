//------------------------------------------------------------------------
// ComponentCharacterController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a character controller. It polls input and sends
// movement instructions to the relevant sibling components.
//------------------------------------------------------------------------

#include "ComponentCharacterController.h"


#include "iostream"
using namespace std;
using namespace glm;


using namespace week7;

//------------------------------------------------------------------------------
// Method:    ComponentCharacterController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentCharacterController::ComponentCharacterController() : m_pThirdPersonCam(nullptr), m_pLookAtCam(nullptr)
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
	// Delete our camera
	if (m_pLookAtCam)
	{
		delete m_pLookAtCam;
		m_pLookAtCam = NULL;
	}

	if (m_pThirdPersonCam)
	{
		delete m_pThirdPersonCam;
		m_pThirdPersonCam = NULL;
	}

	assert(!m_pThirdPersonCam);
	assert(!m_pLookAtCam);
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

	if (!m_pAnimComponent)
	{
		Common::GameObject* pGameObject = this->GetGameObject();
		if (pGameObject)
		{
			m_pAnimComponent = dynamic_cast<ComponentAnimController*>(pGameObject->GetComponent("GOC_AnimController"));
		}
	}

	for (int i = 0; i < 256; ++i)
	{
		m_bKeysDownLast[i] = m_bKeysDown[i];
		m_bKeysDown[i] = (glfwGetKey(i) == GLFW_PRESS);
	}

	Common::Transform& transform = this->GetGameObject()->GetTransform();

	// camera logicc

	// Toggle cameras
	//static bool bIsFrontView = true;
	//static bool bLastCamKeyDown = false;
	//bool bCamKeyDown = glfwGetKey('C');
	//if (bCamKeyDown && !bLastCamKeyDown)
	//{
	//	bIsFrontView = !bIsFrontView;
	//	Common::SceneManager::Instance()->AttachCamera(bIsFrontView ? m_pThirdPersonCam : m_pLookAtCam);
	//}
	//bLastCamKeyDown = bCamKeyDown;
	//
	//if (Common::SceneManager::Instance()->GetCamera() == m_pLookAtCam)
	//{
	//	//vec3  == === 
	//	m_pLookAtCam->SetTarget(transform.GetTranslation());
	//}
	//else if (Common::SceneManager::Instance()->GetCamera() == m_pThirdPersonCam)
	//{
	//	//cout << "TESTING" << endl;
	//	vec3 offset = vec3(0.0f, 7.0f, 0.0f);
	//	m_pThirdPersonCam->Update(p_fDelta);
	//	m_pThirdPersonCam->SetTarget(transform.GetTranslation() + offset);
	//}

	/////
	/////
	/////

	/////
	Common::SceneCamera* camera = Common::SceneManager::Instance()->GetCamera();
	if (!camera) return;
	mat4 viewMatrix = camera->GetViewMatrix();

	vec3 camForward = normalize(vec3(viewMatrix[2].x, 0.0f, -viewMatrix[2].z));
	vec3 camRight = normalize(vec3(-viewMatrix[0].x, 0.0f, viewMatrix[0].z));

	//cout << "Forward: (" << camForward.x << ", " << camForward.y << ", " << camForward.z << ")\n";
	//cout << "Right: (" << camRight.x << ", " << camRight.y << ", " << camRight.z << ")\n";

	camRight = -camRight;
	// End camera logic

	if (glfwGetKey(GLFW_KEY_LSHIFT))// shift to sprintt
	{
		isRunning = true;
		m_fplayerSpeed = ms_fplayerSprintSpeed;
	}
	else
	{
		isRunning = false;
		m_fplayerSpeed = ms_fplayerWalkSpeed;
	}

	vec3 moveDir = vec3(0.0f);


	if (m_bKeysDown['w'] || m_bKeysDown['W'] ||
		m_bKeysDown['a'] || m_bKeysDown['A'] ||
		m_bKeysDown['s'] || m_bKeysDown['S'] ||
		m_bKeysDown['d'] || m_bKeysDown['D'])
	{
		// Move forward on 'w' key
		if ((m_bKeysDown['w'] || m_bKeysDown['W']))
		{
			moveDir += camForward;
			//moveDir += vec3(0.0f, 0.0f, -1.0f);
		}
		// Move left on 'a' key
		if ((m_bKeysDown['a'] || m_bKeysDown['A']))
		{
			moveDir -= camRight;
			//moveDir += vec3(-1.0f, 0.0f, 0.0f);
		}
		// Move backwards on 's' key
		if ((m_bKeysDown['s'] || m_bKeysDown['S']))
		{
			moveDir -= camForward;
			//moveDir += vec3(0.0f, 0.0f, 1.0f);
		}
		// Move right on 'd' key
		if ((m_bKeysDown['d'] || m_bKeysDown['D']))
		{
			moveDir += camRight;
			//moveDir += vec3(1.0f, 0.0f, 0.0f);
		}

		//cout << "MOVE DIR: x: " << moveDir.x << " y: " << moveDir.y << " z: " << moveDir.z << endl;
		//transform.Translate(normalize(moveDir) * m_fplayerSpeed * p_fDelta);		
	}

	if (length(moveDir) > 0.0f)
	{
		moveDir = normalize(vec3(moveDir.x, 0.0f, moveDir.z));
		transform.Translate(moveDir * m_fplayerSpeed * p_fDelta);

		vec3 up = vec3(0.0f, 1.0f, 0.0f);
		mat4 lookAtMatrix = glm::lookAt(transform.GetTranslation(), transform.GetTranslation() + moveDir, up);
		quat targetRotation = quat_cast(inverse(lookAtMatrix));

		float angle = atan2(moveDir.x, moveDir.z);
		transform.SetRotation(vec3(0.0f, degrees(angle), 0.0f)); 


		if (m_pAnimComponent)
		{
			if (isRunning)
			{
				if (!isRunningAnim)
				{
					isRunningAnim = true;
					isWalkingAnim = false;
					m_pAnimComponent->SetAnim("standard_run");
				}
			}
			else
			{
				if (!isWalkingAnim)
				{
					isWalkingAnim = true;
					isRunningAnim = false;
					m_pAnimComponent->SetAnim("walking");
				}
			}
		}
	}
	else
	{
		if (m_pAnimComponent && (isWalkingAnim || isRunningAnim))
		{
			isWalkingAnim = false;
			isRunningAnim = false;
			m_pAnimComponent->SetAnim("idle");
		}
		else if (!isWalkingAnim && !isRunningAnim) //in the very beginning
		{
			m_pAnimComponent->SetAnim("idle");
		}
	}
}

Common::ComponentBase* ComponentCharacterController::CreateComponent(TiXmlNode* p_pNode)
{
	//<Component type="ComponentCharacterController" />
	/*if (std::strcmp(p_pNode->Value(), "ComponentCharacterController") != 0 &&
		std::strcmp(p_pNode->Value(), "GOC_CharacterController") != 0)
	{
		return nullptr;
	}*/

	assert(strcmp(p_pNode->Value(), "GOC_CharacterController") == 0);


	ComponentCharacterController* pController = new ComponentCharacterController();
	return pController;
}

/* LUA SHIT
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
	ComponentBase* pComponent = this->GetGameObject()->GetComponent("GOC_AnimController");
	if (pComponent)
	{
		ComponentAnimController* pAnimController = static_cast<ComponentAnimController*>(pComponent);
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
	ComponentBase* pComponent = this->GetGameObject()->GetComponent("GOC_AnimController");
	if (pComponent)
	{
		ComponentAnimController* pAnimController = static_cast<ComponentAnimController*>(pComponent);
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
	metaTable.RegisterObjectDirect("DoAction_Walk", (ComponentCharacterController*)0, &ComponentCharacterController::LuaDoAction_Walk);
	metaTable.RegisterObjectDirect("DoAction_Idle", (ComponentCharacterController*)0, &ComponentCharacterController::DoAction_Idle);
	metaTable.RegisterObjectDirect("IsActionComplete", (ComponentCharacterController*)0, &ComponentCharacterController::IsActionComplete);
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
*/

