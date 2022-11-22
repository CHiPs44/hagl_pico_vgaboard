#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "external/font8x8/font8x8.h"
#include "BIOS.F08.h"

// BUILD:
//  gcc -Wall -I /usr/lib/gcc/x86_64-linux-gnu/11/include/ font8x8tofontx2.c -o font8x8tofontx2

/*************************************************************************************************/

// Constant: font8x8_basic
// Contains an 8x8 font map for unicode points U+0000 - U+007F (basic latin)
// char font8x8_basic[128][8] = {

// Constant: font8x8_control (font8x8_0080)
// Contains an 8x8 font map for unicode points U+0080 - U+009F (C1/C2 control)
// char font8x8_control[32][8] = {

// Constant: font8x8_00A0
// Contains an 8x8 font map for unicode points U+00A0 - U+00FF (extended latin)
// char font8x8_ext_latin[96][8] = {

// Constant: font8x8_block (font8x8_2580)
// Contains an 8x8 font map for unicode points U+2580 - U+259F (block elements)
// char font8x8_block[32][8] = {

// Constant: font8x8_box (font8x8_2500)
// Contains an 8x8 font map for unicode points U+2500 - U+257F (box drawing)
// char font8x8_box[128][8] = {

// Constant: font8x8_0390
// Contains an 8x8 font map for unicode points U+0390 - U+03C9 (greek characters)
// char font8x8_greek[58][8] = {

// Contains an 8x8 font map for unicode points U+3040 - U+309F (Hiragana)
// Constant: font8x8_3040
// char font8x8_hiragana[96][8] = {

// font8x8_basic        128 U+0000 - U+007F (basic latin)
// font8x8_control       32 U+0080 - U+009F (C1/C2 control)
// font8x8_ext_latin     96 U+00A0 - U+00FF (extended latin)
// font8x8_block         32 U+2580 - U+259F (block elements)
// font8x8_box          128 U+2500 - U+257F (box drawing)
// font8x8_greek         58 U+0390 - U+03C9 (greek characters)
// font8x8_hiragana      96 U+3040 - U+309F (Hiragana)

#define FONTNAME "font8x8"
#define NCHARS (128 + 32 + 96 + 32 + 128 + 58 + 96)

/*************************************************************************************************/

typedef struct _block
{
    uint16_t first_char;
    uint16_t last_char;
} block_t;

uint8_t bitmap[NCHARS * 8];
uint16_t offset = 0;
block_t blocks[256];
uint8_t nblocks = 0;

void append_block(uint16_t first_char, uint16_t last_char, uint8_t *source)
{
    printf("Block #%02d: %05d %04x-%04x (%03d) %p\n",
           nblocks, offset, first_char, last_char, last_char - first_char + 1, source);
    for (uint16_t i = 0; i <= last_char - first_char; i += 1)
    {
        for (uint8_t j = 0; j < 8; j += 1)
        {
            // cf. https://github.com/dhepper/font8x8/blob/master/README#L17
            // The least significant bit of each byte corresponds to the first pixel in a
            // row.
            uint8_t byte1 = source[i * 8 + j];
            uint8_t byte2 = 0;
            uint8_t mask1 = 0b00000001;
            uint8_t mask2 = 0b10000000;
            for (uint8_t k = 0; k < 8; k += 1)
            {
                uint8_t bit = byte1 & mask1 ? 1 : 0;
                if (bit)
                {
                    byte2 |= mask2;
                }
                mask1 *= 2;
                mask2 /= 2;
            }
            bitmap[offset++] = byte2;
        }
    }
    printf("offset=%d nchars*8=%d sizeof=%ld\n", offset, NCHARS * 8, sizeof(bitmap));
    blocks[nblocks].first_char = first_char;
    blocks[nblocks].last_char = last_char;
    nblocks += 1;
}

int main()
{
    // Prepare bitmaps & blocks
    append_block(0x0000, 0x007f, (uint8_t *)&font8x8_basic);
    append_block(0x0080, 0x009F, (uint8_t *)&font8x8_control);
    append_block(0x00a0, 0x00ff, (uint8_t *)&font8x8_ext_latin);
    append_block(0x2580, 0x259F, (uint8_t *)&font8x8_block);
    append_block(0x2500, 0x257F, (uint8_t *)&font8x8_box);
    append_block(0x0390, 0x03c9, (uint8_t *)&font8x8_greek);
    append_block(0x3040, 0x309f, (uint8_t *)&font8x8_hiragana);

    // "Patch" 00=>1f and 80=>9f with data from BIOS.F08
    // since these are all zeroes in font8x8
    for (uint16_t i = 0; i <= 31; i += 1)
    {
        for (uint8_t j = 0; j < 8; j += 1)
        {
            bitmap[i*8+j] = BIOS_F08[i*8+j];
            bitmap[(i+0x80)*8+j] = BIOS_F08[(i+0x80)*8+j];
        }
    }

    FILE *fontx = fopen("font8x8.fnt", "wb");

    // FONTX2 HEADER
    // cf. http://elm-chan.org/docs/dosv/fontx_e.html
    //                11111111
    //      012345678901234567
    //      FONTX2FONT8x8 ____
    //      headerfontnamewhtb
    uint8_t header[18];
    header[0] = 'F';
    header[1] = 'O';
    header[2] = 'N';
    header[3] = 'T';
    header[4] = 'X';
    header[5] = '2';
    header[6] = 'F';
    header[7] = 'O';
    header[8] = 'N';
    header[9] = 'T';
    header[10] = '8';
    header[11] = 'X';
    header[12] = '8';
    header[13] = ' ';
    header[14] = 8u;      // Font width WF (dots)
    header[15] = 8u;      // Font height HF (dots)
    header[16] = 1u;      // Type ISO10646 / Unicode (??? Code flag (1:Shift JIS) ???)
    header[17] = nblocks; // Number of code blocks NB
    printf("%d/%d %p %ld\n", nblocks, header[17], &header, sizeof(header));
    fwrite(&header, sizeof(header), 1, fontx);

    // FONTX2 BLOCKS
    // /!\ little endian
    // fwrite(&blocks, sizeof(block_t), nblocks, fontx);
    for (uint8_t block = 0; block < nblocks; block += 1)
    {
        uint8_t first_char_lo = blocks[block].first_char & 0xff;
        uint8_t first_char_hi = blocks[block].first_char >> 8;
        uint8_t last_char_lo = blocks[block].last_char & 0xff;
        uint8_t last_char_hi = blocks[block].last_char >> 8;
        printf("%02d/%02d: %02x %02x %02x %02x\n",
               block, nblocks, first_char_lo, first_char_hi, last_char_lo, last_char_hi);
        fwrite(&first_char_lo, sizeof(first_char_lo), 1, fontx);
        fwrite(&first_char_hi, sizeof(first_char_hi), 1, fontx);
        fwrite(&last_char_lo, sizeof(last_char_lo), 1, fontx);
        fwrite(&last_char_hi, sizeof(last_char_hi), 1, fontx);
    }

    // FONTX2 BITMAP
    fwrite(&bitmap, sizeof(bitmap), 1, fontx);

    fclose(fontx);
    return 0;
}

/* EOF */
