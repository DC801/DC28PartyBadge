#include <inttypes.h>

#include "mage_rom.h"

#include "EngineROM.h"
#include "EnginePanic.h"
#include "FrameBuffer.h"

MageAnimationFrame::MageAnimationFrame(uint32_t address)
{
	uint32_t size = 0; 
	//read tileindex
	if (EngineROM_Read(address, sizeof(tileId), (uint8_t *)&tileId) != sizeof(tileId))
	{
		goto MageAnimationFrame_Error;
	}

	// Endianness conversion
	convert_endian_u2(&tileId);

	// Increment offset
	address += sizeof(tileId);

	//read duration
	if (EngineROM_Read(address, sizeof(duration), (uint8_t *)&duration) != sizeof(duration))
	{
		goto MageAnimationFrame_Error;
	}

	// Endianness conversion
	convert_endian_u2(&duration);

	return;

MageAnimationFrame_Error:
	ENGINE_PANIC("Failed to read animation frame data");
}

uint16_t MageAnimationFrame::TileId() const
{
	return tileId;
}

uint16_t MageAnimationFrame::Duration() const
{
	return duration;
}

uint32_t MageAnimationFrame::Size() const
{
	uint32_t size = sizeof(tileId) +
		sizeof(duration);

	return size;
}

MageAnimation::MageAnimation(uint32_t address)
{
	uint32_t size = 0; 
	//read tilesetId
	if (EngineROM_Read(address, sizeof(tilesetId), (uint8_t *)&tilesetId) != sizeof(tilesetId))
	{
		goto MageAnimation_Error;
	}

	// Endianness conversion
	convert_endian_u2(&tilesetId);

	// Increment offset
	address += sizeof(tilesetId);

	//read frameCount
	if (EngineROM_Read(address, sizeof(frameCount), (uint8_t *)&frameCount) != sizeof(frameCount))
	{
		goto MageAnimation_Error;
	}

	// Endianness conversion
	convert_endian_u2(&frameCount);

	// Increment offset
	address += sizeof(frameCount);

	// Construct array
	animationFrames = std::make_unique<MageAnimationFrame[]>(frameCount);

	//fill array
	for(uint32_t frameIndex = 0; frameIndex < frameCount; frameIndex++)
	{
		animationFrames[frameIndex] = MageAnimationFrame(address);
		address += animationFrames[frameIndex].Size();
	}

	return;

MageAnimation_Error:
	ENGINE_PANIC("Failed to read animation data");
}

uint16_t MageAnimation::TilesetId() const
{
	return tilesetId;
}

uint16_t MageAnimation::FrameCount() const
{
	return frameCount;
}

MageAnimationFrame MageAnimation::AnimationFrame(uint32_t index) const
{
	if(index < frameCount)
	{
		return animationFrames[index];
	}
	else
	{
		return animationFrames[frameCount];
	}
}

uint32_t MageAnimation::Size() const
{
	uint32_t size = sizeof(tilesetId) +
		sizeof(frameCount);

	for (uint32_t i = 0; i < frameCount; i++)
	{
		size += animationFrames[i].Size();
	}

	return size;
}

MageEntityTypeAnimationDirection::MageEntityTypeAnimationDirection(uint32_t address)
{
	uint32_t size = 0;

	// Read count
	if (EngineROM_Read(address, sizeof(typeId), (uint8_t *)&typeId) != sizeof(typeId))
	{
		goto MageEntityTypeAnimationDirection_Error;
	}

	// Endianness conversion
	convert_endian_u2(&typeId);

	// Increment offset
	address += sizeof(typeId);

	// Read count
	if (EngineROM_Read(address, sizeof(type), (uint8_t *)&type) != sizeof(type))
	{
		goto MageEntityTypeAnimationDirection_Error;
	}

	// Increment offset
	address += sizeof(type);

	// Read count
	if (EngineROM_Read(address, sizeof(renderFlags), (uint8_t *)&renderFlags) != sizeof(renderFlags))
	{
		goto MageEntityTypeAnimationDirection_Error;
	}

	return;

MageEntityTypeAnimationDirection_Error:
	ENGINE_PANIC("Failed to read entity type animation direction data");
}

uint16_t MageEntityTypeAnimationDirection::TypeId() const
{
	return typeId;
}

uint8_t MageEntityTypeAnimationDirection::Type() const
{
	return type;
}

