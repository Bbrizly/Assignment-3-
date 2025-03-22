//------------------------------------------------------------------------
// ComponentRigidBody
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements a rigid body component.
//--------------------------------------------------------------------

#include "BulletPhysicsManager.h"
#include "ComponentRigidBody.h"
#include "GameObject.h"

using namespace Common;

//------------------------------------------------------------------------------
// Method:    ComponentRigidBody
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentRigidBody::ComponentRigidBody()
	:
	m_pBody(NULL),
	m_pCollisionShape(NULL),
	m_bKinematic(false)
{
}

//------------------------------------------------------------------------------
// Method:    ~ComponentRigidBody
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentRigidBody::~ComponentRigidBody()
{
}

//------------------------------------------------------------------------------
// Method:    Init
// Parameter: btCollisionShape * p_pCollisionShape
// Parameter: const std::string & p_strMaterial
// Parameter: float p_fMass
// Parameter: bool p_bIsKinematic
// Returns:   void
// 
// Initializes the rigid body component.
//------------------------------------------------------------------------------
void ComponentRigidBody::Init(btCollisionShape* p_pCollisionShape, const std::string& p_strMaterial, float p_fMass, const glm::vec3& p_vOffset, bool p_bIsKinematic)
{
	m_pCollisionShape = p_pCollisionShape;
	m_bKinematic = p_bIsKinematic;
	m_vOffset = p_vOffset;

	// Set mass
	btScalar mass(p_fMass);
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0,0,0);
	if (isDynamic)
	{
		m_pCollisionShape->calculateLocalInertia(mass, localInertia);
	}

	// Set initial transform
	Transform& transform = this->GetGameObject()->GetTransform();
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(transform.GetTranslation().x, transform.GetTranslation().y, transform.GetTranslation().z));

	// Setup the motion state
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, m_pCollisionShape, localInertia);

	// Material specific properties
	BulletPhysicsMaterialManager::PhysicsMaterial* pMaterial = BulletPhysicsManager::Instance()->GetMaterial(p_strMaterial);
	if (pMaterial)
	{
		rbInfo.m_restitution = pMaterial->restitution;
		rbInfo.m_friction = pMaterial->friction;
	}

	m_pBody = new btRigidBody(rbInfo);
	m_pBody->setUserPointer(this->GetGameObject());

	// Set kinematic flags if this rigid body is manually positioned
	if (m_bKinematic)
	{
		m_pBody->setCollisionFlags( m_pBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		m_pBody->setActivationState(DISABLE_DEACTIVATION);
	}

	// Add rigid body to the world
	BulletPhysicsManager::Instance()->GetWorld()->addRigidBody(m_pBody);
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Update rigid body component.
//------------------------------------------------------------------------------
void ComponentRigidBody::Update(float p_fDelta)
{
    if (!m_bInitialized)
    {
        btCollisionShape* shape = nullptr;
        if (m_storedShape == "box")
        {
            shape = new btBoxShape(btVector3(m_storedHalfExtents.x,
                m_storedHalfExtents.y,
                m_storedHalfExtents.z));
        }
        else if (m_storedShape == "sphere")
        {
            shape = new btSphereShape(m_storedHalfExtents.x);
        }
        else
        {
            shape = new btBoxShape(btVector3(m_storedHalfExtents.x,
                m_storedHalfExtents.y,
                m_storedHalfExtents.z));
        }
        this->Init(shape, m_storedMaterial, m_storedMass, m_storedOffset, m_storedKinematic);
        m_bInitialized = true;
    }


	if (m_bKinematic)
	{
		// Get the rigid body transform
		btTransform trans;
		m_pBody->getMotionState()->getWorldTransform(trans);

		// Apply to the rigid body
		const glm::vec3& vPosition = this->GetGameObject()->GetTransform().GetTranslation();
		const glm::quat& qRotation = this->GetGameObject()->GetTransform().GetRotation();
		trans.setOrigin(btVector3(vPosition.x, vPosition.y, vPosition.z) - btVector3(m_vOffset.x, m_vOffset.y, m_vOffset.z));
		trans.setRotation(btQuaternion(qRotation.x, qRotation.y, qRotation.z, qRotation.w));
		m_pBody->getMotionState()->setWorldTransform(trans);
	}
	else
	{
		// Get the rigid body transform
		btTransform trans;
		m_pBody->getMotionState()->getWorldTransform(trans);

		// Rotation and translation
		glm::quat qRot = glm::quat(trans.getRotation().getW(), trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ());
		glm::vec3 vPos = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
		glm::vec3 vOffset = glm::mat3_cast(qRot) * m_vOffset;

		// Apply to the game object
		Transform& transform = this->GetGameObject()->GetTransform();
		transform.SetTranslation(vPos + vOffset);
		transform.SetRotation(qRot);
	}
}

ComponentBase* ComponentRigidBody::CreateComponent(TiXmlNode* pNode)
{
    ComponentRigidBody* rB = new ComponentRigidBody();

    std::string shapeType = "box";
    glm::vec3 halfExtents(1.5f, 1.5f, 1.5f);
    float mass = 2.0f;
    std::string material = "Crate";
    glm::vec3 offset(0.0f);
    bool kinematic = false;

    TiXmlNode* pChild = pNode->FirstChild();
    while (pChild)
    {
        if (std::strcmp(pChild->Value(), "Param") == 0)
        {
            TiXmlElement* pElem = pChild->ToElement();
            const char* paramName = pElem->Attribute("name");
            const char* paramValue = pElem->Attribute("value");
            if (paramName && paramValue)
            {
                if (std::strcmp(paramName, "shape") == 0)
                {
                    shapeType = paramValue;
                }
                else if (std::strcmp(paramName, "halfExtents") == 0)
                {
                    float x, y, z;
                    if (sscanf(paramValue, "%f %f %f", &x, &y, &z) == 3)
                    {
                        halfExtents = glm::vec3(x, y, z);
                    }
                }
                else if (std::strcmp(paramName, "radius") == 0)
                {
                    float r;
                    if (sscanf(paramValue, "%f", &r) == 1)
                    {
                        halfExtents.x = r;
                    }
                }
                else if (std::strcmp(paramName, "mass") == 0)
                {
                    mass = static_cast<float>(atof(paramValue));
                }
                else if (std::strcmp(paramName, "material") == 0)
                {
                    material = paramValue;
                }
                else if (std::strcmp(paramName, "offset") == 0)
                {
                    float x, y, z;
                    if (sscanf(paramValue, "%f %f %f", &x, &y, &z) == 3)
                    {
                        offset = glm::vec3(x, y, z);
                    }
                }
                else if (std::strcmp(paramName, "kinematic") == 0)
                {
                    kinematic = (std::strcmp(paramValue, "true") == 0);
                }
            }
        }
        pChild = pChild->NextSibling();
    }

    rB->m_storedShape = shapeType;
    rB->m_storedHalfExtents = halfExtents;
    rB->m_storedMass = mass;
    rB->m_storedMaterial = material;
    rB->m_storedOffset = offset;
    rB->m_storedKinematic = kinematic;
    rB->m_bInitialized = false;

    return rB;
}