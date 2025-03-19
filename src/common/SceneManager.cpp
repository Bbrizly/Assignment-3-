//------------------------------------------------------------------------
// SceneManager
//
// Created:	2012/12/23
// Author:	Carel Boers
//	
// A simple scene manager to manage our models and camera.
//------------------------------------------------------------------------

#include "SceneManager.h"
#include "DebugRender.h"
#include <algorithm>

using namespace Common;

// Static singleton instance
SceneManager* SceneManager::s_pSceneManagerInstance = NULL;

//------------------------------------------------------------------------------
// Method:    CreateInstance
// Returns:   void
// 
// Creates the singletone instance.
//------------------------------------------------------------------------------
void SceneManager::CreateInstance()
{
	assert(s_pSceneManagerInstance == NULL);
	s_pSceneManagerInstance = new SceneManager();
}

//------------------------------------------------------------------------------
// Method:    DestroyInstance
// Returns:   void
// 
// Destroys the singleton instance.
//------------------------------------------------------------------------------
void SceneManager::DestroyInstance()
{
	assert(s_pSceneManagerInstance != NULL);
	delete s_pSceneManagerInstance;
	s_pSceneManagerInstance = NULL;
}

//------------------------------------------------------------------------------
// Method:    Instance
// Returns:   SceneManager::SceneManager*
// 
// Access to singleton instance.
//------------------------------------------------------------------------------
SceneManager* SceneManager::Instance()
{
	assert(s_pSceneManagerInstance);
	return s_pSceneManagerInstance;
}

//------------------------------------------------------------------------------
// Method:    SceneManager
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
SceneManager::SceneManager()
	:
	m_pCamera(NULL),
	m_pLight(NULL)
{
	m_pLight = new DirectionalLight();
	m_pLight->m_diffuse = wolf::Color4(1.0f,1.0f,1.0f,1.0f);
	m_pLight->m_ambient = wolf::Color4(0.3f,0.3f,0.3f,1.0f);
	m_pLight->m_specular = wolf::Color4(1.0f,1.0f,1.0f,1.0f);
	m_pLight->m_vDirection = glm::vec3(0.5f,-0.2f,-0.8f);
}

//------------------------------------------------------------------------------
// Method:    ~SceneManager
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
SceneManager::~SceneManager()
{
	if (m_pLight)
	{
		delete m_pLight;
		m_pLight = NULL;
	}
}

void SceneManager::AddTextBox(string text, float x, float y, float width, float height)
{
	if (!m_textRenderer)
	{
		//textrenderer setup
		m_textRenderer = new TextRenderer();
		m_textRenderer->init();
		m_font = m_textRenderer->createFont("Arial"); //"data/Fonts/Arial"

	}
	TextBox* txtBox = m_textRenderer->createTextBox(m_font, text, x, y, width, height);
	txtBox->SetColor(0, 0, 0, 255);
	txtBox->SetAlignment(1);
	txtBox->SetVerticalAlignment(1);
	m_textRenderer->setTextBox(txtBox);
}

TextBox* SceneManager::GetTextBox(int id)
{
	if (m_textRenderer)
	{
		return m_textRenderer->GetTextBoxes()[id];
	}
}

void SceneManager::RemoveTextBox(TextBox* pTextBox)
{
	m_textRenderer->~TextRenderer();
}
void SceneManager::ClearText()
{
	m_textRenderer->~TextRenderer();
}

//------------------------------------------------------------------------------
// Method:    AddModel
// Parameter: wolf::Model * p_pModel
// Returns:   void
// 
// Adds a model the scene manager.
//------------------------------------------------------------------------------
void SceneManager::AddModel(wolf::Model* p_pModel)
{
	m_lModelList.push_back(p_pModel);
}

//------------------------------------------------------------------------------
// Method:    RemoveModel
// Parameter: wolf::Model * p_pModel
// Returns:   void
// 
// Removes a model from the scene manager.
//------------------------------------------------------------------------------
void SceneManager::RemoveModel(wolf::Model* p_pModel)
{
	ModelList::iterator it = std::find(m_lModelList.begin(), m_lModelList.end(), p_pModel);
	if (it != m_lModelList.end())
	{
		m_lModelList.erase(it);
	}	
}

//------------------------------------------------------------------------------
// Method:    Clear
// Returns:   void
// 
// Clears the list of models in the scene manager.
//------------------------------------------------------------------------------
void SceneManager::Clear()
{
	m_lModelList.clear();
}

//------------------------------------------------------------------------------
// Method:    AddSprite
// Parameter: wolf::Sprite * p_pSprite
// Returns:   void
// 
// Adds the given sprite to the scene manager.
//------------------------------------------------------------------------------
void SceneManager::AddSprite(wolf::Sprite* p_pSprite)
{
	m_lSpriteList.push_back(p_pSprite);
}

//------------------------------------------------------------------------------
// Method:    RemoveSprite
// Parameter: wolf::Sprite * p_pSprite
// Returns:   void
// 
// Removes the given sprite from the scene manager.
//------------------------------------------------------------------------------
void SceneManager::RemoveSprite(wolf::Sprite* p_pSprite)
{
	SpriteList::iterator it = std::find(m_lSpriteList.begin(), m_lSpriteList.end(), p_pSprite);
	if (it != m_lSpriteList.end())
	{
		m_lSpriteList.erase(it);
	}
}