uint8_t MageEntityTypeAnimationDirection::RenderFlags() const
{
	return renderFlags;
}

bool MageEntityTypeAnimationDirection::FlipX() const
{
	return renderFlags & FLIPPED_HORIZONTALLY_FLAG;
}

bool MageEntityTypeAnimationDirection::FlipY() const
{
	return renderFlags & FLIPPED_VERTICALLY_FLAG;
}

bool MageEntityTypeAnimationDirection::FlipDiag() const
{
	return renderFlags & FLIPPED_DIAGONALLY_FLAG;
}

uint32_t MageEntityTypeAnimationDirection::Size() const
{
	uint32_t size = sizeof(typeId) +
		sizeof(type) +
		sizeof(renderFlags);

	return size;
}

MageEntityTypeAnimation::MageEntityTypeAnimation(uint32_t address)
{
	north = MageEntityTypeAnimationDirection(address);
	address += north.Size();
	east = MageEntityTypeAnimationDirection(address);
	address += east.Size();
	south = MageEntityTypeAnimationDirection(address);
	address += south.Size();
	west = MageEntityTypeAnimationDirection(address);

	return;
}

MageEntityTypeAnimationDirection MageEntityTypeAnimation::North() const
{
	return north;
}

MageEntityTypeAnimationDirection MageEntityTypeAnimation::East() const
{
	return east;
}

MageEntityTypeAnimationDirection MageEntityTypeAnimation::South() const
{
	return south;
}

MageEntityTypeAnimationDirection MageEntityTypeAnimation::West() const
{
	return west;
}

uint32_t MageEntityTypeAnimation::Size() const
{
	uint32_t size = north.Size() +
		south.Size() +
		east.Size() +
		west.Size();

	return size;
}

MageEntityType::MageEntityType(uint32_t address)
{
	uint32_t size = 0;

	// Read name
	if (EngineROM_Read(address, 16, (uint8_t *)name) != 16)
	{
		goto MageEntityType_Error;
	}

	name[16] = 0; // Null terminate
	//increment address
	address += 16 + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t); //padding

	// Read animationCount
	if (EngineROM_Read(address, sizeof(animationCount), (uint8_t *)&animationCount) != sizeof(animationCount))
	{
		goto MageEntityType_Error;
	}

	//increment address
	address += sizeof(animationCount);

	// Construct array
	entityTypeAnimations = std::make_unique<MageEntityTypeAnimation[]>(animationCount);

	//increment through animations to fill entityTypeAnimations array:
	for(uint32_t animationIndex = 0; animationIndex < animationCount; animationIndex++)
	{
		entityTypeAnimations[animationIndex] = MageEntityTypeAnimation(address);
		address += entityTypeAnimations[animationIndex].Size();
	}

	return;

MageEntityType_Error:
	ENGINE_PANIC("Failed to read entity type direction data");
}

std::string MageEntityType::Name() const
{
	return std::string(name);
}

uint8_t MageEntityType::AnimationCount() const
{
	return animationCount;
}

MageEntityTypeAnimation MageEntityType::EntityTypeAnimation(uint32_t index) const
{
	if(index < animationCount)
	{
		return entityTypeAnimations[index];
	}
	else
	{
		return entityTypeAnimations[animationCount];
	}
}

uint32_t MageEntityType::Size() const
{
	uint32_t size = sizeof(name) +
		sizeof(paddingA) +
		sizeof(paddingB) +
		sizeof(paddingC) +
		sizeof(animationCount);

	for (uint32_t i = 0; i < animationCount; i++)
	{
		size += entityTypeAnimations[i].Size();
	}

	return size;
}

