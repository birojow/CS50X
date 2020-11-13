--[[
    Contains tile data and necessary code for rendering a tile map to the
    screen.
]]

require 'Util'

Map = Class{}

-- tiles
TILE_BRICK = 1
TILE_EMPTY = -1

-- cloud tiles
CLOUD_LEFT = 6
CLOUD_RIGHT = 7

-- bush tiles
BUSH_LEFT = 2
BUSH_RIGHT = 3

-- mushroom tiles
MUSHROOM_TOP = 10
MUSHROOM_BOTTOM = 11

-- jump block
JUMP_BLOCK = 5
JUMP_BLOCK_HIT = 9

-- flagpole tiles
FLAGPOLE_BASE = 16
FLAGPOLE_MIDDLE = 12
FLAGPOLE_TOP = 8

-- sets the flagpole height
FLAGPOLE_HEIGHT = 8

-- flagpole distance to the edge
FLAGPOLE_EDGE = 3

-- flag tiles
FLAG_UP_1 = 13
FLAG_UP_2 = 14
FLAG_DOWN = 15

-- sets the pyramid height
PYRAMID_HEIGHT = 7

-- tiles between pyramid an flagpole
DISTANCE_P_TO_F = 5

-- a speed to multiply delta time to scroll map; smooth value
local SCROLL_SPEED = 62

-- constructor for our map object
function Map:init()

    self.spritesheet = love.graphics.newImage('graphics/spritesheet.png')
    self.sprites = generateQuads(self.spritesheet, 16, 16)
    self.music = love.audio.newSource('sounds/music.wav', 'static')

    self.tileWidth = 16
    self.tileHeight = 16
    self.mapWidth = 30
    self.mapHeight = 28
    self.tiles = {}

    -- applies positive Y influence on anything affected
    self.gravity = 15

    -- associate player with map
    self.player = Player(self)

    -- camera offsets
    self.camX = 0
    self.camY = -3

    -- cache width and height of map in pixels
    self.mapWidthPixels = self.mapWidth * self.tileWidth
    self.mapHeightPixels = self.mapHeight * self.tileHeight

    -- first, fill map with empty tiles
    for y = 1, self.mapHeight do
        for x = 1, self.mapWidth do
            
            -- support for multiple sheets per tile; storing tiles as tables 
            self:setTile(x, y, TILE_EMPTY)
        end
    end

    -- begin generating the terrain using vertical scan lines
    local x = 1
    while x < self.mapWidth do
        -- creates the pyramid, the flagpole, and the flag
        if x == self.mapWidth - PYRAMID_HEIGHT - DISTANCE_P_TO_F - FLAGPOLE_EDGE  then
            -- creates column of tiles going to bottom of map,
            -- from start of pyramid to edge
            for w = x, self.mapWidth, 1 do
                for y = self.mapHeight / 2, self.mapHeight do
                    self:setTile(w, y, TILE_BRICK)
                end
            end

            -- creates the pyramid
            for c = 0, PYRAMID_HEIGHT, 1 do
                for y = self.mapHeight / 2 - c, self.mapHeight / 2 do
                    self:setTile(x, y, TILE_BRICK)
                end
                x = x + 1
            end

            -- creates the flagpole
            x = x + DISTANCE_P_TO_F
            self:setTile(x, self.mapHeight / 2 - FLAGPOLE_HEIGHT, FLAGPOLE_TOP)
            for i = FLAGPOLE_HEIGHT - 1, 2, -1 do
                self:setTile(x, self.mapHeight / 2 - i, FLAGPOLE_MIDDLE)
            end
            self:setTile(x, self.mapHeight / 2 - 1, FLAGPOLE_BASE)

            -- creates the flag
            self:setTile(x + 1, self.mapHeight / 2 - FLAGPOLE_HEIGHT, FLAG_UP_1)

            break
        else
            -- 2% chance to generate a cloud
            -- make sure we're 2 tiles from edge at least
            if x < self.mapWidth - 2 then
                if math.random(20) == 1 then
                    
                    -- choose a random vertical spot 2 TILES ABOVE FLAGPOLE HEIGHT
                    local cloudStart = math.random(self.mapHeight / 2 - FLAGPOLE_HEIGHT - 2)

                    self:setTile(x, cloudStart, CLOUD_LEFT)
                    self:setTile(x + 1, cloudStart, CLOUD_RIGHT)
                end
            end

            -- 5% chance to generate a mushroom, AWAY FROM PYRAMID START
            if math.random(20) == 1 and x < self.mapWidth - PYRAMID_HEIGHT - DISTANCE_P_TO_F - FLAGPOLE_EDGE - 1 then
                -- left side of pipe
                self:setTile(x, self.mapHeight / 2 - 2, MUSHROOM_TOP)
                self:setTile(x, self.mapHeight / 2 - 1, MUSHROOM_BOTTOM)

                -- creates column of tiles going to bottom of map
                for y = self.mapHeight / 2, self.mapHeight do
                    self:setTile(x, y, TILE_BRICK)
                end

                -- next vertical scan line
                x = x + 1

            -- 10% chance to generate bush, being sure to generate AWAY FROM PYRAMID START
            elseif math.random(10) == 1 and x < self.mapWidth - PYRAMID_HEIGHT - DISTANCE_P_TO_F - FLAGPOLE_EDGE - 2 then
                local bushLevel = self.mapHeight / 2 - 1

                -- place bush component and then column of bricks
                self:setTile(x, bushLevel, BUSH_LEFT)
                for y = self.mapHeight / 2, self.mapHeight do
                    self:setTile(x, y, TILE_BRICK)
                end
                x = x + 1
                -- place second bush component and then column of bricks
                self:setTile(x, bushLevel, BUSH_RIGHT)
                for y = self.mapHeight / 2, self.mapHeight do
                    self:setTile(x, y, TILE_BRICK)
                end
                x = x + 1

            -- 10% chance to not generate anything, creating a gap
            elseif math.random(10) ~= 1 then
                -- creates column of tiles going to bottom of map
                for y = self.mapHeight / 2, self.mapHeight do
                    self:setTile(x, y, TILE_BRICK)
                end

                -- chance to create a block for Mario to hit
                -- do not create if too close to pyramid start
                if math.random(15) == 1 and x < self.mapWidth - PYRAMID_HEIGHT - DISTANCE_P_TO_F - FLAGPOLE_EDGE - 1 then
                    self:setTile(x, self.mapHeight / 2 - 4, JUMP_BLOCK)
                end

                -- next vertical scan line
                x = x + 1
            else
                if x < self.mapWidth - PYRAMID_HEIGHT - DISTANCE_P_TO_F - FLAGPOLE_EDGE - 2 then
                    x = x + 2
                end
            end
        end
    end

    -- start the background music
    self.music:setLooping(true)
    self.music:play()