//------------------------------------------------------------------------------
// Method:    ClearSprites
// Returns:   void
// 
// Clears the list of sprites in the scene manager.
//------------------------------------------------------------------------------
void SceneManager::ClearSprites()
{
	m_lSpriteList.clear();
}

//------------------------------------------------------------------------------
// Method:    AttachCamera
// Parameter: SceneCamera * p_pCamera
// Returns:   void
// 
// Attaches the given camera to the scene
//------------------------------------------------------------------------------
void SceneManager::AttachCamera(SceneCamera* p_pCamera)
{
	m_pCamera = p_pCamera;
}

//------------------------------------------------------------------------------
// Method:    GetCamera
// Returns:   SceneCamera*
// 
// Returns the active camera.
//------------------------------------------------------------------------------
SceneCamera* SceneManager::GetCamera()
{
	return m_pCamera;
}

void SceneManager::AddPointLight(const glm::vec3& position, const float range, const glm::vec3& color)
{
	m_PointLights.push_back(PointLight(position, range, color));
}

void SceneManager::UpdatePointLight(const int index, const glm::vec3& position)
{
	if (index >= 0 && index < m_PointLights.size())
	{
		m_PointLights[index].position = position;
	}

	//if (!m_PointLights.empty())
	//{
	//	m_PointLights[index].position = position; // Blue light follows the player
	//}
}


//------------------------------------------------------------------------------
// Method:    Render
// Returns:   void
// 
// Iterates the list of models, applies the camera params to the shader and 
// renders the model.
//------------------------------------------------------------------------------
void SceneManager::Render()
{
	// Can't render without a camera
	if (m_pCamera == NULL)
	{
		return;
	}

	// Get the view/proj matrices from the camera
	const glm::mat4& mProj = m_pCamera->GetProjectionMatrix();
	const glm::mat4& mView = m_pCamera->GetViewMatrix();

	// Iterate over the list of models and render them
	ModelList::iterator it = m_lModelList.begin(), end = m_lModelList.end();
	for (; it != end; ++it)
	{
		wolf::Model* pModel = static_cast<wolf::Model*>(*it);

		for (wolf::Material* pMaterial : pModel->GetMaterials())
		{
			//pMaterial->SetUniform("NumPointLights", (int)m_PointLights.size());
			float strength = 10.0f;

			if (m_PointLights.size() >= 2)
			{
				//glm::vec3 blueLightPos = glm::vec3(0.0f, 10.0f, 1.0f)/* get player position */;
				//glm::vec3 blueLightColor = glm::vec3(0.0f, 0.0f, 1.0f);
				pMaterial->SetUniform("blueLight.PositionRange", glm::vec4(m_PointLights[0].position, m_PointLights[0].range));//blueLightPos
				pMaterial->SetUniform("blueLight.Color", m_PointLights[0].color); //blueLightColor
				pMaterial->SetUniform("blueLight.Strength", strength);


				//glm::vec3 yellowLightPos = glm::vec3(0.0f, 5.0f, -10.0f); // Adjust as needed
				//glm::vec3 yellowLightColor = glm::vec3(1.0f, 1.0f, 0.0f);
				pMaterial->SetUniform("yellowLight.PositionRange", glm::vec4(m_PointLights[1].position, m_PointLights[1].range));//yellowLightPos
				pMaterial->SetUniform("yellowLight.Color", m_PointLights[1].color); //yellowLightColor
				pMaterial->SetUniform("yellowLight.Strength", strength);
			}

			/*for (size_t i = 0; i < m_PointLights.size(); ++i)
			{
				std::string index = std::to_string(i);
				pMaterial->SetUniform("PointLights[" + index + "].Position", m_PointLights[i].position);
				pMaterial->SetUniform("PointLights[" + index + "].Color", m_PointLights[i].color);
			}*/

			// Set the light parameters
			//pMaterial->SetUniform("ViewDir", glm::vec3(0.0f, 0.0f, 1.0f));
			//pMaterial->SetUniform("LightAmbient", m_pLight->m_ambient);
			//pMaterial->SetUniform("LightDiffuse", m_pLight->m_diffuse);
			//pMaterial->SetUniform("LightSpecular", m_pLight->m_specular);
			//pMaterial->SetUniform("LightDir", m_pLight->m_vDirection);
		}

		pModel->Render(mView, mProj);
	}


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Render the sprite list with an ortho camera. 
	// TODO: We should really add the camera separately rather than hard code it.

	const glm::mat4 mOrthoProj = glm::ortho(0.0f,1280.0f,720.0f,0.0f,0.0f,1000.0f);
	SpriteList::iterator sIt = m_lSpriteList.begin(), sEnd = m_lSpriteList.end();
	for (; sIt != sEnd; ++sIt)
	{
		wolf::Sprite* pSprite = static_cast<wolf::Sprite*>(*sIt);
		pSprite->Render(mOrthoProj);
	}
	DebugRender::Instance().SetLayerEnabled("BoundingVolumes", true);
	DebugRender::Instance().Render(mProj, mView);
	

	//RENDERING TEXTTT
	if (m_textRenderer)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_textRenderer->render(mOrthoProj, glm::mat4(1.0));
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}
