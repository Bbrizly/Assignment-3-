//------------------------------------------------------------------------
// ComponentRenderableMesh
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a renderable component that is a mesh.
//------------------------------------------------------------------------

#ifndef COMPONENTRENDERABLEMESH_H
#define COMPONENTRENDERABLEMESH_H

#include "W_Model.h"
#include "ComponentRenderable.h"
#include "tinyxml.h"


#if defined(WIN32)
#include "LuaScriptManager.h"
#endif

namespace week7
{
	class ComponentRenderableMesh : public Common::ComponentRenderable
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentRenderableMesh();
		virtual ~ComponentRenderableMesh();

		virtual const std::string ComponentID(){ return std::string("GOC_RenderableMesh"); }
		virtual void Update(float p_fDelta) {}
	
		//------------------------------------------------------------------------------
		// Public methods for "GOC_Renderable" family of components
		//------------------------------------------------------------------------------
		virtual void Init(const std::string& p_strPath, const std::string &p_strTexturePath, const std::string &p_strVertexProgramPath, const std::string &p_strFragmentProgramPath);
		virtual void SyncTransform();
		wolf::Model* GetModel() { return m_pModel; }

		static Common::ComponentBase* CreateComponent(TiXmlNode* p_pNode);

		// Lua bindings
#if defined(_WIN32)
		static void ExportToLua();
		static LuaPlus::LuaObject LuaNew();
		void LuaInit(const char* p_strPath, const char* p_strTexturePath, const char* p_strVertexProgramPath, const char* p_strFragmentProgramPath);
#endif

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		wolf::Model* m_pModel;
	};
}

#endif // COMPONENTRENDERABLEMESH_H

