-- Vertex helper
function Vert(point, colour)
    return Vertex(point, {0, 0}, {0, 0}, colour)
end

-- Cuboid creation
function Cuboid(t, origin, size, colour)
    -- Front
    table.insert(t, Vert(origin + Vec3(0,       0,       0      ), colour))
    table.insert(t, Vert(origin + Vec3(0,       size.y,  0      ), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  0,       0      ), colour))

    table.insert(t, Vert(origin + Vec3(0,       size.y,  0      ), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  size.y,  0      ), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  0,       0      ), colour))

    -- Left
    table.insert(t, Vert(origin + Vec3(0,       0,       size.z ), colour))
    table.insert(t, Vert(origin + Vec3(0,       size.y,  0      ), colour))
    table.insert(t, Vert(origin + Vec3(0,       0,       0      ), colour))

    table.insert(t, Vert(origin + Vec3(0,       0,       size.z ), colour))
    table.insert(t, Vert(origin + Vec3(0,       size.y,  size.z ), colour))
    table.insert(t, Vert(origin + Vec3(0,       size.y,  0      ), colour))

    -- Right
    table.insert(t, Vert(origin + Vec3(size.x,  0,       0      ), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  size.y,  0      ), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  0,       size.z ), colour))

    table.insert(t, Vert(origin + Vec3(size.x,  size.y,  0      ), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  size.y,  size.z ), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  0,       size.z ), colour))

    -- Back
    table.insert(t, Vert(origin + Vec3(size.x,  0,        size.z), colour))
    table.insert(t, Vert(origin + Vec3(0,       size.y,   size.z), colour))
    table.insert(t, Vert(origin + Vec3(0,       0,        size.z), colour))

    table.insert(t, Vert(origin + Vec3(size.x,  0,        size.z), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  size.y,   size.z), colour))
    table.insert(t, Vert(origin + Vec3(0,       size.y,   size.z), colour))

    -- Top
    table.insert(t, Vert(origin + Vec3(0,       size.y,  size.z ), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  size.y,  0      ), colour))
    table.insert(t, Vert(origin + Vec3(0,       size.y,  0      ), colour))

    table.insert(t, Vert(origin + Vec3(0,       size.y,  size.z ), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  size.y,  size.z ), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  size.y,  0      ), colour))

    -- Bottom
    table.insert(t, Vert(origin + Vec3(0,       0,       0      ), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  0,       0      ), colour))
    table.insert(t, Vert(origin + Vec3(0,       0,       size.z ), colour))

    table.insert(t, Vert(origin + Vec3(size.x,  0,       0      ), colour))
    table.insert(t, Vert(origin + Vec3(size.x,  0,       size.z ), colour))
    table.insert(t, Vert(origin + Vec3(0,       0,       size.z ), colour))
end

-- Hollow cuboid creation
function HollowCuboid(verts, origin, size, thickness, includeFloor, includeCeiling)
    if includeFloor then
        -- Floor
        Cuboid(verts, origin + Vec3(0, 0, 0),       Vec3(size.x, thickness, size.z),    Colour(100, 100, 100, 255))
    end

    if includeCeiling then
        -- Ceiling
        Cuboid(verts, origin + Vec3(0, size.y, 0),    Vec3(size.x, thickness, size.z),    Colour(100, 100, 100, 255))
    end

    -- Front
    Cuboid(verts, origin + Vec3(0, thickness, 0),                           Vec3(size.x, size.y - thickness, thickness),                 Colour(80, 80, 80, 255))
    -- Back
    Cuboid(verts, origin + Vec3(0, thickness, size.z - thickness),           Vec3(size.x, size.y - thickness, thickness),                 Colour(80, 80, 80, 255))
    -- Left
    Cuboid(verts, origin + Vec3(0, thickness, thickness),                   Vec3(thickness, size.y - thickness, size.z - 2*thickness),   Colour(80, 80, 80, 255))
    -- Right
    Cuboid(verts, origin + Vec3(size.x - thickness, thickness, thickness),   Vec3(thickness, size.y - thickness, size.z - 2*thickness),   Colour(80, 80, 80, 255))
end