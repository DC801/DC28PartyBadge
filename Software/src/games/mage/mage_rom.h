#ifndef _MAGE_ROM_H
#define _MAGE_ROM_H

#include <memory>
#include <string>
#include "mage.h"

class MageHeader
{
private:
	uint32_t counts;
	std::unique_ptr<uint32_t[]> offsets;
	std::unique_ptr<uint32_t[]> lengths;

public:
	MageHeader() : counts{0},
		offsets{std::make_unique<uint32_t[]>(1)},
		lengths{std::make_unique<uint32_t[]>(1)}
	{ };

	MageHeader(uint32_t address);

	uint32_t count() const;
	uint32_t offset(uint8_t num) const;
	uint32_t length(uint8_t num) const;
	uint32_t size() const;
	bool valid() const;
}; //class MageHeader

class MageMap
{
private:
	char name[17];
	uint16_t tileWidth;
	uint16_t tileHeight;
	uint16_t width;
	uint16_t height;
	uint8_t layerCount;
	uint16_t entityCount;
	std::unique_ptr<uint16_t[]> entityGlobalIds;
	std::unique_ptr<uint32_t[]> mapLayerOffsets;

public:
	MageMap() : name{0},
		tileWidth{0},
		tileHeight{0},
		width{0},
		height{0},
		layerCount{0},
		entityCount{0},
		entityGlobalIds{std::make_unique<uint16_t[]>(1)},
		mapLayerOffsets{std::make_unique<uint32_t[]>(1)}
	{ };

	MageMap(uint32_t address);

	std::string Name() const;
	uint16_t TileWidth() const;
	uint16_t TileHeight() const;
	uint16_t Width() const;
	uint16_t Height() const;
	uint8_t LayerCount() const;
	uint16_t EntityCount() const;
	uint16_t EntityId(uint16_t num) const;
	uint32_t LayerOffset(uint16_t num) const;
	uint32_t Size() const;
}; //class MageMap

class MageTileset
{
private:
	char name[17];
	uint16_t imageIndex;
	uint16_t imageWidth;
	uint16_t imageHeight;
	uint16_t tileWidth;
	uint16_t tileHeight;
	uint16_t cols;
	uint16_t rows;
	std::unique_ptr<uint8_t[]> tiles;

public:
	MageTileset() : name{""},
		imageIndex{0},
		imageWidth{0},
		imageHeight{0},
		tileWidth{0},
		tileHeight{0},
		cols{0},
		rows{0},
		tiles{std::make_unique<uint8_t[]>(1)}
	{ };

	MageTileset(uint32_t address);

	std::string Name() const;
	uint16_t ImageIndex() const;
	uint16_t ImageWidth() const;
	uint16_t ImageHeight() const;
	uint16_t TileWidth() const;
	uint16_t TileHeight() const;
	uint16_t Cols() const;
	uint16_t Rows() const;
	uint8_t Tile(uint32_t index) const;
	uint32_t Size() const;
	bool Valid() const;
}; //class MageTileset

class MageAnimationFrame
{
private:
	uint16_t tileIndex;
	uint16_t duration;
public:
	MageAnimationFrame() : tileIndex{0},
		duration{0}
	{};

	MageAnimationFrame(uint32_t address);

	uint16_t TileIndex() const;
	uint16_t Duration() const;
	uint32_t Size() const;
}; //class MageAnimationFrame


class MageAnimation
{
private:
	uint16_t tilesetIndex;
	uint16_t frameCount;

	std::unique_ptr<MageAnimationFrame[]> animationFrames;
public:
	MageAnimation() : tilesetIndex{0},
		frameCount{0},
		animationFrames{std::make_unique<MageAnimationFrame[]>(frameCount)}
	{};

	MageAnimation(uint32_t address);

	uint16_t TilesetIndex() const;
	uint16_t FrameCount() const;
	MageAnimationFrame AnimationFrame(uint32_t index) const;
	uint32_t Size() const;
}; //class MageAnimation

class MageEntityTypeAnimationDirection
{
private:
	uint16_t typeId;
	uint8_t type;
	uint8_t renderFlags;
public:
	MageEntityTypeAnimationDirection() : typeId{0},
		type{0},
		renderFlags{0}
	{};

	MageEntityTypeAnimationDirection(uint32_t address);

	uint16_t TypeID() const;
	uint8_t Type() const;
	uint8_t RenderFlags() const;
	bool FlipX() const;
	bool FlipY() const;
	bool FlipDiag() const;
	uint32_t Size() const;

}; //class MageEntityTypeAnimationDirection

class MageEntityTypeAnimation
{
private:
	MageEntityTypeAnimationDirection north;
	MageEntityTypeAnimationDirection east;
	MageEntityTypeAnimationDirection south;
	MageEntityTypeAnimationDirection west;
public:
	MageEntityTypeAnimation() : 
		north{MageEntityTypeAnimationDirection(0)},
		east{MageEntityTypeAnimationDirection(0)},
		south{MageEntityTypeAnimationDirection(0)},
		west{MageEntityTypeAnimationDirection(0)}
	{};

