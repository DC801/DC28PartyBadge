#include "common.h"
#include <inttypes.h>

bool running = true;
FrameBuffer *mage_canvas;
uint32_t lastTime;
uint32_t now;
uint32_t delta_time;

uint16_t _uint16Native = 0xFF00;
uint8_t *_uint16TopBit = (uint8_t *)&_uint16Native;
bool _needsSwap = *_uint16TopBit == 0x00;

void ceU2 (uint16_t *value) {
    if (_needsSwap) {
        *value = __builtin_bswap16(*value);
    }
}
void ceU2Buf (uint16_t *buf, size_t bufferSize) {
    if (_needsSwap) {
        for (size_t i = 0; i < bufferSize; i++) {
            buf[i] = __builtin_bswap16(buf[i]);
        }
    }
}
void ceU4 (uint32_t *value) {
    if (_needsSwap) {
        *value = __builtin_bswap32(*value);
    }
}
void ceU4Buf (uint32_t *buf, size_t bufferSize) {
    if (_needsSwap) {
        for (size_t i = 0; i < bufferSize; i++) {
            buf[i] = __builtin_bswap32(buf[i]);
        }
    }
}

void handle_input () {
    #ifdef DC801_DESKTOP
    SDL_Event e;
    if (application_quit != 0)
    {
        running = false;
    }

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            running = false;
            break;
        }

        if (e.type == SDL_KEYDOWN)
        {
            if (
                e.key.keysym.sym == SDLK_ESCAPE
                || e.key.keysym.scancode == SDL_SCANCODE_Q
            )
            {
                running = false;
                break;
            }
        }
    }

    nrf_delay_ms(5);
    #endif

    #ifdef DC01_EMBEDDED
    app_usbd_event_queue_process();
    #endif
}

void mage_game_loop () {
    now = millis();
    delta_time = now - lastTime;

    mage_canvas->clearScreen(RGB(0,0,255));
    mage_canvas->drawHorizontalLine(10, 20, 100, RGB(0,255,0));

    mage_canvas->drawImageFromFile(
        64,
        64,
        32,
        32,
        "MAGE/game.dat",
        0,
        0,
        32
    );
    mage_canvas->drawImageFromFile(
        64,
        96,
        32,
        32,
        "MAGE/exa.dat",
        0,
        0,
        32
    );
    mage_canvas->blt();

    lastTime = now;
}

GameDataMemoryAddresses dataMemoryAddresses = {};

uint32_t count_with_offsets (
    uint8_t *data,
    uint32_t **count,
    uint32_t **offsets,
    uint32_t **lengths
) {
    uint32_t offset = 0;
    printf("count_with_offsets START ----------------------\n");

    printf("count_with_offsets data: %p\n", data);
    *count = (uint32_t *) data + offset;
    printf("count_with_offsets *count: %p\n", *count);
    offset += 1;
    ceU4(*count);
    printf("count_with_offsets Data Size: %" PRIu32 "\n", **count);

    *offsets = (uint32_t *) data + offset;
    printf("count_with_offsets *offsets: %p\n", *offsets);
    offset += **count;
    printf("count_with_offsets offsets before swizzle: %" PRIu32 "\n", **offsets);
    ceU4Buf(*offsets, **count);
    printf("count_with_offsets offsets after swizzle:  %" PRIu32 "\n", **offsets);

    *lengths = (uint32_t *) data + offset;
    printf("count_with_offsets *lengths: %p\n", *lengths);
    offset += **count;
    printf("count_with_offsets lengths before swizzle: %" PRIu32 "\n", **lengths);
    ceU4Buf(*lengths, **count);
    printf("count_with_offsets lengths after swizzle:  %" PRIu32 "\n", **lengths);

    printf("count_with_offsets END ----------------------\n");
    return offset * 4; // but return the offset in # of bytes
}

