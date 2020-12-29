#include "common.h"
#include "EngineROM.h"
#include "EnginePanic.h"
#include "FrameBuffer.h"
#include "fonts/Monaco9.h"
#include "games/mage/mage_defines.h"



#ifdef DC801_EMBEDDED
#include "qspi.h"
extern QSPI qspiControl;

bool EngineROM_Init(void)
{
	//first check the SD card to see if it has a game.dat to check against the ROM chip:
	const char filename[] = MAGE_GAME_DAT_PATH;
	FIL gameDat;
	FRESULT result;
	UINT count;
	char gameDatTimestampSD[ENGINE_ROM_MAGIC_STRING_LENGTH + ENGINE_ROM_TIMESTAMP_LENGTH+1] {0};
	char gameDatTimestampROM[ENGINE_ROM_MAGIC_STRING_LENGTH + ENGINE_ROM_TIMESTAMP_LENGTH+1] {0};
	uint32_t gameDatFilesize = 0;

	//get the game.dat filesize:
	gameDatFilesize = util_sd_file_size(filename);
	char debugString[128];
	sprintf(
		debugString,
		"gameDatFilesize: %08u",
		gameDatFilesize
	);
	p_canvas()->printMessage(
		debugString,
		Monaco9,
		0xffff,
		16,
		64
	);
	p_canvas()->blt();

	// Open magegame.dat file on SD card
	result = f_open(&gameDat, filename, FA_READ | FA_OPEN_EXISTING);
	if (result != FR_OK) {
		return false;
	}

	//get the gameDatTimestampSD from the SD card game.dat:
	result = f_lseek(&gameDat, 0);
	if (result != FR_OK) {
		return false;
	}
	//read from the file into the gameDatTimestampSD buffer:
	result = f_read(&gameDat, (uint8_t *)gameDatTimestampSD, ENGINE_ROM_MAGIC_STRING_LENGTH + ENGINE_ROM_TIMESTAMP_LENGTH, &count);
	if (result != FR_OK) {
		return false;
	}

	//get the gameDatTimestampROM from the ROM chip:
	if(EngineROM_Read(
		0,
		ENGINE_ROM_MAGIC_STRING_LENGTH + ENGINE_ROM_TIMESTAMP_LENGTH,
		(uint8_t *)gameDatTimestampROM
	) != ENGINE_ROM_MAGIC_STRING_LENGTH + ENGINE_ROM_TIMESTAMP_LENGTH) {
		ENGINE_PANIC("Failed to read timestamp from ROM");
	}

	//compare the two timestamps:
	bool timestampsMatch = (strcmp(gameDatTimestampSD, gameDatTimestampROM) == 0);

	if(!timestampsMatch){
		//we need a prompt to see if they want to erase the chip: -Tim
		if(!EngineROM_SD_Copy(gameDatFilesize, gameDat)){
			ENGINE_PANIC("SD Copy Operation was not successful.");
		}
	}
/*
	// Verify magic string is on ROM when we're done:
	if (EngineROM_Magic((const uint8_t*)ENGINE_ROM_MAGIC_STRING, ENGINE_ROM_MAGIC_STRING_LENGTH) != true)
	{
		ENGINE_PANIC("Failed to match Game Magic");
	} */

	//close game.dat file when done:
	result = f_close(&gameDat);

	return true;
}