// Initializer list, default construct values
//   Don't waste any resources constructing unique_ptr's
//   Each header is constructed with offsets from the previous
MageGameControl::MageGameControl()
{
	uint32_t offset = 8;

	currentMapId = DEFAULT_MAP;

	mapHeader = MageHeader(offset);
	offset += mapHeader.size();

	tileHeader = MageHeader(offset);
	offset += tileHeader.size();

	animationHeader = MageHeader(offset);
	offset += animationHeader.size();

	entityTypeHeader = MageHeader(offset);
	offset += entityTypeHeader.size();

	entityHeader = MageHeader(offset);
	offset += entityHeader.size();

	imageHeader = MageHeader(offset);
	offset += imageHeader.size();

	tilesets = std::make_unique<MageTileset[]>(tileHeader.count());

	for (uint32_t i = 0; i < tileHeader.count(); i++)
	{
		tilesets[i] = MageTileset(tileHeader.offset(i));
	}
	
	animations = std::make_unique<MageAnimation[]>(animationHeader.count());

	for (uint32_t i = 0; i < animationHeader.count(); i++)
	{
		animations[i] = MageAnimation(animationHeader.offset(i));
	}
	
	entityTypes = std::make_unique<MageEntityType[]>(entityTypeHeader.count());

	for (uint32_t i = 0; i < entityTypeHeader.count(); i++)
	{
		entityTypes[i] = MageEntityType(entityTypeHeader.offset(i));
	}

	entities = std::make_unique<MageEntity[]>(MAX_ENTITIES_PER_MAP);
	
	playerEntityIndex = NO_PLAYER;

	entityRenderableData = std::make_unique<MageEntityRenderableData[]>(MAX_ENTITIES_PER_MAP);

	//load the map
	LoadMap(currentMapId);
}

uint32_t MageGameControl::Size() const
{
	uint32_t size = sizeof(currentMapId) +
		mapHeader.size() +
		tileHeader.size() +
		animationHeader.size() +
		entityTypeHeader.size() +
		entityHeader.size() +
		imageHeader.size() +
		map.Size() +
		sizeof(MageEntity)*MAX_ENTITIES_PER_MAP+
		sizeof(uint16_t)*MAX_ENTITIES_PER_MAP;

	for (uint32_t i = 0; i < tileHeader.count(); i++)
	{
		size += tilesets[i].Size();
	}

	for (uint32_t i = 0; i < animationHeader.count(); i++)
	{
		size += animations[i].Size();
	}

	for (uint32_t i = 0; i < entityHeader.count(); i++)
	{
		size += entityTypes[i].Size();
	}

	return size;
}

const MageTileset& MageGameControl::Tile(uint32_t index) const
{
	static MageTileset tile;
	if (!tilesets) return tile;

	if (tileHeader.count() > index)
	{
		return tilesets[index];
	}

	return tile;
}

const MageMap& MageGameControl::Map() const
{
	return map;
}

MageEntity MageGameControl::LoadEntity(uint32_t address)
{
	uint32_t size = 0;
	MageEntity entity;

	//Read Name
	if (EngineROM_Read(address, 16, (uint8_t *)entity.name) != 16)
	{
		goto MageEntity_Error;
	}

	//increment address
	address += 16;

	// Read entity.primaryId
	if (EngineROM_Read(address, sizeof(entity.primaryId), (uint8_t *)&entity.primaryId) != sizeof(entity.primaryId))
	{
		goto MageEntity_Error;
	}

	// Endianness conversion
	convert_endian_u2(&entity.primaryId);

	//increment address
	address += sizeof(entity.primaryId);

	// Read entity.secondaryId
	if (EngineROM_Read(address, sizeof(entity.secondaryId), (uint8_t *)&entity.secondaryId) != sizeof(entity.secondaryId))
	{
		goto MageEntity_Error;
	}

	// Endianness conversion
	convert_endian_u2(&entity.secondaryId);

	//increment address
	address += sizeof(entity.secondaryId);

	// Read entity.scriptId
	if (EngineROM_Read(address, sizeof(entity.scriptId), (uint8_t *)&entity.scriptId) != sizeof(entity.scriptId))
	{
		goto MageEntity_Error;
	}

	// Endianness conversion
	convert_endian_u2(&entity.scriptId);

	//increment address
	address += sizeof(entity.scriptId);

	// Read entity.x
	if (EngineROM_Read(address, sizeof(entity.x), (uint8_t *)&entity.x) != sizeof(entity.x))
	{
		goto MageEntity_Error;
	}

	// Endianness conversion
	convert_endian_u2(&entity.x);

	//increment address
	address += sizeof(entity.x);

	// Read entity.y
	if (EngineROM_Read(address, sizeof(entity.y), (uint8_t *)&entity.y) != sizeof(entity.y))
	{
		goto MageEntity_Error;
	}

	// Endianness conversion
	convert_endian_u2(&entity.y);

	//increment address
	address += sizeof(entity.y);

	// Read entity.primaryIdType
	if (EngineROM_Read(address, sizeof(entity.primaryIdType), (uint8_t *)&entity.primaryIdType) != sizeof(entity.primaryIdType))
	{
		goto MageEntity_Error;
	}

	//increment address
	address += sizeof(entity.primaryIdType);

	// Read entity.currentAnimation
	if (EngineROM_Read(address, sizeof(entity.currentAnimation), (uint8_t *)&entity.currentAnimation) != sizeof(entity.currentAnimation))
	{
		goto MageEntity_Error;
	}

	//increment address
	address += sizeof(entity.currentAnimation);

	// Read entity.currentFrame
	if (EngineROM_Read(address, sizeof(entity.currentFrame), (uint8_t *)&entity.currentFrame) != sizeof(entity.currentFrame))
	{
		goto MageEntity_Error;
	}

	//increment address
	address += sizeof(entity.currentFrame);

	// Read entity.direction
	if (EngineROM_Read(address, sizeof(entity.direction), (uint8_t *)&entity.direction) != sizeof(entity.direction))
	{
		goto MageEntity_Error;
	}

	//increment address
	address += sizeof(entity.direction);

	// Read entity.hackableState
	if (EngineROM_Read(address, sizeof(entity.hackableState), (uint8_t *)&entity.hackableState) != sizeof(entity.hackableState))
	{
		goto MageEntity_Error;
	}

	//set padding to 0
	entity.padding = 0;

	return entity;

MageEntity_Error:
	ENGINE_PANIC("Failed to read entity type direction data");
}

