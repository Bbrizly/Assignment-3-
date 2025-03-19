-------------------------------------------------------------------------------
-- main_example3.lua
--
-- Test script that sets up the scene for this week's sample code and then
-- performs a scripted sequence using coroutines.
-------------------------------------------------------------------------------


local game = GetGame()
local gameObjectManager = game:GetGameObjectManager()
co = nil

-------------------------------------------------------------------------------
-- init
-------------------------------------------------------------------------------
function init()
	-- Create a game object
	gameObject = gameObjectManager:CreateGameObjectXML("week5/ExampleGame/data/character.xml")
	transform = gameObject:GetTransform()
	transform:Scale(0.05, 0.05, 0.05)

	-- Add a component renderable mesh
	componentRenderableMesh = CreateComponentRenderableMesh()
	componentRenderableMesh:Init("data/characters/swat/Swat.pod", "data/characters/swat/", "data/shaders/skinned.vsh", "data/shaders/skinned.fsh")
	gameObject:AddComponent(componentRenderableMesh)

	-- Add the character controller component
	componentCharacterController = CreateComponentCharacterController()
	local temp = componentCharacterController:IsActionComplete()
	gameObject:AddComponent(componentCharacterController)

	-- Perform a scripted sequence
	co = coroutine.create(activity)
	coroutine.resume(co)

	-- Everything initialized OK
	return true
end

-------------------------------------------------------------------------------
-- update
-------------------------------------------------------------------------------
function update(deltaTime)
	coroutine.resume(co)
	return true
end

-------------------------------------------------------------------------------
-- wait
-------------------------------------------------------------------------------
function wait(controller)
	local lController = controller
	while lController:IsActionComplete() == false do
		coroutine.yield()
	end
end

-------------------------------------------------------------------------------
-- activity
-------------------------------------------------------------------------------
function activity()
	local lController = componentCharacterController

	-- Walk to (-5, 0, 0)
	lController:DoAction_Walk(-5.0, 0.0, 0.0)

	-- Wait for previous action
	wait(lController)

	-- Idle for 5 seconds
	lController:DoAction_Idle(5.0)

	-- Wait for previous action
	wait(lController)

	-- Walk to (5, 0, 0)
	lController:DoAction_Walk(5.0, 0.0, 0.0)
end
