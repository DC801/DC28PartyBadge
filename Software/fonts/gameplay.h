#ifndef GAMEPLAY_H
#define GAMEPLAY_H

static const uint8_t gameplay5pt7bBitmaps[]  = {
  0xFF, 0xF3, 0xC0, 0xDE, 0xF6, 0x6D, 0xFF, 0xFB, 0x66, 0xDF, 0xFF, 0xB6,
  0x6C, 0x30, 0xCF, 0xFF, 0x83, 0xEF, 0xC3, 0xCF, 0xFF, 0xCC, 0xE3, 0xCC,
  0x30, 0xE1, 0x8E, 0x1C, 0x33, 0xC6, 0xFE, 0xFE, 0xC0, 0xF7, 0x77, 0xC6,
  0xC6, 0xFE, 0x7C, 0xFC, 0x7F, 0xCC, 0xCC, 0xCF, 0x70, 0xEF, 0x33, 0x33,
  0x3F, 0xE0, 0xE0, 0x30, 0x63, 0xFF, 0xF3, 0x06, 0x0C, 0x00, 0xF4, 0xFF,
  0xFC, 0xF0, 0x06, 0x0C, 0x30, 0xC1, 0x8E, 0x18, 0x30, 0xC0, 0x7D, 0xFF,
  0x1E, 0x3C, 0x78, 0xF1, 0xFF, 0x7C, 0xEF, 0x33, 0x33, 0x33, 0x30, 0xFD,
  0xFC, 0x1B, 0xFF, 0xD8, 0x30, 0x7F, 0xFE, 0xFD, 0xFC, 0x19, 0xF3, 0xC0,
  0xC1, 0xFF, 0xFC, 0xC7, 0x8F, 0x1F, 0xF7, 0xE0, 0xC1, 0x83, 0x06, 0xFF,
  0xFF, 0x07, 0xE7, 0xE0, 0xF1, 0xFF, 0x7C, 0x7F, 0xFF, 0x07, 0xEF, 0xF8,
  0xF1, 0xFF, 0x7C, 0xFF, 0xFC, 0x18, 0x71, 0xC6, 0x0C, 0x18, 0x30, 0x7D,
  0xFF, 0x1F, 0xF7, 0xD8, 0xF1, 0xFF, 0x7C, 0x7D, 0xFF, 0x1F, 0xF7, 0xE0,
  0xC1, 0x83, 0x06, 0xF0, 0xF0, 0xF0, 0xF8, 0xFF, 0xF1, 0xFF, 0xFC, 0xFF,
  0xFF, 0x1F, 0xFF, 0xCF, 0xFF, 0x80, 0xFF, 0xFC, 0x00, 0x0F, 0xFF, 0xC0,
  0xFF, 0xF1, 0xFF, 0xFC, 0xFF, 0xFF, 0x1F, 0xFF, 0xCF, 0xFF, 0x80, 0xFF,
  0xF0, 0xCF, 0x38, 0xC0, 0x0C, 0x30, 0xFF, 0xF1, 0xFF, 0xFC, 0xFF, 0xFF,
  0x1F, 0xFF, 0xCF, 0xFF, 0x80, 0x7D, 0xFF, 0x1F, 0xFF, 0xF8, 0xF1, 0xE3,
  0xC6, 0xFD, 0xFF, 0x1F, 0xFF, 0xD8, 0xF1, 0xFF, 0xFC, 0xFF, 0xFF, 0x06,
  0x0C, 0x18, 0x31, 0xFF, 0x7C, 0xFD, 0xFF, 0x1E, 0x3C, 0x78, 0xF1, 0xFF,
  0xFC, 0x7F, 0xFF, 0x07, 0xCF, 0x98, 0x30, 0x7F, 0x7E, 0x7F, 0xFF, 0x07,
  0xCF, 0x98, 0x30, 0x60, 0xC0, 0xFF, 0xFF, 0xC0, 0xCE, 0xCF, 0xC3, 0xC3,
  0xFF, 0x7E, 0xC7, 0x8F, 0x1F, 0xFF, 0xF8, 0xF1, 0xE3, 0xC6, 0xFF, 0xFF,
  0xC0, 0x1E, 0x3C, 0x18, 0x30, 0x78, 0xF1, 0xFF, 0x7C, 0xC7, 0x8F, 0x1F,
  0xFF, 0xD8, 0xF1, 0xE3, 0xC6, 0xC1, 0x83, 0x06, 0x0C, 0x18, 0x30, 0x7F,
  0x7E, 0xC3, 0xE7, 0xE7, 0xF7, 0xDB, 0xDB, 0xCB, 0xC3, 0xC3, 0xC7, 0xCF,
  0x9F, 0xBD, 0x7B, 0xF3, 0xE7, 0xC6, 0x7D, 0xFF, 0x1E, 0x3C, 0x78, 0xF1,
  0xFF, 0x7C, 0xFD, 0xFF, 0x1F, 0xFF, 0xD8, 0x30, 0x60, 0xC0, 0x7D, 0xFF,
  0x1E, 0x3C, 0x78, 0xF7, 0xFF, 0x7E, 0x30, 0xFD, 0xFF, 0x1F, 0xFF, 0xD8,
  0xF1, 0xE3, 0xC6, 0xFF, 0xFF, 0x07, 0xE7, 0xE0, 0xF1, 0xFF, 0x7C, 0xFF,
  0xFC, 0xC1, 0x83, 0x06, 0x0C, 0x18, 0x30, 0xC7, 0x8F, 0x1E, 0x3C, 0x78,
  0xF1, 0xFF, 0x7C, 0xC7, 0x8F, 0x1E, 0x3C, 0x6D, 0x9B, 0x1C, 0x10, 0xC3,
  0xC3, 0xCB, 0xDB, 0xDB, 0xF7, 0xE7, 0xE7, 0xC3, 0xC6, 0xCD, 0xB1, 0xC3,
  0x87, 0x0E, 0x36, 0x67, 0x8C, 0xC7, 0x8F, 0x1F, 0xF7, 0xE0, 0xF1, 0xFF,
  0x7C, 0xFF, 0xFC, 0x30, 0xC1, 0x86, 0x0C, 0x30, 0xFF, 0xFC, 0xFF, 0xCC,
  0xCC, 0xCF, 0xF0, 0xC0, 0xC1, 0x81, 0xC1, 0x83, 0x83, 0x83, 0x06, 0xFF,
  0x33, 0x33, 0x3F, 0xF0, 0xFF, 0xF1, 0xFF, 0xFC, 0xFF, 0xFF, 0x1F, 0xFF,
  0xCF, 0xFF, 0x80, 0xFF, 0xFC, 0xFF, 0xF1, 0xFF, 0xFC, 0xFF, 0xFF, 0x1F,
  0xFF, 0xCF, 0xFF, 0x80, 0x7D, 0xFF, 0x1F, 0xFF, 0xF8, 0xF1, 0xE3, 0xC6,
  0xFD, 0xFF, 0x1F, 0xFF, 0xD8, 0xF1, 0xFF, 0xFC, 0xFF, 0xFF, 0x06, 0x0C,
  0x18, 0x31, 0xFF, 0x7C, 0xFD, 0xFF, 0x1E, 0x3C, 0x78, 0xF1, 0xFF, 0xFC,
  0x7F, 0xFF, 0x07, 0xCF, 0x98, 0x30, 0x7F, 0x7E, 0x7F, 0xFF, 0x07, 0xCF,
  0x98, 0x30, 0x60, 0xC0, 0xFF, 0xFF, 0xC0, 0xCE, 0xCF, 0xC3, 0xC3, 0xFF,
  0x7E, 0xC7, 0x8F, 0x1F, 0xFF, 0xF8, 0xF1, 0xE3, 0xC6, 0xFF, 0xFF, 0xC0,
  0x1E, 0x3C, 0x18, 0x30, 0x78, 0xF1, 0xFF, 0x7C, 0xC7, 0x8F, 0x1F, 0xFF,
  0xD8, 0xF1, 0xE3, 0xC6, 0xC1, 0x83, 0x06, 0x0C, 0x18, 0x30, 0x7F, 0x7E,
  0xC3, 0xE7, 0xE7, 0xF7, 0xDB, 0xDB, 0xCB, 0xC3, 0xC3, 0xC7, 0xCF, 0x9F,
  0xBD, 0x7B, 0xF3, 0xE7, 0xC6, 0x7D, 0xFF, 0x1E, 0x3C, 0x78, 0xF1, 0xFF,
  0x7C, 0xFD, 0xFF, 0x1F, 0xFF, 0xD8, 0x30, 0x60, 0xC0, 0x7D, 0xFF, 0x1E,
  0x3C, 0x78, 0xF7, 0xFF, 0x7E, 0x30, 0xFD, 0xFF, 0x1F, 0xFF, 0xD8, 0xF1,
  0xE3, 0xC6, 0xFF, 0xFF, 0x07, 0xE7, 0xE0, 0xF1, 0xFF, 0x7C, 0xFF, 0xFC,
  0xC1, 0x83, 0x06, 0x0C, 0x18, 0x30, 0xC7, 0x8F, 0x1E, 0x3C, 0x78, 0xF1,
  0xFF, 0x7C, 0xC7, 0x8F, 0x1E, 0x3C, 0x6D, 0x9B, 0x1C, 0x10, 0xC3, 0xC3,
  0xCB, 0xDB, 0xDB, 0xF7, 0xE7, 0xE7, 0xC3, 0xC6, 0xCD, 0xB1, 0xC3, 0x87,
  0x0E, 0x36, 0x67, 0x8C, 0xC7, 0x8F, 0x1F, 0xF7, 0xE0, 0xF1, 0xFF, 0x7C,
  0xFF, 0xFC, 0x30, 0xC1, 0x86, 0x0C, 0x30, 0xFF, 0xFC, 0x3B, 0xD8, 0xCC,
  0x31, 0x8F, 0x38, 0xFF, 0xFF, 0xC0, 0xE7, 0x8C, 0x61, 0x98, 0xDE, 0xE0,
  0x67, 0xFF, 0x78 };

