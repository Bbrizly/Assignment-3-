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

    -- spawnCrateStacks()
    spawnWalls()
    
    -- gameObject = gameObjectManager:CreateGameObjectXML("data/scripts/crate.xml")
	-- transform = gameObject:GetTransform()
    -- transform:Translate(0, 10, 0)

    -- gameObject = gameObjectManager:CreateGameObjectXML("data/scripts/wall.xml")

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

function spawnCrate(x, y, z)
    local crate = gameObjectManager:CreateGameObjectXML("data/scripts/crate.xml")
    local transform = crate:GetTransform()
    transform:Translate(x, y, z)
    return crate
end

function spawnCrateStacks()
    local stackPositions = {
        { -10, 0, -10 },
        { 10, 0, -10 },
        { -10, 0, 10 },
        { 10, 0, 10 }
    }
    local cratesPerStack = 9
    local crateHeight = 2.0
    for i, pos in ipairs(stackPositions) do
        for j = 0, cratesPerStack - 1 do
            spawnCrate(pos[1], pos[2] + j * crateHeight, pos[3])
        end
    end
end

function spawnWall(posStr, scaleStr)
    local wall = gameObjectManager:CreateGameObjectXML("data/scripts/wall.xml")
    local transform = wall:GetTransform()
    
    transform:Translate(posStr[1], posStr[2], posStr[3])
    transform:Scale(scaleStr[1], scaleStr[2], scaleStr[3])

    -- local rb = wall:GetComponent("GOC_RigidBody")
    -- if rb and rb.UpdateCollisionShape then
        -- rb:UpdateCollisionShape(scaleTable)
    -- end

    return wall
end

function spawnWalls()
    local wallData = {
        { {0, 5, -50}, {50, 10, 0.1} },
        { {0, 5, 50},  {50, 10, 0.1} },
        { {-50, 5, 0}, {0.1, 10, 50} },
        { {50, 5, 0},  {0.1, 10, 50} }
    }
    for i, data in ipairs(wallData) do
        spawnWall(data[1], data[2])
    end
end