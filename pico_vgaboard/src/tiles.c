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

#include <string.h>
#include <wchar.h>

#include "../include/tiles.h"

bool tileset_init(tileset_t *tileset)
{
    // Ensure there is at least 1 tile and at most 256 tiles in this tile set
    if (tileset->rows == 0 || tileset->cols == 0 || tileset->rows * tileset->cols > 256)
        return false;
    // Ensure tile width & height are "reasonable"
    if (tileset->tile_w < 8 || tile_w > 32)
        return false;
    if (tileset->tile_h < 8 || tile_h > 32)
        return false;
    // Ensure tile width in pixels is a multiple number of bytes
    switch (tileset->log_depth)
    {
    case LOG_DEPTH_2_COLORS: // 8 bits per pixel / 1 byte for 8 pixels
        // => width is 8, 16, 24, 32, ...
        if (tileset->tile_w % 8 != 0)
            return false;
        break;
    case LOG_DEPTH_4_COLORS: // 2 bits per pixel / 1 byte for 4 pixels
        // => width is 4, 8, 12, 16, ...
        if (tileset->tile_w % 4 != 0)
            return false;
        break;
    case LOG_DEPTH_16_COLORS: // 4 bits per pixel / 1 byte for 2 pixels
        // => width is 2, 4, 6, 8, ...
        if (tileset->tile_w % 2 != 0)
            return false;
        break;
    }
    tileset->tile_size = tileset_get_tile_size(tileset->tile_w, tileset->tile_h, tileset->log_depth);
    if (tileset->tile_size == 0)
        return false;
    tileset->tile_stride = tileset_get_tile_stride(tileset->tile_w, tileset->tile_size, tileset->rows, tileset->cols);
    return true;
}

uint16_t tileset_get_tile_size(uint16_t w, uint16_t h, uint8_t log_depth)
{
    switch (log_depth)
    {
    case LOG_DEPTH_2_COLORS:
        if ((w * h) % 8 != 0)
            return 0;
        return (w * h) / 8;
    case LOG_DEPTH_4_COLORS:
        if ((w * h) % 4 != 0)
            return 0;
        return (w * h) / 4;
    case LOG_DEPTH_16_COLORS:
        if ((w * h) % 2 != 0)
            return 0;
        return (w * h) / 2;
    case LOG_DEPTH_256_COLORS:
        return (w * h) / 1;
    case LOG_DEPTH_65536_COLORS:
        return (w * h) * 2;
    default:
        return 0;
    }
}

void tilemap_convert_text_to_data(uint8_t cols, uint8_t rows, wchar *text, uint8_t *data, wchar_t *text_to_tile)
{
    for (uint8_t row = 0; row < rows; row++)
    {
        for (uint8_t col = 0; col < cols; col++)
        {
            wchar_t c = text[row * cols + col];
            uint8_t t = 0;
            for (int i = 0; i < wcslen(text_to_tile); i++)
            {
                if (c == text_to_tile[i])
                {
                    t = i;
                    break;
                }
            }
            data[row * cols + col] = t;
        }
    }
}

bool tilemap_init(tilemap_t *tilemap)
{
    // Initialize scrolling at "home"
    tilemap->scroll_row = 0;
    tilemap->scroll_col = 0;
    tilemap->pixel_x = 0;
    tilemap->pixel_y = 0;
    // Move by 1 tile & 1 pixel by default
    tilemap->delta_row = 1;
    tilemap->delta_col = 1;
    tilemap->delta_x = 1;
    tilemap->delta_y = 1;
    return true;
}

bool tilemap_scroll(tilemap_t *tilemap, uint8_t direction)
{
    int16_t row, col;
    int8_t pixel_x, pixel_y;
    // uint8_t row_delta, uint8_t col_delta, uint8_t pixel_x_delta, uint8_t pixel_y_delta;

    switch (direction)
    {
    case SCROLL_EAST:
        // "micro" scrolling
        tilemap->pixel_x += tilemap->delta_x;
        if (tilemap->pixel_x < tilemap->tileset->tile_w)
        {
            return false;
        }
        tilemap->pixel_x = 0;
        // "macro" scrolling
        tilemap->scroll_row = tilemap->scroll_row + tilemap->row_delta;
        if (row < tilemap->rows)
        {
            return false;
        }
        tilemap->scroll_row = tilemap->rows - 1;
        return true;
    case SCROLL_NORTH_EAST:
        break;
    case SCROLL_NORTH:
        // "micro" scrolling
        tilemap->pixel_y -= tilemap->delta_y;
        if (tilemap->pixel_y > 0)
        {
            return false;
        }
        tilemap->pixel_y = 0;
        // "macro" scrolling
        tilemap->scroll_col = tilemap->scroll_col + tilemap->col_delta;
        if (tilemap->scroll_row < tilemap->rows)
        {
            return false;
        }
        tilemap->scroll_col = tilemap->rows - 1;
        return true;
    case SCROLL_NORTH_WEST:
        break;
    case SCROLL_WEST:
        // "micro" scrolling
        tilemap->pixel_x -= tilemap->delta_x;
        if (tilemap->pixel_x > -tilemap->tileset->tile_w)
        {
            return false;
        }
        tilemap->pixel_x = 0;
        // "macro" scrolling
        row = tilemap->scroll_row - row_delta;
        if (row > 0)
        {
            return false;
        }
        row = 0;
        return true;
    case SCROLL_SOUTH_WEST:
        break;
    case SCROLL_SOUTH:
        break;
    case SCROLL_SOUTH_EAST:
        break;
    default:
        return false;
    }
    return true;
}

void tilemap_render_bitmap_line_8(tilemap_t *tilemap, unit16_t line_number, uint8_t *line_data)
{
    tileset_t *tileset = tilemap->tileset;
    int16_t tile_index;
    int8_t row, col, *tile_data;
    int16_t i;

    i = 0;
    while (i < tilemap->screen_w)
    {
        // use only "macro" scroll position (in tiles) for now
        row = (tilemap->scroll_row) % tilemap->rows;
        col = (tilemap->scroll_col) % tilemap->cols;
        tile_number = tilemap->data[row * tilemap->rows + col];
        tile_data = tileset->data[tile_number];
        tile_byte = *(tile_data + row *)
                        line_data[i] = tile_byte;
    }
}

void tilemap_render_bitmap_line_16(tilemap_t *tilemap, unit16_t line_number, uint16_t *line_data)
{
    panic("TODO: tilemap_render_bitmap_line_16!");
}

int16_t tilemap_render_scanline(tilemap_t *tilemap, uint16_t scanline_number, uint32_t *scanline_data, uint16_t scanline_data_max)
{
    int16_t scanline_data_used;

    if (tilemap->screen_y < scanline_number || tilemap->screen_y + tilemap->screen_h >= scanline_number)
    {
        scanline_data[0] = COMPOSABLE_RAW_1P | (0 << 16);
        scanline_data[1] = COMPOSABLE_EOL_SKIP_ALIGN;
        scanline_data_used = 2;
    }
    else
    {
        tilemap_render_bitmap_line(tilemap, , line_data);
    }

    return scanline_data_used;
}