static const GFXglyph gameplay5pt7bGlyphs[]  = {
  {     0,   0,   0,   3,    0,    1 },   // 0x20 ' '
  {     0,   2,   9,   3,    0,   -8 },   // 0x21 '!'
  {     3,   5,   3,   6,    0,   -8 },   // 0x22 '"'
  {     5,   7,   9,   7,    0,   -8 },   // 0x23 '#'
  {    13,   6,  12,   7,    0,  -10 },   // 0x24 '$'
  {    22,   7,   9,   8,    0,   -8 },   // 0x25 '%'
  {    30,   8,   9,   8,    0,   -8 },   // 0x26 '&'
  {    39,   2,   3,   3,    0,   -8 },   // 0x27 '''
  {    40,   4,   9,   4,    0,   -8 },   // 0x28 '('
  {    45,   4,   9,   8,    3,   -8 },   // 0x29 ')'
  {    50,   3,   1,   3,   -1,   -8 },   // 0x2A '*'
  {    51,   7,   7,   8,    0,   -7 },   // 0x2B '+'
  {    58,   2,   3,   3,    0,   -1 },   // 0x2C ','
  {    59,   7,   2,   8,    0,   -5 },   // 0x2D '-'
  {    61,   2,   2,   3,    0,   -1 },   // 0x2E '.'
  {    62,   7,   9,   8,    0,   -8 },   // 0x2F '/'
  {    70,   7,   9,   8,    0,   -8 },   // 0x30 '0'
  {    78,   4,   9,   5,    0,   -8 },   // 0x31 '1'
  {    83,   7,   9,   8,    0,   -8 },   // 0x32 '2'
  {    91,   7,   9,   8,    0,   -8 },   // 0x33 '3'
  {    99,   7,   9,   8,    0,   -8 },   // 0x34 '4'
  {   107,   7,   9,   8,    0,   -8 },   // 0x35 '5'
  {   115,   7,   9,   8,    0,   -8 },   // 0x36 '6'
  {   123,   7,   9,   8,    0,   -8 },   // 0x37 '7'
  {   131,   7,   9,   8,    0,   -8 },   // 0x38 '8'
  {   139,   7,   9,   8,    0,   -8 },   // 0x39 '9'
  {   147,   2,   6,   3,    0,   -7 },   // 0x3A ':'
  {   149,   2,   7,   3,    0,   -7 },   // 0x3B ';'
  {   151,   9,   9,   9,    0,   -8 },   // 0x3C '<'
  {   162,   7,   6,   8,    0,   -7 },   // 0x3D '='
  {   168,   9,   9,   9,    0,   -8 },   // 0x3E '>'
  {   179,   6,   9,   7,    0,   -8 },   // 0x3F '?'
  {   186,   9,   9,   9,    0,   -8 },   // 0x40 '@'
  {   197,   7,   9,   8,    0,   -8 },   // 0x41 'A'
  {   205,   7,   9,   8,    0,   -8 },   // 0x42 'B'
  {   213,   7,   9,   8,    0,   -8 },   // 0x43 'C'
  {   221,   7,   9,   8,    0,   -8 },   // 0x44 'D'
  {   229,   7,   9,   8,    0,   -8 },   // 0x45 'E'
  {   237,   7,   9,   8,    0,   -8 },   // 0x46 'F'
  {   245,   8,   9,   9,    0,   -8 },   // 0x47 'G'
  {   254,   7,   9,   8,    0,   -8 },   // 0x48 'H'
  {   262,   2,   9,   3,    0,   -8 },   // 0x49 'I'
  {   265,   7,   9,   8,    0,   -8 },   // 0x4A 'J'
  {   273,   7,   9,   8,    0,   -8 },   // 0x4B 'K'
  {   281,   7,   9,   8,    0,   -8 },   // 0x4C 'L'
  {   289,   8,   9,   9,    0,   -8 },   // 0x4D 'M'
  {   298,   7,   9,   8,    0,   -8 },   // 0x4E 'N'
  {   306,   7,   9,   8,    0,   -8 },   // 0x4F 'O'
  {   314,   7,   9,   8,    0,   -8 },   // 0x50 'P'
  {   322,   7,  10,   8,    0,   -8 },   // 0x51 'Q'
  {   331,   7,   9,   8,    0,   -8 },   // 0x52 'R'
  {   339,   7,   9,   8,    0,   -8 },   // 0x53 'S'
  {   347,   7,   9,   8,    0,   -8 },   // 0x54 'T'
  {   355,   7,   9,   8,    0,   -8 },   // 0x55 'U'
  {   363,   7,   9,   8,    0,   -8 },   // 0x56 'V'
  {   371,   8,   9,   9,    0,   -8 },   // 0x57 'W'
  {   380,   7,  10,   8,    0,   -9 },   // 0x58 'X'
  {   389,   7,   9,   8,    0,   -8 },   // 0x59 'Y'
  {   397,   7,  10,   8,    0,   -9 },   // 0x5A 'Z'
  {   406,   4,   9,   4,    0,   -8 },   // 0x5B '['
  {   411,   7,   9,   8,    0,   -8 },   // 0x5C '\'
  {   419,   4,   9,   5,    0,   -8 },   // 0x5D ']'
  {   424,   9,   9,   9,    0,   -8 },   // 0x5E '^'
  {   435,   7,   2,   8,    0,   -1 },   // 0x5F '_'
  {   437,   9,   9,   9,    0,   -8 },   // 0x60 '`'
  {   448,   7,   9,   8,    0,   -8 },   // 0x61 'a'
  {   456,   7,   9,   8,    0,   -8 },   // 0x62 'b'
  {   464,   7,   9,   8,    0,   -8 },   // 0x63 'c'
  {   472,   7,   9,   8,    0,   -8 },   // 0x64 'd'
  {   480,   7,   9,   8,    0,   -8 },   // 0x65 'e'
  {   488,   7,   9,   8,    0,   -8 },   // 0x66 'f'
  {   496,   8,   9,   9,    0,   -8 },   // 0x67 'g'
  {   505,   7,   9,   8,    0,   -8 },   // 0x68 'h'
  {   513,   2,   9,   3,    0,   -8 },   // 0x69 'i'
  {   516,   7,   9,   8,    0,   -8 },   // 0x6A 'j'
  {   524,   7,   9,   8,    0,   -8 },   // 0x6B 'k'
  {   532,   7,   9,   8,    0,   -8 },   // 0x6C 'l'
  {   540,   8,   9,   9,    0,   -8 },   // 0x6D 'm'
  {   549,   7,   9,   8,    0,   -8 },   // 0x6E 'n'
  {   557,   7,   9,   8,    0,   -8 },   // 0x6F 'o'
  {   565,   7,   9,   8,    0,   -8 },   // 0x70 'p'
  {   573,   7,  10,   8,    0,   -8 },   // 0x71 'q'
  {   582,   7,   9,   8,    0,   -8 },   // 0x72 'r'
  {   590,   7,   9,   8,    0,   -8 },   // 0x73 's'
  {   598,   7,   9,   8,    0,   -8 },   // 0x74 't'
  {   606,   7,   9,   8,    0,   -8 },   // 0x75 'u'
  {   614,   7,   9,   8,    0,   -8 },   // 0x76 'v'
  {   622,   8,   9,   9,    0,   -8 },   // 0x77 'w'
  {   631,   7,  10,   8,    0,   -9 },   // 0x78 'x'
  {   640,   7,   9,   8,    0,   -8 },   // 0x79 'y'
  {   648,   7,  10,   8,    0,   -9 },   // 0x7A 'z'
  {   657,   5,   9,   5,    0,   -8 },   // 0x7B '{'
  {   663,   2,   9,   3,    0,   -8 },   // 0x7C '|'
  {   666,   5,   9,   6,    0,   -8 },   // 0x7D '}'
  {   672,   7,   3,   8,    0,   -6 } }; // 0x7E '~'

static const GFXfont gameplay5pt7b  = {
  (uint8_t  *)gameplay5pt7bBitmaps,
  (GFXglyph *)gameplay5pt7bGlyphs,
  0x20, 0x7E, 12 };

// Approx. 1347 bytes

#endif
