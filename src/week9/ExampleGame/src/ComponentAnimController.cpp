//------------------------------------------------------------------------
// ComponentAnimController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements an animation controller. It manages a list of 
// animations and applies them to a ComponentRenderable on the parent
// GameObject (if one exists).
//------------------------------------------------------------------------

#include "W_Model.h"
#include "ComponentAnimController.h"
#include "ComponentRenderableMesh.h"
#include "GameObject.h"

using namespace week9;

//------------------------------------------------------------------------------
// Method:    ComponentAnimController
// Parameter: int p_iAnimSpeed - animation speed (FPS)
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentAnimController::ComponentAnimController(int p_iAnimSpeed)
	:
	m_iAnimSpeed(p_iAnimSpeed),
	m_pCurrentAnim(nullptr),
	m_fAnimFrame(0.0f),
	m_pRenderable(nullptr)
{
}

//------------------------------------------------------------------------------
// Method:    ~ComponentAnimController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentAnimController::~ComponentAnimController()
{
	AnimMap::iterator it = m_animMap.begin(), end = m_animMap.end();
	for (; it != end; ++it)
	{
		m_pCurrentAnim = static_cast<Anim*>(it->second);
		delete m_pCurrentAnim;
	}
	m_animMap.clear();
}

//------------------------------------------------------------------------------
// Method:    CreateComponent
// Parameter: TiXmlNode * p_pNode
// Returns:   Common::ComponentBase*
//
// Factory construction method. Returns an initialized ComponentAnimController
// instance if the passed in XML is valid, NULL otherwise.
//------------------------------------------------------------------------------
Common::ComponentBase* ComponentAnimController::CreateComponent(TiXmlNode* p_pNode)
{
	assert(strcmp(p_pNode->Value(), "GOC_AnimController") == 0);
	ComponentAnimController* pAnimControllerComponent = new ComponentAnimController();

	// Iterate Anim elements in the XML
	TiXmlNode* pChildNode = p_pNode->FirstChild();
	while (pChildNode != NULL)
	{
		const char* szNodeName = pChildNode->Value();
		if (strcmp(szNodeName, "Anim") == 0)
		{
			// Parse attributes
			TiXmlElement* pElement = pChildNode->ToElement();

			// Name
			const char* szName = pElement->Attribute("name");
			if (szName == NULL)
			{
				delete pAnimControllerComponent;
				return NULL;
			}

			// Resource
			const char* szResource = pElement->Attribute("resource");
			if (szResource == NULL)
			{
				delete pAnimControllerComponent;
				return NULL;
			}

			// Start Frame
			int iStartFrame;
			if (pElement->QueryIntAttribute("start", &iStartFrame) != TIXML_SUCCESS)
			{
				delete pAnimControllerComponent;
				return NULL;
			}

			// End Frame
			int iEndFrame;
			if (pElement->QueryIntAttribute("end", &iEndFrame) != TIXML_SUCCESS)
			{
				delete pAnimControllerComponent;
				return NULL;
			}

			// Loop
			bool bLoop;
			if (pElement->QueryBoolAttribute("loop", &bLoop) != TIXML_SUCCESS)
			{
				delete pAnimControllerComponent;
				return NULL;
			}

			// Add the animation
			pAnimControllerComponent->AddAnim(szName, szResource, iStartFrame, iEndFrame, bLoop);
		}

		pChildNode = pChildNode->NextSibling();
	}

	return pAnimControllerComponent;
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Updates the current animation and looks for a sibling component in the parent
// GameObject in order to set the animation frame.
//------------------------------------------------------------------------------
void ComponentAnimController::Update(float p_fDelta)
{
	if (m_pCurrentAnim)
	{
		m_fAnimFrame += (p_fDelta * static_cast<float>(m_iAnimSpeed));
		if(m_fAnimFrame >= static_cast<float>(m_pCurrentAnim->m_iEndFrame))
		{
			if (m_pCurrentAnim->m_bLoop)
			{
				m_fAnimFrame = static_cast<float>(m_pCurrentAnim->m_iStartFrame);
			}
			else
			{
				m_fAnimFrame = 0.0f;
				m_pCurrentAnim = NULL;
			}
		}

		// If the animation is still playing, then set the frame on our sibling component
		if (m_pCurrentAnim)
		{
			if (m_pRenderable == nullptr)
			{
				// See if we're attached to an object yet. We allow initializing the component before we have a GameObject
				// so we need to add all the anims here
				Common::GameObject* pGameObject = this->GetGameObject();
				ComponentBase* pComponent = pGameObject->GetComponent("GOC_Renderable");
				if (pComponent)
				{
					m_pRenderable = static_cast<ComponentRenderableMesh*>(pComponent);
					for (auto &animInfo : m_animMap)
					{
						m_pRenderable->GetModel()->AddAnimation(animInfo.first, animInfo.second->m_strResource);
					}
					m_pRenderable->GetModel()->SetAnim(m_pCurrentAnim->m_strAnimName);
				}
			}

			if (m_pRenderable != nullptr)
			{
				m_pRenderable->GetModel()->SetAnimFrame(m_fAnimFrame);
			}
		}
	}
}

//------------------------------------------------------------------------------
// Method:    AddAnim
// Parameter: const std::string & p_strAnimName
// Parameter: const std::string & p_strResourcePath
// Parameter: int p_iStartFrame
// Parameter: int p_iEndFrame
// Parameter: bool p_bLoop
// Returns:   void
// 
// Adds an animation to the animation controller.
//------------------------------------------------------------------------------
void ComponentAnimController::AddAnim(const std::string &p_strAnimName, const std::string &p_strResourcePath, int p_iStartFrame, int p_iEndFrame, bool p_bLoop)
{
	// Make sure we don't already have an anim with this name
	if (m_animMap.find(p_strAnimName) == m_animMap.end())
	{
		Anim* pAnim = new Anim(p_strAnimName, p_strResourcePath, p_iStartFrame, p_iEndFrame, p_bLoop);
		m_animMap.insert(std::pair<std::string, Anim*>(p_strAnimName, pAnim));
	}
}

//------------------------------------------------------------------------------
// Method:    SetAnim
// Parameter: const std::string & p_strAnimName
// Returns:   void
// 
// Sets the given animation as the current animation.
//------------------------------------------------------------------------------
void ComponentAnimController::SetAnim(const std::string &p_strAnimName)
{
	AnimMap::const_iterator it = m_animMap.find(p_strAnimName);
	if (it != m_animMap.end())
	{
		m_pCurrentAnim = static_cast<Anim*>(it->second);
		m_fAnimFrame = m_pCurrentAnim->m_iStartFrame;

		if (m_pRenderable != nullptr)
		{
			m_pRenderable->GetModel()->SetAnim(p_strAnimName);
		}
	}
}
