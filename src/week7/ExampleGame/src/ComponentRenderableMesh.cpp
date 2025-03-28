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
#include "iostream"

using namespace week7;

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

	if (!m_pModel)
	{
		std::cerr << "Failed to load model: " << p_strPath << std::endl;
	}
	else
	{
		std::cout << "Loaded model: " << p_strPath << std::endl;
	}

	if (m_pModel->GetMaterials().empty())
	{
		std::cerr << "Failed to load texture: " << p_strTexturePath << std::endl;
	}
	else
	{
		std::cout << "Loaded texture: " << p_strTexturePath << std::endl;
	}

	std::cout << "Vertex Shader: " << p_strVertexProgramPath << std::endl;
	std::cout << "Fragment Shader: " << p_strFragmentProgramPath << std::endl;

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

Common::ComponentBase* ComponentRenderableMesh::CreateComponent(TiXmlNode* p_pNode)
{
	assert(strcmp(p_pNode->Value(), "GOC_RenderableMesh") == 0);
	//// <Component type="ComponentRenderableMesh"> orr <GOC_RenderableMesh>
	//if (std::strcmp(p_pNode->Value(), "ComponentRenderableMesh") != 0 &&
	//	std::strcmp(p_pNode->Value(), "GOC_RenderableMesh") != 0)
	//{
	//	return nullptr;
	//}

	ComponentRenderableMesh* pMesh = new ComponentRenderableMesh();

	std::string modelFile, modelPath, vertShader, fragShader;

	TiXmlNode* pChildNode = p_pNode->FirstChild();
	while (pChildNode)
	{
		if (std::strcmp(pChildNode->Value(), "Param") == 0)
		{
			TiXmlElement* pElement = pChildNode->ToElement();
			const char* szName = pElement->Attribute("name");
			const char* szValue = pElement->Attribute("value");
			if (szName && szValue)
			{
				if (std::strcmp(szName, "modelFile") == 0)
					modelFile = szValue;
				else if (std::strcmp(szName, "modelPath") == 0)
					modelPath = szValue;
				else if (std::strcmp(szName, "vertexShader") == 0)
					vertShader = szValue;
				else if (std::strcmp(szName, "fragmentShader") == 0)
					fragShader = szValue;
			}
		}
		pChildNode = pChildNode->NextSibling();
	}

	if (!modelFile.empty() && !modelPath.empty() && !vertShader.empty() && !fragShader.empty())
	{
		pMesh->Init(modelFile, modelPath, vertShader, fragShader);
	}

	return pMesh;
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
	metaTable.RegisterObjectDirect("Init", (ComponentRenderableMesh*)0, &ComponentRenderableMesh::LuaInit);
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

