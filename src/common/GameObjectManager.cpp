//------------------------------------------------------------------------
// GameObjectManager
//
// Created:	2012/12/26
// Author:	Carel Boers
//	
// Manages the collection of Game Objects used by a game.
//------------------------------------------------------------------------

#include "GameObjectManager.h"
#include "ComponentRenderable.h"
#include "iostream"


using namespace Common;

//------------------------------------------------------------------------------
// Method:    GameObjectManager
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
GameObjectManager::GameObjectManager()
{
}

//------------------------------------------------------------------------------
// Method:    ~GameObjectManager
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
GameObjectManager::~GameObjectManager()
{
	assert(m_mGOMap.size() == 0);
}

//------------------------------------------------------------------------------
// Method:    CreateGameObject
// Returns:   GameObject*
// 
// Factory creation method for empty GameObjects.
//------------------------------------------------------------------------------
GameObject* GameObjectManager::CreateGameObject()
{
	GameObject* pGO = new GameObject(this);
	m_mGOMap.insert(std::pair<std::string, GameObject*>(pGO->GetGUID(), pGO));
	return pGO;
}

//------------------------------------------------------------------------------
// Method:    DestroyGameObject
// Parameter: GameObject * p_pGameObject
// Returns:   void
// 
// Destroys the given Game Object and removes it from internal map of all Game
// Objects.
//------------------------------------------------------------------------------
void GameObjectManager::DestroyGameObject(GameObject* p_pGameObject)
{
	GameObject* pGO = NULL;
	GameObjectMap::iterator it = m_mGOMap.find(p_pGameObject->GetGUID());
	if (it != m_mGOMap.end())
	{
		pGO = (GameObject*)it->second;
		delete pGO;
		m_mGOMap.erase(it);
	}
}

//------------------------------------------------------------------------------
// Method:    DestroyAllGameObjects
// Returns:   void
// 
// Clears the internal map of all GameObjects and deletes them.
//------------------------------------------------------------------------------
void GameObjectManager::DestroyAllGameObjects()
{
	GameObject* pGO = NULL;
	GameObjectMap::iterator it = m_mGOMap.begin(), end = m_mGOMap.end();
	for (; it != end; ++it)
	{
		pGO = (GameObject*)it->second;
		delete pGO;
	}
	m_mGOMap.clear();
}

//------------------------------------------------------------------------------
// Method:    GetGameObject
// Parameter: const std::string & p_strGOGUID
// Returns:   GameObject*
// 
// Returns a GameObject mapped to by the given GUID. Returns NULL if no GO is 
// found.
//------------------------------------------------------------------------------
GameObject* GameObjectManager::GetGameObject(const std::string &p_strGOGUID)
{
	GameObject* pGO = NULL;
	GameObjectMap::iterator it = m_mGOMap.find(p_strGOGUID);
	if (it != m_mGOMap.end())
	{
		pGO = (GameObject*)it->second;
	}

	return pGO;
}

//------------------------------------------------------------------------------
// Method:    SetGameObjectGUID
// Parameter: GameObject * p_pGameObject
// Parameter: const std::string &p_strGOGUID
// Returns:   bool
// 
// Changes the name of the given GUID and updates the internal mapping. Returns 
// true for a successful rename, false on naming collision.
//------------------------------------------------------------------------------
bool GameObjectManager::SetGameObjectGUID(GameObject* p_pGameObject, const std::string &p_strGOGUID)
{
	GameObject* pGO = this->GetGameObject(p_strGOGUID);
	if (pGO)
	{
		// GUID already taken
		return false;
	}

	GameObjectMap::iterator it = m_mGOMap.find(p_pGameObject->GetGUID());
	if (it == m_mGOMap.end())
	{
		// No mapping - Game Object wasn't created from this GameObjectManager instance
		return false;
	}

	// Erase the old mapping, update the GUID and add it back
	m_mGOMap.erase(it);
	p_pGameObject->SetGUID(p_strGOGUID);
	m_mGOMap.insert(std::pair<std::string, GameObject*>(p_pGameObject->GetGUID(), p_pGameObject));
	return true;
}