end

-- return whether a given tile is collidable
function Map:collides(tile)
    -- define our collidable tiles
    local collidables = {
        TILE_BRICK, JUMP_BLOCK, JUMP_BLOCK_HIT,
        MUSHROOM_TOP, MUSHROOM_BOTTOM
    }

    -- iterate and return true if our tile type matches
    for _, v in ipairs(collidables) do
        if tile.id == v then
            return true
        end
    end

    return false
end

-- return whether a given tile is the flag
function Map:collidesWithFlag(tile)
    -- define our collidable tiles
    local collidables = {
        FLAGPOLE_BASE, FLAGPOLE_MIDDLE, FLAGPOLE_TOP,
        FLAG_UP_1, FLAG_UP_2
    }

    -- iterate and return true if our tile type matches
    for _, v in ipairs(collidables) do
        if tile.id == v then
            return true
        end
    end

    return false
end

-- function to update camera offset with delta time
function Map:update(dt)
    self.player:update(dt)
    
    -- keep camera's X coordinate following the player, preventing camera from
    -- scrolling past 0 to the left and the map's width
    self.camX = math.max(0, math.min(self.player.x - VIRTUAL_WIDTH / 2,
        math.min(self.mapWidthPixels - VIRTUAL_WIDTH, self.player.x)))
end

-- gets the tile type at a given pixel coordinate
function Map:tileAt(x, y)
    return {
        x = math.floor(x / self.tileWidth) + 1,
        y = math.floor(y / self.tileHeight) + 1,
        id = self:getTile(math.floor(x / self.tileWidth) + 1, math.floor(y / self.tileHeight) + 1)
    }
end

-- returns an integer value for the tile at a given x-y coordinate
function Map:getTile(x, y)
    return self.tiles[(y - 1) * self.mapWidth + x]
end

-- sets a tile at a given x-y coordinate to an integer value
function Map:setTile(x, y, id)
    self.tiles[(y - 1) * self.mapWidth + x] = id
end

-- renders our map to the screen, to be called by main's render
function Map:render()
    for y = 1, self.mapHeight do
        for x = 1, self.mapWidth do
            local tile = self:getTile(x, y)
            if tile ~= TILE_EMPTY then
                love.graphics.draw(self.spritesheet, self.sprites[tile],
                    (x - 1) * self.tileWidth, (y - 1) * self.tileHeight)
            end
        end
    end

    self.player:render()
end