void MageGameControl::LoadMap(uint16_t index)
{
	currentMapId = index;

	//load new map:
	map = MageMap(mapHeader.offset(currentMapId));

	#ifdef DC801_DESKTOP
		if(map.EntityCount() > MAX_ENTITIES_PER_MAP)
		{
			fprintf(stderr, "Error: Game is attempting to load more than 32 entities on one map.");
		}
	#endif
	
	for (uint32_t i = 0; i < MAX_ENTITIES_PER_MAP; i++)
	{
		//only populate the entities that are on the current map.
		if(i < entityHeader.count())
		{
			entities[i] = LoadEntity(entityHeader.offset(map.EntityId(i)));
		}
	}

	for (uint32_t i = 0; i < MAX_ENTITIES_PER_MAP; i++)
	{
		//all entities start with 0 frame ticks
		entityRenderableData[i].currentFrameTicks = 0;
		//other values are filled in when getEntityRenderableData is called:
		getEntityRenderableData(i);
	}

	//update playerEntity pointer whenever a new map is loaded:
	GetPointerToPlayerEntity(std::string(PLAYER_CHARACTER_NAME_STRING));
}

void MageGameControl::GetPointerToPlayerEntity(std::string name)
{

	for(uint16_t i=0; i<map.EntityCount(); i++)
	{
		if(entities[i].primaryIdType == MageEntityPrimaryIdType::ENTITY_TYPE)
		{
			if(std::string(entities[i].name) == name)
			{
				// printf("Is this entity the player? A: %s; B %s\n", std::string(entities[i].name).c_str(), name.c_str());
				playerEntityIndex = i;
			}
		}
		else if(i == (map.EntityCount()-1))
		{
			//if no valid match for the player is in the map, return NO_PLAYER
			playerEntityIndex = NO_PLAYER;
		}
	}
}