	MageEntityTypeAnimation(uint32_t address);

	MageEntityTypeAnimationDirection North() const;
	MageEntityTypeAnimationDirection East() const;
	MageEntityTypeAnimationDirection South() const;
	MageEntityTypeAnimationDirection West() const;
	uint32_t Size() const;

}; //class MageEntityTypeAnimation

class MageEntityType
{
private:
	char name[17];
	uint8_t paddingA;
	uint8_t paddingB;
	uint8_t paddingC;
	uint8_t animationCount;
	std::unique_ptr<MageEntityTypeAnimation[]> entityTypeAnimations;
public:
	MageEntityType() : name{0},
		paddingA{0},
		paddingB{0},
		paddingC{0},
		animationCount{0},
		entityTypeAnimations{std::make_unique<MageEntityTypeAnimation[]>(animationCount)}
	{};

	MageEntityType(uint32_t address);

	std::string Name() const;
	//padding is not used, not making getter functions.
	uint8_t AnimationCount() const;
	MageEntityTypeAnimation EntityTypeAnimation(uint32_t index) const;
	uint32_t Size() const;
}; //class MageEntityType

class MageRom
{
private:
	//Stores the current map's index value
	uint32_t currentMapIndex;

	//this is a pointer to the Player Entity
	MageEntity* playerEntity;

	//these header objects store the header information for all datasets on the ROM,
	//including address offsets for each item, and the length of the item in memory.
	MageHeader mapHeader;
	MageHeader tileHeader;
	MageHeader animationHeader;
	MageHeader entityTypeHeader;
	MageHeader entityHeader;
	MageHeader imageHeader;

	//this is where the current map data from the ROM is stored.
	MageMap map;

	//this is an array of the tileset data on the ROM.
	//each entry is an indexed tileset.
	std::unique_ptr<MageTileset[]> tilesets;

	//this is an array of the animation data on the ROM
	//each entry is an indexed animation.
	std::unique_ptr<MageAnimation[]> animations;

	//this is an array of the entity types on the ROM
	//each entry is an indexed entity type.
	std::unique_ptr<MageEntityType[]> entityTypes;

	//this is the hackable array of entities that are on the current map
	//the data contained within is the data that can be hacked in the hex editor.
	std::unique_ptr<MageEntity[]> entities;

	//this is an arrya storing the most current data needed to draw entities
	//on the screen in their current animation state.
	std::unique_ptr<MageEntityRenderableData[]> entityRenderableData;
public:
	//when the MageRom is created, it will populate all the above variables from ROM.
	MageRom();

	//returns the size in memory of the MageRom object.
	uint32_t Size() const;

	//this will return a specific MageTileset object by index.
	const MageTileset& Tile(uint32_t index) const;

	//this will return the current map object.
	const MageMap& Map() const;

	//this will load a map to be the current map.
	void LoadMap(uint16_t index);

	//this will fill in an entity structure's data from ROM
	MageEntity LoadEntity(uint32_t address);

	//this will render the map onto the screen.
	void DrawMap(uint8_t layer, int32_t camera_x, int32_t camera_y) const;

	//this calculates the relevant info to be able to draw an entity based on the
	//current state of the data in MageRom and stores the info in entityRenderableData
	void getEntityRenderableData(uint32_t index);

	//this will update the current entities based on the current frame data
	void UpdateEntities(uint32_t delta_time);

	//this will draw the entities over the current state of the screen
	void DrawEntities(int32_t cameraX, int32_t cameraY);

}; //class MageRom

// extern MageDataMemoryAddresses dataMemoryAddresses;
// extern MageTileset *allTilesets;
// extern uint32_t mapIndex;
// extern uint32_t currentMapIndex;
// extern MageMap currentMap;
// extern MageTileset *currentMapTilesets;
// extern MageEntity *currentMapEntities;
// extern uint16_t *currentMapEntityFrameTicks;
// extern MageAnimationFrame *currentMapEntityFrames;
// extern MageEntity **currentMapEntitiesSortedByRenderOrder;
// extern uint8_t *data;

// uint32_t load_data_headers();

/*void load_tilesets_headers(
	MageTileset *tilesetPointer,
	uint32_t tilesetIndex
);*/

// void load_all_tilesets();
// void load_map_headers(uint32_t incomingMapIndex);
// void correct_entity_type_endians();
// void correct_animation_endians();
// void correct_entity_endians();

// MageEntityType* get_entity_type_by_index(uint8_t index);
// uint16_t* get_image_by_index(uint8_t index);
// MageTileset* get_tileset_by_id(uint8_t index);

#endif //_MAGE_ROM_H
