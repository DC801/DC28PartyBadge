#ifndef _MAGE_GAME_CONTROL
#define _MAGE_GAME_CONTROL

#include "mage_defines.h"
#include "mage_header.h"
#include "mage_map.h"
#include "mage_tileset.h"
#include "mage_animation.h"
#include "mage_entity_type.h"
#include "mage_geometry.h"
#include "mage_color_palette.h"
#include <vector>

#define MAGE_COLLISION_SPOKE_COUNT 6

// color palette corruption detection - requires much ram, can only be run on desktop
#ifdef DC801_DESKTOP
#define LOG_COLOR_PALETTE_CORRUPTION_INSIDE_MAGE_GAME(value) verifyAllColorPalettes((value));
#endif //DC801_DESKTOP
#ifdef DC801_EMBEDDED
#define LOG_COLOR_PALETTE_CORRUPTION_INSIDE_MAGE_GAME(value) //(value)
#endif //DC801_EMBEDDED

/*
The MageGameControl object handles several important tasks. It's basically the
core of the entire MAGE() game, and contains all the important variables that
track the state of the game.

MageGameControl's first important function is to read all relevant data from
the ROM chip into RAM, and also to have helper functions to read additional
ROM data on demand as needed when updating variables.

MageGameControl also tracks the state of all hackable entities in the game, and
interprets the data contained in the array when deciding what needs to be drawn
to the screen.

Finally, MageGameControl handles the actual act of updating the state of the
game based on input data and rendering it all to the screen every frame.
*/

class MageGameControl
{
private:
	//these header objects store the header information for all datasets on the ROM,
	//including address offsets for each item, and the length of the item in memory.
	MageHeader mapHeader;
	MageHeader tilesetHeader;
	MageHeader animationHeader;
	MageHeader entityTypeHeader;
	MageHeader entityHeader;
	MageHeader geometryHeader;
	MageHeader scriptHeader;
	MageHeader portraitHeader;
	MageHeader dialogHeader;
	MageHeader serialDialogHeader;
	MageHeader colorPaletteHeader;
	MageHeader stringHeader;
	MageHeader saveFlagHeader;
	MageHeader variableHeader;
	MageHeader imageHeader;

	//used to verify whether a save is compatible with game data
	uint32_t engineVersion;
	uint32_t scenarioDataCRC32;
	uint32_t scenarioDataLength;

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

	//this is an array storing the most current data needed to draw entities
	//on the screen in their current animation state.
	std::unique_ptr<MageEntityRenderableData[]> entityRenderableData;

	//this is an array of all the colorPalettes objects in the ROM
	std::unique_ptr<MageColorPalette[]> colorPalettes;

	//a couple of state variables for tracking player movement:
	float mageSpeed;
	bool isMoving;

	Point playerVelocity = {
		.x = 0,
		.y = 0,
	};
	Point adjustedCameraPosition= {
		.x = 0,
		.y = 0,
	};

	uint8_t filteredMapLocalEntityIds[MAX_ENTITIES_PER_MAP] = {0};
	uint8_t mapLocalEntityIds[MAX_ENTITIES_PER_MAP] = {0};

public:
	//this is the hackable array of entities that are on the current map
	//the data contained within is the data that can be hacked in the hex editor.
	std::unique_ptr<MageEntity[]> entities;

	//this is the index value of where the playerEntity is located within
	//the entities[] array and also the offset to it from hackableDataAddress
	uint8_t playerEntityIndex;

	uint8_t currentSaveIndex;
	MageSaveGame currentSave;

	//this lets us make it so that inputs stop working for the player
	bool playerHasControl;
	bool playerHasHexEditorControl;
	bool playerHasHexEditorControlClipboard;
	bool isLEDControlEnabled;
	bool isCollisionDebugOn;
	bool isEntityDebugOn;
	uint8_t filteredEntityCountOnThisMap;
	bool cameraShaking = false;
	float cameraShakePhase = 0;
	uint8_t cameraShakeAmplitude = 0;
	uint8_t cameraFollowEntityId = NO_PLAYER;
	Point cameraPosition = {
		.x = 0,
		.y = 0,
	};

	//when the MageGameControl object is created, it will populate all the above variables from ROM.
	MageGameControl();

	//returns the size in memory of the MageGameControl object.
	uint32_t Size() const;

	void setCurrentSaveToFreshState();
	void readSaveFromRomIntoRam(
		bool silenceErrors = false
	);
	void saveGameSlotSave();
	void saveGameSlotErase(uint8_t slotIndex);
	void saveGameSlotLoad(uint8_t slotIndex);

	//this will return a specific MageTileset object by index.
	const MageTileset& Tileset(uint32_t index) const;

	//this will return the current map object.
	MageMap& Map();

	//this will fill in an entity structure's data from ROM
	MageEntity LoadEntity(uint32_t address);

	//this takes map data by index and fills all the variables in the map object:
	void PopulateMapData(uint16_t index);