GameObject* GameObjectManager::CreateGameObject(const std::string& p_strGameObject)
{
	// Load the document and return NULL if it fails
	TiXmlDocument doc(p_strGameObject.c_str());
	if (!doc.LoadFile()) {
		return nullptr;
	}

	// Look for <GameObject>
	TiXmlNode* pNode = doc.FirstChild("GameObject");
	if (!pNode) return nullptr;

	GameObject* pGO = new GameObject(this);
	m_mGOMap.insert(std::make_pair(pGO->GetGUID(), pGO));

	// We do TWO passes:
	//  1) parse a possible <Transform>
	//  2) parse other <components>  (So the transform is known earlier or at least done.)
	// Or you can do a single pass that checks node name.

	TiXmlNode* pChildNode = pNode->FirstChild();
	while (pChildNode)
	{
		//PARSING THE TRAnSFORM IN XMLL

		if (std::strcmp(pChildNode->Value(), "Transform") == 0)
		{
			TiXmlElement* pElem = pChildNode->ToElement();

			const char* scaleAttr = pElem->Attribute("scale");
			const char* translationAttr = pElem->Attribute("translation");
			const char* rotationAttr = pElem->Attribute("rotation");

			if (scaleAttr)
			{
				float sx, sy, sz;
				if (3 == sscanf(scaleAttr, "%f %f %f", &sx, &sy, &sz))
				{
					pGO->GetTransform().SetScale(glm::vec3(sx, sy, sz));
				}
			}
			if (translationAttr)
			{
				float tx, ty, tz;
				if (3 == sscanf(translationAttr, "%f %f %f", &tx, &ty, &tz))
				{
					pGO->GetTransform().SetTranslation(glm::vec3(tx, ty, tz));
				}
			}
			if (rotationAttr)
			{
				float rx, ry, rz;
				if (3 == sscanf(rotationAttr, "%f %f %f", &rx, &ry, &rz))
				{
					pGO->GetTransform().SetRotation(glm::vec3(rx, ry, rz));
				}
			}
		}
		else
		{
			const char* szComponentName = pChildNode->Value();
			auto it = m_mComponentFactoryMap.find(szComponentName);
			if (it != m_mComponentFactoryMap.end())
			{
				auto factory = it->second;
				ComponentBase* pComp = factory(pChildNode);
				if (pComp) pGO->AddComponent(pComp);
			}
		}

		pChildNode = pChildNode->NextSibling();
	}

	return pGO;
}