void MageGameControl::DrawMap(uint8_t layer, int32_t camera_x, int32_t camera_y) const
{
	uint32_t tilesPerLayer = map.Width() * map.Height();

	for (uint32_t i = 0; i < tilesPerLayer; i++)
	{
		int32_t x = (int32_t)((map.TileWidth() * (i % map.Width())) - camera_x);
		int32_t y = (int32_t)((map.TileHeight() * (i / map.Width())) - camera_y);

		if ((x < (-map.TileWidth()) ||
			(x > WIDTH) ||
			(y < (-map.TileHeight())) ||
			(y > HEIGHT)))
		{
			continue;
		}

		uint32_t address = map.LayerOffset(layer);

		if (address == 0)
		{
			continue;
		}


		uint16_t tileId = 0;
		uint8_t tilesetId = 0;
		uint8_t flags = 0;

		address += i * (sizeof(tileId) + sizeof(tilesetId) + sizeof(flags));

		if (EngineROM_Read(address, sizeof(tileId), (uint8_t *)&tileId) != sizeof(tileId))
		{
			ENGINE_PANIC("Failed to fetch map layer tile info");
		}

		convert_endian_u2(&tileId);
		address += sizeof(tileId);

		if (tileId == 0)
		{
			continue;
		}

		tileId -= 1;

		if (EngineROM_Read(address, sizeof(tilesetId), &tilesetId) != sizeof(tilesetId))
		{
			ENGINE_PANIC("Failed to fetch map layer tile info");
		}

		address += sizeof(tilesetId);

		if (EngineROM_Read(address, sizeof(flags), &flags) != sizeof(flags))
		{
			ENGINE_PANIC("Failed to fetch map layer tile info");
		}

		const MageTileset &tileset = Tile(tilesetId);

		if (tileset.Valid() != true)
		{
			continue;
		}

		address = imageHeader.offset(tileset.ImageId());

		canvas.drawChunkWithFlags(
			address,
			x,
			y,
			tileset.TileWidth(),
			tileset.TileHeight(),
			(tileId % tileset.Cols()) * tileset.TileWidth(),
			(tileId / tileset.Cols()) * tileset.TileHeight(),
			tileset.ImageWidth(),
			TRANSPARENCY_COLOR,
			flags
		);

		// if (led_states[LED_MEM0])
		// {
		// 	canvas.blt();
		// }
	}
}

void MageGameControl::getEntityRenderableData(uint32_t index)
{
	//current entity for use in the loop:
	MageEntity currentEnt = entities[index];

	//increment frame and reset ticks if frame has been active long enough:
	//Scenario 1: entity is of a standard EntityType:
	if(currentEnt.primaryIdType == MageEntityPrimaryIdType::ENTITY_TYPE)
	{
		//check if entity type has an animation count:
		uint16_t entityTypeIndex = entities[index].primaryId;
		if(( entityTypes[entityTypeIndex].AnimationCount() ) > 0)
		{
			//get the current animation
			MageEntityTypeAnimation currentAnimation = entityTypes[entityTypeIndex].EntityTypeAnimation(currentEnt.currentAnimation);
			MageEntityTypeAnimationDirection directedAnimation; 
			if(currentEnt.direction == MageEntityAnimationDirection::NORTH)
			{
				directedAnimation = currentAnimation.North();
			}
			else if(currentEnt.direction == MageEntityAnimationDirection::EAST)
			{
				directedAnimation = currentAnimation.East();
			}
			else if(currentEnt.direction == MageEntityAnimationDirection::SOUTH)
			{
				directedAnimation = currentAnimation.South();
			}
			else if(currentEnt.direction == MageEntityAnimationDirection::WEST)
			{
				directedAnimation = currentAnimation.West();
			}
			else
			{
				//this shouldn't be possible, so error:
				#ifdef DC801_DESKTOP
					fprintf(stderr, "Error: entity primaryIdType is invalid.");
				#endif
			}
			//now that we have the animation, figure out the current animation index that needs to be run:
			if(directedAnimation.Type() == 0)
			{
				entityRenderableData[index].tilesetId = animations[directedAnimation.TypeId()].TilesetId();
				entityRenderableData[index].tileId = animations[directedAnimation.TypeId()].AnimationFrame(currentEnt.currentFrame).TileId();
				entityRenderableData[index].duration = animations[directedAnimation.TypeId()].AnimationFrame(currentEnt.currentFrame).Duration();
				entityRenderableData[index].frameCount = animations[directedAnimation.TypeId()].FrameCount();
				entityRenderableData[index].renderFlags = directedAnimation.RenderFlags();
			}
			else
			{
				//do I need to subtract 1 from this to get the right tileset, since they are 0-indexed? -Tim
				entityRenderableData[index].tilesetId = directedAnimation.Type();
				entityRenderableData[index].tileId = directedAnimation.TypeId();
				entityRenderableData[index].duration = animations[directedAnimation.TypeId()].AnimationFrame(currentEnt.currentFrame).Duration();
				entityRenderableData[index].frameCount = animations[directedAnimation.TypeId()].FrameCount();
				entityRenderableData[index].renderFlags = directedAnimation.RenderFlags();
			}
		}
	}
	//Scenario 2: entity is an animation:
	else if(entities[index].primaryIdType == MageEntityPrimaryIdType::ANIMATION)
	{
		uint16_t aniIndex = entities[index].primaryId;
		entityRenderableData[index].tilesetId = animations[aniIndex].TilesetId();
		entityRenderableData[index].tileId = animations[aniIndex].AnimationFrame(entities[index].currentFrame).TileId();
		entityRenderableData[index].duration = animations[aniIndex].AnimationFrame(currentEnt.currentAnimation).Duration();
		entityRenderableData[index].frameCount = animations[aniIndex].FrameCount();
		entityRenderableData[index].renderFlags = entities[index].direction;
	}
	else if(entities[index].primaryIdType == MageEntityPrimaryIdType::TILESET)
	{
		entityRenderableData[index].tilesetId = entities[index].primaryId;
		entityRenderableData[index].tileId = entities[index].secondaryId;
		entityRenderableData[index].duration = 0; //unused
		entityRenderableData[index].frameCount = 0; //unused
		entityRenderableData[index].renderFlags = entities[index].direction;
	}
	else
	{
		//this shouldn't be possible, so error:
		#ifdef DC801_DESKTOP
			fprintf(stderr, "Error: entity primaryIdType is invalid.");
		#endif
	}
}