//this will copy from the file `MAGE/game.dat` on the SD card into the ROM chip.
bool EngineROM_SD_Copy(uint32_t gameDatFilesize, FIL gameDat){
	if(gameDatFilesize > ENGINE_ROM_MAX_DAT_FILE_SIZE){
		ENGINE_PANIC("Your game.dat is larger than 33550336 bytes.\nYou will need to reduce its size to use it\non this board.");
	}
	char debugString[128];
	FRESULT result;
	UINT count;
	p_canvas()->fillRect(
		0,
		96,
		WIDTH,
		96,
		0x0000
	);
	p_canvas()->printMessage(
		"Erasing ROM chip",
		Monaco9,
		0xffff,
		16,
		96
	);
	p_canvas()->blt();
	//then write the entire SD card game.dat file to the ROM chip MAGE_SD_CHUNK_READ_SIZE bytes at a time.
	uint32_t currentAddress = 0;
	uint8_t strBuffer[ENGINE_ROM_SD_CHUNK_READ_SIZE] {0};
	//start by erasing the whole chip:
	if(!qspiControl.chipErase()){
		ENGINE_PANIC("Failed to erase ROM Chip.");
	}
	while(currentAddress < gameDatFilesize){
		uint32_t chunkSize = MIN(ENGINE_ROM_SD_CHUNK_READ_SIZE, (gameDatFilesize - currentAddress));
		//seek to the currentAddress on the SD card:
		result = f_lseek(&gameDat, currentAddress);
		if (result != FR_OK) {
			ENGINE_PANIC("Error seeking to game.dat position\nduring ROM erase procedure.");
		}
		//then read the next set of bytes into the buffer:
		result = f_read(
			&gameDat,
			strBuffer,
			chunkSize,
			&count
		);
		if (result != FR_OK) {
			ENGINE_PANIC("Error reading game.dat from SD card\nduring ROM erase procedure.");
		}
		//write the buffer to the ROM chip:
		uint32_t romPagesToWrite = chunkSize / ENGINE_ROM_WRITE_PAGE_SIZE;
		uint32_t partialPageBytesLeftOver = chunkSize % ENGINE_ROM_WRITE_PAGE_SIZE;
		if(partialPageBytesLeftOver){
			romPagesToWrite += 1;
		}
		for(uint32_t i=0; i<romPagesToWrite; i++)
		{
			//debug_print("Writing ROM Page %d/%d offset from %d", i, romPagesToWrite, currentAddress);
			uint32_t romPageOffset = i*ENGINE_ROM_WRITE_PAGE_SIZE;
			bool shouldUsePartialBytes = (i == (romPagesToWrite - 1)) && (partialPageBytesLeftOver != 0);
			uint32_t writeSize = shouldUsePartialBytes
				? partialPageBytesLeftOver
				: ENGINE_ROM_WRITE_PAGE_SIZE;

			if(i == (romPagesToWrite - 1)){
				debug_print("Write Size at %d is %d", i, writeSize);
			}
			if(EngineROM_Write(
				currentAddress + romPageOffset,
				writeSize,
				strBuffer + romPageOffset
			) != writeSize ) {
				ENGINE_PANIC("Failed to write buffer to ROM chip\nduring ROM erase procedure.");
			}
			//verify that the data was correctly written or return false.
			int64_t verifyResult = EngineROM_Verify(
				currentAddress + romPageOffset,
				writeSize,
				strBuffer + romPageOffset
			);
			if(verifyResult != ENGINE_ROM_VERIFY_SUCCESS){
				debug_print("EngineROM_Verify failed at address %d", verifyResult);
				return false;
			}
		}
		//Debug Print:
		sprintf(
			debugString,
			"currentAddress: %08u\ngameDatFilesize:%08u",
			currentAddress,
			gameDatFilesize
		);
		p_canvas()->fillRect(
			0,
			96,
			WIDTH,
			96,
			0x0000
		);
		p_canvas()->printMessage(
			debugString,
			Monaco9,
			0xffff,
			16,
			96
		);
		p_canvas()->blt();
		currentAddress += chunkSize;
	}
	//print success message:
	p_canvas()->fillRect(
		0,
		96,
		WIDTH,
		96,
		0x0000
	);
	p_canvas()->printMessage(
		"SD -> ROM chip copy success",
		Monaco9,
		0xffff,
		16,
		96
	);
	p_canvas()->blt();
	return true;
}

void EngineROM_Deinit(void) { }

