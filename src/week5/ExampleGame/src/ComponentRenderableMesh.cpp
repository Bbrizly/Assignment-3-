//------------------------------------------------------------------------
// ComponentRenderableMesh
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a component that is renderable.
//------------------------------------------------------------------------

#include "W_Model.h"
#include "ComponentRenderableMesh.h"
#include "GameObject.h"
#include "SceneManager.h"

using namespace week5;

//------------------------------------------------------------------------------
// Method:    ComponentRenderableMesh
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentRenderableMesh::ComponentRenderableMesh()
	:
	m_pModel(NULL)
{
}

//------------------------------------------------------------------------------
// Method:    ~ComponentRenderableMesh
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentRenderableMesh::~ComponentRenderableMesh()
{
	// Remove the model from the scene
	Common::SceneManager::Instance()->RemoveModel(m_pModel);

	// Delete the model
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = NULL;
	}
}

//------------------------------------------------------------------------------
// Method:    Init
// Parameter: const std::string & p_strPath
// Parameter: const std::string & p_strTexturePath
// Parameter: const std::string & p_strVertexProgramPath
// Parameter: const std::string & p_strFragmentProgramPath
// Returns:   void
// 
// Initializes this component by loading the renderable object.
//------------------------------------------------------------------------------
void ComponentRenderableMesh::Init(const std::string& p_strPath, const std::string &p_strTexturePath, const std::string &p_strVertexProgramPath, const std::string &p_strFragmentProgramPath)
{
	m_pModel = new wolf::Model(p_strPath, p_strTexturePath, p_strVertexProgramPath, p_strFragmentProgramPath);
	Common::SceneManager::Instance()->AddModel(m_pModel);
}

//------------------------------------------------------------------------------
// Method:    SyncTransform
// Returns:   void
// 
// TODO: document me.
//------------------------------------------------------------------------------
void ComponentRenderableMesh::SyncTransform()
{
	m_pModel->SetTransform(this->GetGameObject()->GetTransform().GetTransformation());
}

#if defined(_WIN32)
//------------------------------------------------------------------------------
// Method:    ExportToLua
// Returns:   void
// 
// Export interface to Lua.
//------------------------------------------------------------------------------
void ComponentRenderableMesh::ExportToLua()
{
	LuaPlus::LuaObject metaTable = Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().CreateTable("ComponentRenderableMeshMetaTable");
	metaTable.SetObject("__index", metaTable);

	// Register methods
	Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().RegisterDirect("CreateComponentRenderableMesh", &ComponentRenderableMesh::LuaNew);
	metaTable.RegisterObjectDirect("Init", (ComponentRenderableMesh*) 0, &ComponentRenderableMesh::LuaInit);
}

//------------------------------------------------------------------------------
// Method:    LuaNew
// Returns:   LuaPlus::LuaObject
// 
// Construction of ComponentRenderableMesh to be exported to Lua.
//------------------------------------------------------------------------------
LuaPlus::LuaObject ComponentRenderableMesh::LuaNew()
{
	ComponentRenderableMesh* pComponent = new ComponentRenderableMesh();
	LuaPlus::LuaObject luaInstance;
	luaInstance.AssignNewTable(Common::LuaScriptManager::Instance()->GetLuaState());

	luaInstance.SetLightUserData("__object", pComponent);

	LuaPlus::LuaObject metaTable = Common::LuaScriptManager::Instance()->GetLuaState()->GetGlobals().GetByName("ComponentRenderableMeshMetaTable");
	luaInstance.SetMetaTable(metaTable);

	return luaInstance;
}

//------------------------------------------------------------------------------
// Method:    LuaInit
// Parameter: const char * p_strPath
// Parameter: const char * p_strTexturePath
// Parameter: const char * p_strVertexProgramPath
// Parameter: const char * p_strFragmentProgramPath
// Returns:   void
// 
// Initializes this component by loading the renderable object.
//------------------------------------------------------------------------------
void ComponentRenderableMesh::LuaInit(const char* p_strPath, const char* p_strTexturePath, const char* p_strVertexProgramPath, const char* p_strFragmentProgramPath)
{
	this->Init(std::string(p_strPath), std::string(p_strTexturePath), std::string(p_strVertexProgramPath), std::string(p_strFragmentProgramPath));
}
#endif
