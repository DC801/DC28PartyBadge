#ifndef _MAGE_HEX_H
#define _MAGE_HEX_H

#include "mage.h"
#include "../../modules/led.h"
#include "../../../fonts/practical8pt7b.h"
#include "FrameBuffer.h"

extern FrameBuffer *mage_canvas;

extern uint8_t *hexEditorState;
extern void toggle_hex_editor();
extern void render_hex_editor();

#endif //_MAGE_HEX_H
