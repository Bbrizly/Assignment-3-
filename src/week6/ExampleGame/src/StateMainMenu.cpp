//------------------------------------------------------------------------
// StateMainMenu
//
// Created:	2013/02/10
// Author:	Carel Boers
//	
// Main menu state.
//------------------------------------------------------------------------

#include "ComponentRenderableSprite.h"
#include "ExampleGame.h"
#include "GameObject.h"
#include "StateMainMenu.h"
#include "SceneManager.h"
#include "States.h"

using namespace week6;

//------------------------------------------------------------------------------
// Method:    StateMainMenu
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
StateMainMenu::StateMainMenu()
{
}

//------------------------------------------------------------------------------
// Method:    ~StateMainMenu
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
StateMainMenu::~StateMainMenu()
{
}

//------------------------------------------------------------------------------
// Method:    Enter
// Returns:   void
// 
// Called when this state becomes active.
//------------------------------------------------------------------------------
void StateMainMenu::Enter()
{
	Common::GameObject* pButton  = ExampleGame::GetInstance()->GameObjectManager()->CreateGameObject();
	ComponentRenderableSprite* pSprite = new ComponentRenderableSprite();
	pSprite->Init("week6/ExampleGame/data/ui/ButtonPlayNow.tga", "week6/ExampleGame/data/ui/sprite.vsh", "week6/ExampleGame/data/ui/sprite.fsh", glm::vec2(155.0f, 56.0f));
	pButton->AddComponent(pSprite);
	pButton->GetTransform().SetTranslation(glm::vec3(562.5f, 200.0f, 0.0f));

	pButton = ExampleGame::GetInstance()->GameObjectManager()->CreateGameObject();
	pSprite = new ComponentRenderableSprite();
	pSprite->Init("week6/ExampleGame/data/ui/ButtonLeaderboard.tga", "week6/ExampleGame/data/ui/sprite.vsh", "week6/ExampleGame/data/ui/sprite.fsh", glm::vec2(155.0f, 56.0f));
	pButton->AddComponent(pSprite);
	pButton->GetTransform().SetTranslation(glm::vec3(562.5f, 280.0f, 0.0f));
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Called each from when this state is active.
//------------------------------------------------------------------------------
void StateMainMenu::Update(float p_fDelta)
{
	// NOTE: This is a terrible way to do this. This mouse click checks should be part of the button code.
	int iButton = glfwGetMouseButton(0);
	if (iButton)
	{
		int x, y = 0;
		glfwGetMousePos(&x, &y);

		if (x > 562.5f && x < 717.5f)
		{
			if (y > 200.0f && y < 256.0f)
			{
				// Play now pressed
				m_pStateMachine->GoToState(eStateGameplay_Gameplay);
			}
			else if (y > 280.0f && y > 336.0f)
			{
				// Leaderboard pressed
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
void StateMainMenu::Exit()
{
	ExampleGame::GetInstance()->GameObjectManager()->DestroyAllGameObjects();
}