	//this will load a map to be the current map.
	void LoadMap(uint16_t index);

	//this handles inputs that apply in ALL game states. That includes when
	//the hex editor is open, when it is closed, when in any menus, etc.
	void applyUniversalInputs();

	//this takes input information and moves the playerEntity around
	//If there is no playerEntity, it just moves the camera freely.
	void applyGameModeInputs(uint32_t deltaTime);

	void applyCameraEffects(uint32_t deltaTime);

	//this will check in the direction the player entity is facing and start
	//an on_interact script for an entity if any qualify.
	void handleEntityInteract(
		bool hack = false
	);

	//this will render the map onto the screen.
	void DrawMap(uint8_t layer);

	//the functions below will validate specific properties to see if they are valid.
	//these are used to ensure that we don't get segfaults from using the hacked entity data.
	uint16_t getValidMapId(uint16_t mapId);
	uint8_t getValidPrimaryIdType(uint8_t primaryIdType);
	uint16_t getValidTilesetId(uint16_t tilesetId);
	uint16_t getValidTileId(uint16_t tileId, uint16_t tilesetId);
	uint16_t getValidAnimationId(uint16_t animationId);
	uint16_t getValidAnimationFrame(uint16_t animationFrame, uint16_t animationId);
	uint16_t getValidEntityTypeId(uint16_t entityTypeId);
	MageEntityType* getValidEntityType(uint16_t entityTypeId);
	uint16_t getValidMapLocalScriptId(uint16_t scriptId);
	uint16_t getValidGlobalScriptId(uint16_t scriptId);
	uint8_t  getValidEntityTypeAnimationId(uint8_t entityTypeAnimationId, uint16_t entityTypeId);
	MageEntityAnimationDirection getValidEntityTypeDirection(
		MageEntityAnimationDirection direction
	);
	MageEntityAnimationDirection getRelativeEntityTypeDirection(
		MageEntityAnimationDirection direction
	);
	MageEntityAnimationDirection updateDirectionAndPreserveFlags(
		MageEntityAnimationDirection desired,
		MageEntityAnimationDirection previous
	);
	MageGeometry getGeometryFromMapLocalId(uint16_t mapLocalGeometryId);
	MageGeometry getGeometryFromGlobalId(uint16_t globalGeometryId);
	MageColorPalette* getValidColorPalette(uint16_t colorPaletteId);
	uint8_t getFilteredEntityId(uint8_t mapLocalEntityId) const;
	uint8_t getMapLocalEntityId(uint8_t filteredEntityId) const;
	MageEntity* getEntityByMapLocalId(uint8_t mapLocalEntityId) const;
	MageEntityRenderableData* getEntityRenderableDataByMapLocalId(uint8_t mapLocalEntityId);
	std::string getString(
		uint16_t stringId,
		int16_t mapLocalEntityId
	);
	MageTileset* getValidTileset(uint16_t tilesetId);
	std::string getEntityNameStringById(int8_t mapLocalEntityId);
	uint32_t getImageAddress(uint16_t imageId);
	uint32_t getPortraitAddress(uint16_t portraitId);
	uint32_t getDialogAddress(uint16_t dialogId);
	uint32_t getSerialDialogAddress(uint16_t serialDialogId);

	//this returns the address offset for a specific script Id:
	uint32_t getScriptAddressFromGlobalScriptId(uint32_t scriptId);

	//this calculates the relevant info to be able to draw an entity based on the
	//current state of the data in MageGameControl and stores the info in entityRenderableData
	void updateEntityRenderableData(
		uint8_t mapLocalEntityId,
		bool skipTilesetCheck = false
	);

	void updateEntityRenderableBoxes(
		MageEntityRenderableData *data,
		const MageEntity *entity,
		const MageTileset *tileset
	) const;

	//this will update the current entities based on the current state of their state variables
	void UpdateEntities(uint32_t deltaTime);

	void computeEntityYAxisSort(
		uint8_t *entitySortOrder,
		uint8_t filteredEntityCountOnThisMap
	);

	//this will draw the entities over the current state of the screen
	void DrawEntities();

	//this will draw the current map's geometry over the current state of the screen
	void DrawGeometry();

	Point getPushBackFromTilesThatCollideWithPlayer();

	void getRenderableStateFromAnimationDirection(
		MageEntityRenderableData *data,
		const MageEntity *entity,
		const MageEntityTypeAnimationDirection *animationDirection
	);

	void copyNameToAndFromPlayerAndSave(bool intoSaveRam) const;

	std::vector<std::string> getEntityNamesInRoom() const;

	#ifdef DC801_DESKTOP
	void verifyAllColorPalettes(const char* errorTriggerDescription);
	#endif //DC801_DESKTOP

	uint16_t entityTypeCount();
	uint16_t animationCount();
	uint16_t tilesetCount();

	void logAllEntityScriptValues(const char *string);
}; //class MageGameControl

#endif //_MAGE_GAME_CONTROL
