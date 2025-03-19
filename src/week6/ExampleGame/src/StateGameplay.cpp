//------------------------------------------------------------------------
// StateGameplay
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Gameplay state.
//------------------------------------------------------------------------

#include "ComponentAnimController.h"
#include "ComponentCharacterController.h"
#include "ComponentRenderableMesh.h"
#include "ComponentRenderableSprite.h"
#include "ExampleGame.h"
#include "StateGameplay.h"
#include "States.h"

using namespace week6;

//------------------------------------------------------------------------------
// Method:    StateGameplay
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
StateGameplay::StateGameplay()
{
}

//------------------------------------------------------------------------------
// Method:    ~StateGameplay
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
StateGameplay::~StateGameplay()
{
}

//------------------------------------------------------------------------------
// Method:    Enter
// Returns:   void
// 
// Called when this state becomes active.
//------------------------------------------------------------------------------
void StateGameplay::Enter()
{
	// Create an initialized GameObject
	Common::GameObject* pCharacter = ExampleGame::GetInstance()->GameObjectManager()->CreateGameObject("week6/ExampleGame/data/character.xml");
	pCharacter->GetTransform().Scale(glm::vec3(0.05f, 0.05, 0.05f));

	Common::ComponentBase* pComponent = pCharacter->GetComponent("GOC_AnimController");
	if (pComponent)
	{
		ComponentAnimController* pAnimController = static_cast<ComponentAnimController*>(pComponent);
		pAnimController->SetAnim("idle");
	}

	// Create a renderable component for it
	ComponentRenderableMesh* pRenderableComponent = new ComponentRenderableMesh();
	pRenderableComponent->Init("data/characters/swat/Swat.pod", "data/characters/swat/", "data/shaders/skinned.vsh", "data/shaders/skinned.fsh");
	pCharacter->AddComponent(pRenderableComponent);

	// Create a controller component for it
	ComponentCharacterController* pCharacterControllerComponent = new ComponentCharacterController();
	pCharacter->AddComponent(pCharacterControllerComponent);

	// Main Menu button
	Common::GameObject* pButton  = ExampleGame::GetInstance()->GameObjectManager()->CreateGameObject();
	ComponentRenderableSprite* pSprite = new ComponentRenderableSprite();
	pSprite->Init("week6/ExampleGame/data/ui/ButtonMainMenu.tga", "week6/ExampleGame/data/ui/sprite.vsh", "week6/ExampleGame/data/ui/sprite.fsh", glm::vec2(155.0f, 56.0f));
	pButton->AddComponent(pSprite);
	pButton->GetTransform().SetTranslation(glm::vec3(1125.0f, 0.0f, 0.0f));
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Called each from when this state is active.
//------------------------------------------------------------------------------
void StateGameplay::Update(float p_fDelta)
{
	// NOTE: This is a terrible way to do this. This mouse click checks should be part of the button code.
	int iButton = glfwGetMouseButton(0);
	if (iButton)
	{
		int x, y = 0;
		glfwGetMousePos(&x, &y);

		if (x > 1125.0f && x < 1280.0f)
		{
			if (y > 0.0f && y < 56.0f)
			{
				// Main menu pressed
				m_pStateMachine->GoToState(eStateGameplay_MainMenu);
			}
		}
	}
}

//------------------------------------------------------------------------------
// Method:    Exit
// Returns:   void
// 
// Called when this state becomes inactive.
//------------------------------------------------------------------------------
void StateGameplay::Exit()
{	
	ExampleGame::GetInstance()->GameObjectManager()->DestroyAllGameObjects();
}