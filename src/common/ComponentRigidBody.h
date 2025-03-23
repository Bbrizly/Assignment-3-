//------------------------------------------------------------------------
// ComponentRigidBody
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a rigid body component.
//------------------------------------------------------------------------

#ifndef COMPONENTRIGIDBODY_H
#define COMPONENTRIGIDBODY_H

#include "btBulletDynamicsCommon.h"
#include "ComponentBase.h"
#include "tinyxml.h"

namespace Common
{
	class ComponentRigidBody : public Common::ComponentBase
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		ComponentRigidBody();
		virtual ~ComponentRigidBody();

		virtual const std::string ComponentID(){ return std::string("GOC_RigidBody"); }
		virtual const std::string FamilyID(){ return std::string("GOC_RigidBody"); }
		virtual void Update(float p_fDelta);

		void AddConstraint(btTypedConstraint* constraint)
		{
			m_constraints.push_back(constraint);
		}

		static ComponentBase* CreateComponent(TiXmlNode* pNode);

		btRigidBody* GetRigidBody()
		{
			if (!m_bInitialized)
			{
				this->Update(0.0f);
			}
			return m_pBody;
		}

		//------------------------------------------------------------------------------
		// Public methods for "GOC_RigidBody" family of components
		//------------------------------------------------------------------------------
		virtual void Init(btCollisionShape* p_pCollisionShape, const std::string& p_strMaterial, float p_fMass, const glm::vec3& p_vOffset, bool p_bIsKinematic = false);

		void Scale(const glm::vec3& scale);
		void ScaleXYZ(float sx, float sy, float sz) { Scale(glm::vec3(sx, sy, sz)); }
		#if defined(_WIN32)
			static void ExportToLua();
		#endif

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		btRigidBody* m_pBody;

		btCollisionShape* m_pCollisionShape;

		glm::vec3 m_vOffset;

		bool m_bKinematic;
		bool m_disabledDeactivation = false;

		//initalization shit
		bool m_bInitialized = false;
		std::string m_storedShape;
		glm::vec3 m_storedHalfExtents;
		float m_storedMass;
		std::string m_storedMaterial;
		glm::vec3 m_storedOffset;
		bool m_storedKinematic;


		std::vector<btTypedConstraint*> m_constraints;


	};
}

#endif // COMPONENTRIGIDBODY_H