void load_data_headers (uint8_t *data) {
    uint32_t offset = 8; // seek past identifier
    offset += count_with_offsets(
        data + offset,
        &dataMemoryAddresses.mapCount,
        &dataMemoryAddresses.mapOffsets,
        &dataMemoryAddresses.mapLengths
    );
    offset += count_with_offsets(
        data + offset,
        &dataMemoryAddresses.tilesetCount,
        &dataMemoryAddresses.tilesetOffsets,
        &dataMemoryAddresses.tilesetLengths
    );
    offset += count_with_offsets(
        data + offset,
        &dataMemoryAddresses.imageCount,
        &dataMemoryAddresses.imageOffsets,
        &dataMemoryAddresses.imageLengths
    );
    printf("data: %p\n", data);

    printf("dataMemoryAddresses.mapCount: %p\n", dataMemoryAddresses.mapCount);
    printf("dataMemoryAddresses.mapOffsets[0]: %p\n", dataMemoryAddresses.mapOffsets);
    printf("dataMemoryAddresses.mapLengths[0]: %p\n", dataMemoryAddresses.mapLengths);
    printf("dataMemoryAddresses.tilesetCount: %p\n", dataMemoryAddresses.tilesetCount);
    printf("dataMemoryAddresses.tilesetOffsets[0]: %p\n", dataMemoryAddresses.tilesetOffsets);
    printf("dataMemoryAddresses.tilesetLengths[0]: %p\n", dataMemoryAddresses.tilesetLengths);
    printf("dataMemoryAddresses.imageCount: %p\n", dataMemoryAddresses.imageCount);
    printf("dataMemoryAddresses.imageOffsets[0]: %p\n", dataMemoryAddresses.imageOffsets);
    printf("dataMemoryAddresses.imageLengths[0]: %p\n", dataMemoryAddresses.imageLengths);

    printf("dataMemoryAddresses.mapCount: %" PRIu32 "\n", *dataMemoryAddresses.mapCount);
    printf("dataMemoryAddresses.mapOffsets[0]: %" PRIu32 "\n", *dataMemoryAddresses.mapOffsets);
    printf("dataMemoryAddresses.mapLengths[0]: %" PRIu32 "\n", *dataMemoryAddresses.mapLengths);
    printf("dataMemoryAddresses.tilesetCount: %" PRIu32 "\n", *dataMemoryAddresses.tilesetCount);
    printf("dataMemoryAddresses.tilesetOffsets[0]: %" PRIu32 "\n", *dataMemoryAddresses.tilesetOffsets);
    printf("dataMemoryAddresses.tilesetLengths[0]: %" PRIu32 "\n", *dataMemoryAddresses.tilesetLengths);
    printf("dataMemoryAddresses.imageCount: %" PRIu32 "\n", *dataMemoryAddresses.imageCount);
    printf("dataMemoryAddresses.imageOffsets[0]: %" PRIu32 "\n", *dataMemoryAddresses.imageOffsets);
    printf("dataMemoryAddresses.imageLengths[0]: %" PRIu32 "\n", *dataMemoryAddresses.imageLengths);
    printf("offset: %" PRIu32 "\n", offset);
}

int MAGE() {
    char dataFilePath[] = "MAGE/game.dat";
    uint32_t dataFileSize = util_sd_file_size(dataFilePath);
    uint8_t data[dataFileSize];
    util_sd_load_file(
        dataFilePath,
        data,
        dataFileSize
    );
    char identifier[9];
    identifier[8] = 0; // null terminate it manually
    memcpy(identifier, data, 8);
    uint8_t indentifierCompare = strcmp(
        "MAGEGAME",
        identifier
    );
    if (indentifierCompare == 0) {
        printf("It's the right type!\n");
    } else {
        printf("It's the WRONG type!\n");
        exit(1);
    }

    load_data_headers(data);
    mage_canvas = p_canvas();
    lastTime = millis();
    while (running)
    {
        handle_input();
        mage_game_loop();
    }
    return 0;
}