void MageGameControl::UpdateEntities(uint32_t deltaTime)
{
	//cycle through all map entities:
	for(uint8_t i = 0; i < map.EntityCount(); i++)
	{
		//increment the frame ticks based on the delta_time since the last check:
		entityRenderableData[i].currentFrameTicks += deltaTime;
		
		//update entity info:
		getEntityRenderableData(i);

		//check for frame change and adjust if needed:
		if(entityRenderableData[i].currentFrameTicks >= entityRenderableData[i].duration)
		{
			entities[i].currentFrame++;
			if(entities[i].currentFrame >= entityRenderableData[i].frameCount)
			{
				entities[i].currentFrame = 0;
			}
		
			//update the entity info again with the corrected frame index:
			getEntityRenderableData(i);
		}
	}
}

void MageGameControl::DrawEntities(int32_t cameraX, int32_t cameraY)
{
	//first sort entities by their y values:
	uint16_t entitySortOrder[map.EntityCount()];
	//init index array:
	for(uint16_t i=0; i<map.EntityCount(); i++)
	{
		entitySortOrder[i] = i;
	}

	// One by one move boundary of unsorted subarray
	for (uint16_t i = 0; i < map.EntityCount() - 1; i++)
	{

		// Find the minimum element in unsorted array
		uint16_t min_idx = i;
		for (uint16_t j = i + 1; j < map.EntityCount(); j++)
		{
			if (entities[entitySortOrder[j]].y < entities[entitySortOrder[min_idx]].y)
			{
				min_idx = j;
			}
		}

		// Swap the found minimum element
		// with the first element
		uint16_t temp = entitySortOrder[min_idx];
		entitySortOrder[min_idx] = entitySortOrder[i];
		entitySortOrder[i] = temp;
	}

	//now that we've got a sorted array with the lowest y values first, 
	//iterate through it and draw the entities one by one:
	for(uint16_t i=0; i<map.EntityCount(); i++)
	{
		uint16_t entityIndex = entitySortOrder[i];
		uint32_t imageIndex = tilesets[entityRenderableData[entityIndex].tilesetId].ImageId();
		uint32_t tileWidth = tilesets[entityRenderableData[entityIndex].tilesetId].TileWidth();
		uint32_t tileHeight = tilesets[entityRenderableData[entityIndex].tilesetId].TileHeight();
		uint32_t cols = tilesets[entityRenderableData[entityIndex].tilesetId].Cols();
		uint32_t tileId = entityRenderableData[entityIndex].tileId;
		uint32_t address = imageHeader.offset(imageIndex);

		int32_t source_x = (tileId % cols) * tileWidth;
		int32_t source_y = (tileId / cols) * tileHeight;

		int32_t x = entities[entityIndex].x - cameraX;
		int32_t y = entities[entityIndex].y - cameraY - tileHeight;
		canvas.drawChunkWithFlags(
			address,
			x,
			y,
			tilesets[entityRenderableData[entityIndex].tilesetId].TileWidth(),
			tilesets[entityRenderableData[entityIndex].tilesetId].TileHeight(),
			source_x,
			source_y,
			tilesets[entityRenderableData[entityIndex].tilesetId].ImageWidth(),
			TRANSPARENCY_COLOR,
			entityRenderableData[entityIndex].renderFlags
		);
	}
}
