/*

MIT License

Copyright (c) 2021-2024 CHiPs44 <chips44@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

This file is part of the Raspberry Pi Pico VGA board HAL for the HAGL graphics library:
 - https://github.com/CHiPs44/hagl_pico_vgaboard
 - https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#ifndef _PICO_VGABOARD_TILES_H
#define _PICO_VGABOARD_TILES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define LOG_DEPTH_2_COLORS 0
#define LOG_DEPTH_4_COLORS 1
#define LOG_DEPTH_16_COLORS 2
#define LOG_DEPTH_256_COLORS 3
#define LOG_DEPTH_65536_COLORS 4

/*
           N=90
            ^
            |
  W=180 <---+---> E=0
            |
            v
          S=270
*/
#define SCROLL_EAST 0
#define SCROLL_NORTH_EAST 45
#define SCROLL_NORTH 90
#define SCROLL_NORTH_WEST 135
#define SCROLL_WEST 180
#define SCROLL_SOUTH_WEST 215
#define SCROLL_SOUTH 270
#define SCROLL_SOUTH_EAST 315

    /**
     * @brief Tile set: collection of TWxTH tiles arranged in an SWxSH grid
     */
    typedef struct _tileset_t
    {
        /** @brief tile width in pixels, must be aligned to bytes, e.g. 8, 16, 24, 32 for 1bpp, ... */
        uint8_t tile_w;
        /** @brief tile height in pixels */
        uint8_t tile_h;
        /** @brief log_depth: 0 for 1 bpp, 1 for 2 bpp, 2 for 4 bpp, 3 for 8 bpp, 4 for 16 bpp */
        uint8_t log_depth;
        /** @brief size in bytes, f(width, height, depth) */
        uint16_t tile_size;
        /** @brief color palette for all tiles in tile set */
        uint8_t *palette;
        // Bitmap of up to 256 tiles arranged in rows and columns
        /** @brief row count */
        uint8_t rows;
        /** @brief column count */
        uint8_t cols;
        /** @brief data bitmap, tile #n will be at tile_size * (cols * (n / rows) + n % cols) (= tile_size * n?)*/
        uint8_t *data;
    } tileset_t;

    /**
     * @brief Validate & initialize a tile set
     *
     * @param tileset pointer to tile set structure
     * @return true if tile set is valid
     * @return false if not
     */
    bool tileset_init(tileset_t *tileset);

    /**
     * @brief Compute tile size
     *
     * @param w tile width
     * @param h tile_height
     * @param log_depth 1/2/4/8/16 bits depth
     * @return uint16_t tile size in bytes
     */
    uint16_t tileset_get_tile_size(uint16_t w, uint16_t h, uint8_t log_depth);

    /**
     * @brief Convert text tilemap to binary tilemap
     *
     * @param cols map width
     * @param rows map height
     * @param text map text representation
     * @param data map
     * @param text_to_tile conversion table: wchar at index x will be interpreted as tile #x
     */
    void tilemap_convert_text_to_data(uint8_t cols, uint8_t rows, wchar *text, uint8_t *data, wchar_t *text_to_tile);

    /**
     * @brief Tile map: MWxWH grid of tiles from a tile set, displayed on a rectangular region of screen
     */
    typedef struct _tilemap_t
    {
        // definition: up to 256x256 tiles
        tileset_t *tileset;
        uint8_t rows;
        uint8_t cols;
        uint8_t *data;
        // screen area (in pixels)
        uint16_t screen_x;
        uint16_t screen_y;
        uint16_t screen_w;
        uint16_t screen_h;
        // "macro" scroll position & speed (in tiles)
        int8_t scroll_col;
        int8_t scroll_row;
        uint8_t delta_row;
        uint8_t delta_col;
        // "micro" scroll position (in pixels)
        int8_t pixel_x;
        int8_t pixel_y;
        uint8_t delta_x;
        uint8_t delta_y;
    } tilemap_t;

    /**
     * @brief Validate & initialize tile map
     *
     * @param tilemap pointer to tile map structure
     * @return true if tile map is valid
     * @return false if not
     */
    bool tilemap_init(tilemap_t *tilemap);

    /**
     * @brief Scroll tile map in one of 8 directions
     * 
     * @param tilemap pointer to tile map structure
     * @param direction one of 8 directions SCROLL_XXXX
     * @return true if blocked in that direction
     * @return false if not
     */
    bool tilemap_scroll(tilemap_t *tilemap, uint8_t direction);

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_TILES_H */