uint32_t EngineROM_Read(uint32_t address, uint32_t length, uint8_t *data)
{
	if (data == NULL)
	{
		ENGINE_PANIC("EngineROM_Read: Null pointer");
	}

	//figure out values for handling unaligned bytes.
	uint32_t truncatedAlignedLength = (length / sizeof(uint32_t)) * sizeof(uint32_t);
	//read in all but the last word if aligned data
	if(!qspiControl.read(data, truncatedAlignedLength, address))
	{
		ENGINE_PANIC("Failed to QSPI Read.");
	}
	uint32_t numUnalignedBytes = length - truncatedAlignedLength;
	if(numUnalignedBytes)
	{
		address += truncatedAlignedLength;
		uint8_t lastWord[sizeof(uint32_t)] = {0, 0, 0, 0};
		//read in the last word of aligned data to its own variable:
		if(!qspiControl.read(lastWord, sizeof(uint32_t), address))
		{
			ENGINE_PANIC("Failed to QSPI Read.");
		}
		//fill in the unaligned bytes only and ignore the rest:
		for(uint8_t i=0; i<numUnalignedBytes; i++){
			data[truncatedAlignedLength+i] = lastWord[i];
		}
	}
	return length;
}

uint32_t EngineROM_Write(uint32_t address, uint32_t length, const uint8_t *data)
{
	if (data == NULL)
	{
		ENGINE_PANIC("EngineROM_Write: Null pointer");
	}

	if(length % sizeof(uint32_t)){
		ENGINE_PANIC("Length of write is not aligned to uint32_t\nYou can't do this, fix whatever is\nsending an unaligned write.");
	}

	if(!qspiControl.write(data, length, address))
	{
		ENGINE_PANIC("Failed to QSPI write");
	}

	return length;
}
#endif

#ifdef DC801_DESKTOP

FILE *romfile = NULL;
#include <errno.h>
#include <string.h>

bool EngineROM_Init(void)
{
	romfile = fopen("MAGE/game.dat", "r+b");

	if (romfile == NULL)
	{
		int error = errno;
		fprintf(stderr, "Error: %s\n", strerror(error));
		ENGINE_PANIC("Failed to load Game Data");
	}
	return true;
}

void EngineROM_Deinit(void)
{
	if (romfile == NULL)
	{
		ENGINE_PANIC("Game Data file is not open");
	}

	if (fclose(romfile) != 0)
	{
		ENGINE_PANIC("Failed to close Game Data file");
	}

	romfile = NULL;
}

uint32_t EngineROM_Read(uint32_t address, uint32_t length, uint8_t *data)
{
	if (romfile == NULL || data == NULL)
	{
		ENGINE_PANIC("Game Data file is not open");
	}

	if (fseek(romfile, address, SEEK_SET) != 0)
	{
		ENGINE_PANIC("Failed to seek into Game Data");
	}

	return fread(data, sizeof(uint8_t), length, romfile);
}

uint32_t EngineROM_Write(uint32_t address, uint32_t length, const uint8_t *data)
{
	if (romfile == NULL || data == NULL)
	{
		ENGINE_PANIC("Game Data file is not open");
	}

	if (fseek(romfile, address, SEEK_SET) != 0)
	{
		ENGINE_PANIC("Failed to seek into Game Data");
	}

	return fwrite(data, sizeof(uint8_t), length, romfile);
}
#endif

int64_t EngineROM_Verify(uint32_t address, uint32_t length, const uint8_t *data)
{
	if (data == NULL)
	{
		ENGINE_PANIC("EngineROM_Verify: Null pointer");
	}

	uint8_t readBuffer[length];
	if(EngineROM_Read(address, length, readBuffer) != length){
		ENGINE_PANIC("Failed to read from Rom in EngineROM_Verify");
	}

	for(uint32_t i=0; i<length; i++){
		if(data[i] != readBuffer[i]){
			debug_print("Address:%d:%d:%d",i,data[i],readBuffer[i]);
			//return address in ROM where memory does not match
			return address+i;
		}
	}

	return ENGINE_ROM_VERIFY_SUCCESS;
}

bool EngineROM_Magic(const uint8_t *magic, uint8_t length)
{
	uint8_t buffer[length];
	uint8_t *ptr = buffer;

	uint32_t read = EngineROM_Verify(0, length, buffer);
	if (read != length)
	{
		//let out the magic smoke
		ENGINE_PANIC("Failed to match Game Data magic");
	}

	for (int i = 0; i < length; i++)
	{
		if (*magic++ != *ptr++)
		{
			return false;
		}
	}

	return true;
}
