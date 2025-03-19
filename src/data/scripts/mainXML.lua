-- mainXML.lua

local game = GetGame()
local gameObjectManager = game:GetGameObjectManager()
co = nil

function init()
    print("STARTING")
    
    
    gameObject = gameObjectManager:CreateGameObjectXML("data/scripts/camera.xml") 
    print("CAMERA DONE")

    gameObject = gameObjectManager:CreateGameObjectXML("data/scripts/character.xml")
    print("CHARACTER DONE")
    
    gameObject = gameObjectManager:CreateGameObjectXML("data/scripts/coin_spawner.xml")
    print("COIN SPAWNER XML LOADED")

    gameObject = gameObjectManager:CreateGameObjectXML("data/scripts/hud.xml")
    print("HUD DONE")

    gameObject = gameObjectManager:CreateGameObjectXML("data/scripts/ground.xml")
    gameObject = gameObjectManager:CreateGameObjectXML("data/scripts/lamppost.xml")
    print("ENVIRONMENT DONE")
    return true
end
-------------------------------------------------------------------------------
-- update
-------------------------------------------------------------------------------
function update(deltaTime)
    -- gameObjectManager.update(deltaTime)
	-- coroutine.resume(co)
	return true
end