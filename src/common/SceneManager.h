//------------------------------------------------------------------------
// SceneManager
//
// Created:	2012/12/23
// Author:	Carel Boers
//	
// A simple scene manager to manage our models and camera.
//------------------------------------------------------------------------

#ifndef COMMON_SCENEMANAGER_H
#define COMMON_SCENEMANAGER_H

#include "W_Model.h"
#include "W_Sprite.h"
#include "SceneCamera.h"

#include "wolf/TextRendering/TextRenderer.h"
#include "wolf/TextRendering/TextBox.h"

namespace Common
{
	class SceneManager
	{
		// Typedef for convenience
		typedef std::vector<wolf::Model*> ModelList;
		typedef std::vector<wolf::Sprite*> SpriteList;
	
		// A directional light
		struct DirectionalLight
		{
			glm::vec3		m_vDirection;
			wolf::Color4    m_ambient;
			wolf::Color4	m_diffuse;
			wolf::Color4	m_specular;
			DirectionalLight() : m_diffuse(0,0,0,0), m_specular(0,0,0,0), m_ambient(0,0,0,0) {}    
		};

		struct PointLight
		{
			glm::vec3 position;
			glm::vec3 color;
			float range;

			PointLight(const glm::vec3& pos, float range, const glm::vec3& col)
				: position(pos), range(range), color(col) {}
		};

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		static void CreateInstance();
		static void DestroyInstance();
		static SceneManager* Instance();

		void AddTextBox(string text, float x, float y, float width, float height);
		TextBox* GetTextBox(int id);
		void RemoveTextBox(TextBox* pTextBox);
		void ClearText();

		void AddModel(wolf::Model* p_pModel);
		void RemoveModel(wolf::Model* p_pModel);
		void Clear();

		void AddSprite(wolf::Sprite* p_pSprite);
		void RemoveSprite(wolf::Sprite* p_pSprite);
		void ClearSprites();

		void AttachCamera(SceneCamera* p_pCamera);
		SceneCamera* GetCamera();

		void AddPointLight(const glm::vec3& position, float range, const glm::vec3& color);
		void UpdatePointLight(const int index, const glm::vec3& position);

		DirectionalLight* GetDefaultLight() { return m_pLight; }
		
		ModelList getModels() { return m_lModelList; }

		void Render();

	private:
		//------------------------------------------------------------------------------
		// Private methods.
		//------------------------------------------------------------------------------
		
		// Constructor/Destructor are private because we're a Singleton
		SceneManager();
		virtual ~SceneManager();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Static singleton instance
		static SceneManager* s_pSceneManagerInstance;

		// alist of textboxes to render
		TextRenderer* m_textRenderer = nullptr;
		Font* m_font = nullptr;

		// A list of models to render
		ModelList m_lModelList;

		// A list of sprites to render
		SpriteList m_lSpriteList;

		// A camera to view the scene
		SceneCamera* m_pCamera;
		
		// Default directional light
		DirectionalLight* m_pLight;

		std::vector<PointLight> m_PointLights;
	};

} // namespace common

#endif // COMMON_SCENEMANAGER_H