/* SPDX-License-Identifier: MIT-0 */

BGAR5515 get_dark_color(bool darkest)
{
    if (pico_vgaboard->depth > 8)
    {
        return PICO_SCANVIDEO_PIXEL_FROM_RGB8(0, 0, 0);
    }
    
}