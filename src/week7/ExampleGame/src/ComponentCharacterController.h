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
#include "SceneManager.h"
#include "SceneCamera.h"
#include "ThirdPersonCamera.h"
#include "W_Model.h"
#include "ComponentAnimController.h"
#include "ComponentRenderableMesh.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "tinyxml.h"


namespace week7
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

		void SetCameras(ThirdPersonCamera* thirdPersonCam, Common::SceneCamera* lookAtCam)
		{
			m_pThirdPersonCam = thirdPersonCam;
			m_pLookAtCam = lookAtCam;
		}

		static Common::ComponentBase* CreateComponent(TiXmlNode* p_pNode);

		// Control interface
		void DoAction_Walk(const glm::vec3& p_vDestination);
		void DoAction_Idle(const float p_fDuration);
		bool IsActionComplete();

#if defined(_WIN32)
		void ExportToLua()
		{
			LuaPlus::LuaObject metaTable = Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().CreateTable("ComponentCharacterControllerMetaTable");
			metaTable.SetObject("__index", metaTable);

			// Register methods
			//Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().RegisterDirect("CreateComponentCharacterController", &ComponentCharacterController::LuaNew);
			//metaTable.RegisterObjectDirect("Init", (ComponentCharacterController*)0, &ComponentCharacterController::LuaInit);
		}

		LuaPlus::LuaObject LuaNew()
		{
			ComponentCharacterController* pComponent = new ComponentCharacterController();
			LuaPlus::LuaObject luaInstance;
			luaInstance.AssignNewTable(Common::LuaScriptManager::Instance()->GetLuaState());

			luaInstance.SetLightUserData("__object", pComponent);

			LuaPlus::LuaObject metaTable = Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().GetByName("ComponentCharacterControllerMetaTable");
			luaInstance.SetMetaTable(metaTable);

			return luaInstance;
		}

		void LuaDoAction_Walk(float x, float y, float z);
#endif

		virtual const std::string FamilyID() { return std::string("GOC_CharacterController"); }
		virtual const std::string ComponentID(){ return std::string("GOC_CharacterController"); }
		virtual void Update(float p_fDelta);

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		ThirdPersonCamera* m_pThirdPersonCam;
		Common::SceneCamera* m_pLookAtCam;

		// Keys buffer
		bool m_bKeysDown[256];
		bool m_bKeysDownLast[256];
		bool isWalkingAnim = false;
		bool isRunning = false;
		bool isRunningAnim = false;

		float m_fplayerSpeed = 0.0f;

		float ms_fplayerWalkSpeed = 10.0f;
		float ms_fplayerSprintSpeed = 30.0f;

		//ComponentBase* animComponent;// = pGameObject->GetComponent("GOC_AnimController");

		ComponentAnimController* m_pAnimComponent = nullptr;
	};
}

#endif // COMPNENTCHARACTERCONTROLLER_H

