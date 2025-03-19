-------------------------------------------------------------------------------
-- main_example1.lua
--
-- Test script invokes some LuaPrint() calls to show communication between
-- C++ and script.
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- init
-------------------------------------------------------------------------------
function init()

	-- Example table
	primesTable = {1, 2, 3, 5, 7, 11}
	for i,v in ipairs(primesTable) do
		LuaPrint("key[" .. i .. "] = value[" .. v .. "]")
	end
	
	-- Example table with non-integer based keys
	animLengthTable = {walk = 54, idle = 23, run = 41}
	for k,v in pairs(animLengthTable) do
		LuaPrint("key[" .. k .. "] = value[" .. v .. "]")
	end
	
	-- Everything initialized OK
	return true
end

-------------------------------------------------------------------------------
-- update
-------------------------------------------------------------------------------
function update(deltaTime)

	-- We're done running, return false.
	return false
end