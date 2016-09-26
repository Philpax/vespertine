dofile "interface.lua"
dofile "primitives.lua"

math.randomseed(os.time())

lastBuilding = nil
function MakeBuilding(levelCount, wallHeight, windowCount, cellWidth, windowWidth, thickness)
    if lastBuilding ~= nil then
        mesh.remove(lastBuilding)
    end

	print("Making building")

    local verts = {}
    local origin = Vec3(200, 58, 450)
    local windowSize = Vec2(windowWidth, windowWidth + 0.2)

    for level = 0, levelCount-1 do
        local levelOrigin = origin + Vec3(0, wallHeight * level, 0)
        Cuboid(verts, levelOrigin + Vec3(thickness, 0, thickness), Vec3(windowCount*cellWidth - 2*thickness, 0.1, windowCount*cellWidth - 2*thickness), Colour(80, 80, 80, 255))

        local colourScale = math.lerp(80, 120, (level+1)/levelCount)
        local colour = Colour(colourScale, colourScale, colourScale, 255)
        for i = 0, windowCount-1 do
            -- Front
            WindowWallCell(verts, levelOrigin + Vec3(cellWidth*i, 0, 0), Vec3(cellWidth, wallHeight, thickness), colour, windowSize, false)
            -- Back
            WindowWallCell(verts, levelOrigin + Vec3(cellWidth*i, 0, windowCount*cellWidth - thickness), Vec3(cellWidth, wallHeight, thickness), colour, windowSize, false)
            -- Left
            WindowWallCell(verts, levelOrigin + Vec3(0, 0, cellWidth*i), Vec3(cellWidth, wallHeight, thickness), colour, windowSize, true)
            -- Right
            WindowWallCell(verts, levelOrigin + Vec3(windowCount*cellWidth - thickness, 0, cellWidth*i), Vec3(cellWidth, wallHeight, thickness), colour, windowSize, true)
        end
    end

    -- Final ceiling
    Cuboid(verts, origin + Vec3(0, levelCount*wallHeight, 0), Vec3(windowCount*cellWidth, 0.1, windowCount*cellWidth), Colour(80, 80, 80, 255))
    -- Corner pillars
    local pillarSize = 0.5
    Cuboid(verts, origin + Vec3(thickness, 0, thickness), Vec3(pillarSize, levelCount*wallHeight, pillarSize), Colour(60, 60, 60, 255))
    Cuboid(verts, origin + Vec3(windowCount*cellWidth - (pillarSize + thickness), 0, thickness), Vec3(pillarSize, levelCount*wallHeight, pillarSize), Colour(60, 60, 60, 255))
    Cuboid(verts, origin + Vec3(thickness, 0, windowCount*cellWidth - (pillarSize + thickness)), Vec3(pillarSize, levelCount*wallHeight, pillarSize), Colour(60, 60, 60, 255))
    Cuboid(verts, origin + Vec3(windowCount*cellWidth - (pillarSize + thickness), 0, windowCount*cellWidth - (pillarSize + thickness)), Vec3(pillarSize, levelCount*wallHeight, pillarSize), Colour(60, 60, 60, 255))

    lastBuilding = mesh.add(verts)
    print("New building!")
end

levelCount = 10
wallHeight = 3
windowCount = 4
cellWidth = 4
windowWidth = cellWidth - 0.5
thickness = 0.15

function pulse()
    imgui.window("World Control", function()
		levelCount = imgui.sliderInt("Levels", levelCount, 3, 20)
		imgui.separator()

		cellWidth = imgui.sliderDec("Cell Width", cellWidth, 3, 6)
		wallHeight = imgui.sliderDec("Wall Height", wallHeight, 2, 4)
		thickness = imgui.sliderDec("Wall Thickness", thickness, 0.05, 0.25)
		imgui.separator()
		
		windowCount = imgui.sliderInt("Window Count", windowCount, 3, 7)
		windowWidth = imgui.sliderDec("Window Width", windowWidth, 0.25, cellWidth - 0.25)
		imgui.separator()

        if imgui.button("Make Building") then
			MakeBuilding(levelCount, wallHeight, windowCount, cellWidth, windowWidth, thickness)
		end
    end)
end