/*
//------------------------------------------------------------------------------
// Method:    CreateGameObject
// Parameter: const std::string & p_strGameObject
// Returns:   GameObject*
// 
// Factory creation method for GameObjects defined by an XML file. Returns NULL
// if the GameObject couldn't be properly constructed.
//------------------------------------------------------------------------------
GameObject* GameObjectManager::CreateGameObject(const std::string& p_strGameObject)
{
	
	// Load the document and return NULL if it fails to parse
	TiXmlDocument doc(p_strGameObject.c_str());
	if (doc.LoadFile() == false)
	{
		return NULL;
	}

	// Look for the root "GameObject" node and return NULL if it's missing
	TiXmlNode* pNode = doc.FirstChild("GameObject");
	if (pNode == NULL)
	{
		return NULL;
	}

	// Create the game object
	GameObject* pGO = new GameObject(this);
	std::cout << "Name: " << pGO->GetGUID() << "\n\n";
	m_mGOMap.insert(std::pair<std::string, GameObject*>(pGO->GetGUID(), pGO));

	// Iterate components in the XML and delegate to factory methods to construct components
	TiXmlNode* pComponentNode = pNode->FirstChild();
	while (pComponentNode != NULL)
	{
		const char* szComponentName = pComponentNode->Value();
		ComponentFactoryMap::iterator it = m_mComponentFactoryMap.find(szComponentName);
		if (it != m_mComponentFactoryMap.end())
		{
			ComponentFactoryMethod factory = it->second;
			ComponentBase* pComponent = factory(pComponentNode);
			if (pComponent != NULL)
			{
				pGO->AddComponent(pComponent);
			}
		}

		pComponentNode = pComponentNode->NextSibling();
	}
		
	return pGO;
}
*/
//------------------------------------------------------------------------------
// Method:    RegisterComponentFactory
// Parameter: const std::string & p_strComponentId
// Parameter: ComponentFactoryMethod p_factoryMethod
// Returns:   void
// 
// Registers a component factory for a given component Id.
//------------------------------------------------------------------------------
void GameObjectManager::RegisterComponentFactory(const std::string& p_strComponentId, ComponentFactoryMethod p_factoryMethod)
{
	ComponentFactoryMap::iterator it = m_mComponentFactoryMap.find(p_strComponentId);
	if (it != m_mComponentFactoryMap.end())
	{
		return; // Already registered
	}

	// Insert it
	m_mComponentFactoryMap.insert(std::pair<std::string, ComponentFactoryMethod>(p_strComponentId, p_factoryMethod));
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Updates all the GameObjects*
//------------------------------------------------------------------------------
void GameObjectManager::Update(float p_fDelta)
{

	/*for (auto x : m_mGOMap)
	{
		std::cout << "\n\n";
		x.second->ShowComponents();
	}*/

	GameObject* pGO = NULL;
	GameObjectMap::iterator it = m_mGOMap.begin(), end = m_mGOMap.end();
	for (; it != end; ++it)
	{
		pGO = (GameObject*)it->second;
		pGO->Update(p_fDelta);
	}

	for (auto it = m_mGOMap.begin(); it != m_mGOMap.end();)
	{
		GameObject* pGO = it->second;
		if (pGO->IsMarkedForDestruction())
		{
			delete pGO; // calls destructor, which calls DeleteAllComponents
			it = m_mGOMap.erase(it);
		}
		else
		{
			++it;
		}
	}

}

//------------------------------------------------------------------------------
// Method:    SyncTransforms
// Returns:   void
// 
// Syncs the GameObject transform to the renderable component (if one 
// exists for the given GameObject).
//------------------------------------------------------------------------------
void GameObjectManager::SyncTransforms()
{
	GameObject* pGO = NULL;
	GameObjectMap::iterator it = m_mGOMap.begin(), end = m_mGOMap.end();
	for (; it != end; ++it)
	{
		pGO = (GameObject*)it->second;
	
		// If this GO has a Renderable component, sync it's transform from the parent GO
		Common::ComponentBase* pComponent = pGO->GetComponent("GOC_Renderable");
		if (pComponent)
		{
			ComponentRenderable* pRenderable = static_cast<ComponentRenderable*>(pComponent);
			pRenderable->SyncTransform();
		}
	}
}

#if defined (_WIN32)

//------------------------------------------------------------------------------
// Method:    ExportToLua
// Returns:   void
// 
// Export interface to Lua.
//------------------------------------------------------------------------------
void GameObjectManager::ExportToLua()
{
	LuaPlus::LuaObject metaTable = LuaScriptManager::Instance()->GetLuaState()->GetGlobals().CreateTable("GameObjectManagerMetaTable");
	metaTable.SetObject("__index", metaTable);

	// Register methods
	LuaScriptManager::Instance()->GetLuaState()->GetGlobals().RegisterDirect("CreateGameObjectManager", &GameObjectManager::LuaNew);
	metaTable.RegisterObjectDirect("CreateGameObject", (GameObjectManager*) 0, &GameObjectManager::LuaCreateGameObject);
	metaTable.RegisterObjectDirect("CreateGameObjectXML", (GameObjectManager*) 0, &GameObjectManager::LuaCreateGameObjectXML);
	metaTable.RegisterObjectDirect("Update", (GameObjectManager*) 0, &GameObjectManager::Update);
	metaTable.RegisterObjectDirect("SyncTransforms", (GameObjectManager*) 0, &GameObjectManager::SyncTransforms);
}

//------------------------------------------------------------------------------
// Method:    LuaNew
// Returns:   LuaPlus::LuaObject
// 
// Cosntructs a GameObjectManager to be used by Lua.
//------------------------------------------------------------------------------
LuaPlus::LuaObject GameObjectManager::LuaNew()
{
	GameObjectManager* pManager = new GameObjectManager();
	LuaPlus::LuaObject luaInstance;
	luaInstance.AssignNewTable(LuaScriptManager::Instance()->GetLuaState());

	luaInstance.SetLightUserData("__object", pManager);

	LuaPlus::LuaObject metaTable = LuaScriptManager::Instance()->GetLuaState()->GetGlobals().GetByName("GameObjectManagerMetaTable");
	luaInstance.SetMetaTable(metaTable);

	return luaInstance;
}

//------------------------------------------------------------------------------
// Method:    LuaCreateGameObject
// Returns:   LuaPlus::LuaObject
// 
// Creates a new GameObject instance for use in Lua.
//------------------------------------------------------------------------------
LuaPlus::LuaObject GameObjectManager::LuaCreateGameObject()
{
	GameObject* pGO = CreateGameObject();
	LuaPlus::LuaObject luaInstance;
	luaInstance.AssignNewTable(LuaScriptManager::Instance()->GetLuaState());

	luaInstance.SetLightUserData("__object", pGO);

	LuaPlus::LuaObject metaTable = LuaScriptManager::Instance()->GetLuaState()->GetGlobals().GetByName("GameObjectMetaTable");
	luaInstance.SetMetaTable(metaTable);

	return luaInstance;
}

//------------------------------------------------------------------------------
// Method:    LuaCreateGameObjectXML
// Parameter: const char * p_strPath
// Returns:   LuaPlus::LuaObject
// 
// Creates a new GameObject instance from a given XML file for use in Lua.
//------------------------------------------------------------------------------
LuaPlus::LuaObject GameObjectManager::LuaCreateGameObjectXML(const char* p_strPath)
{
	std::cout<< "creating: " << p_strPath << "\n";

	GameObject* pGO = CreateGameObject(p_strPath);

	LuaPlus::LuaObject luaInstance;
	luaInstance.AssignNewTable(LuaScriptManager::Instance()->GetLuaState());

	luaInstance.SetLightUserData("__object", pGO);

	LuaPlus::LuaObject metaTable = LuaScriptManager::Instance()->GetLuaState()->GetGlobals().GetByName("GameObjectMetaTable");
	luaInstance.SetMetaTable(metaTable);

	return luaInstance;
}
 
#endif
