/* SPDX-License-Identifier: MIT-0 */

/**
 * @brief FONTX font with explicit width, height & name
 * 
 */
typedef struct _font_t {
    const uint8_t *fontx;
    const uint8_t w;
    const uint8_t h;
    const wchar_t *name;
} font_t;

/* EOF */
