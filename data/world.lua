dofile "interface.lua"
dofile "primitives.lua"

-- Create a building from floors
function Building(origin, floorCount, width, depth, floorPadding)
    local verts = {}

    if not floorPadding then
        floorPadding = 0
    end

    local thickness = 0.15
    local height = 3
    for i=1, floorCount-1 do
        Floor(verts, origin, thickness, height, width, depth, true, false, floorPadding)
        origin = origin + Vec3(0, height, 0)
    end
    Floor(verts, origin, thickness, height, width, depth, true, true, floorPadding)

    mesh.add(verts)
end

-- Create a city from buildings
math.randomseed(0)

local width = 18
local depth = 24
local origin = Vec3(200, 56, 450)

local xCount = 8
local yCount = 8

for y=1, xCount do
    local rowOrigin = origin
    local maxDepth = 0

    for x=1, yCount do
        local ourWidth = width + ((math.random() - 0.5) * 10)
        local ourDepth = depth + ((math.random() - 0.5) * 10)
        maxDepth = math.max(maxDepth, ourDepth)

        local centeredX = x - xCount/2
        local centeredY = y - yCount/2
        local distFromCenter = math.sqrt(centeredX * centeredX + centeredY * centeredY)

        local height = math.floor(65 - 15 * (distFromCenter))
        height = height + (4 * math.random())

        Building(rowOrigin, height, ourWidth, ourDepth, math.random() * 0.5)
        rowOrigin = rowOrigin + Vec3(ourWidth + 5, 0, 0)
    end
    origin = origin + Vec3(0, 0, maxDepth + 5)
end