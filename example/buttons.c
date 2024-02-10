/* SPDX-License-Identifier: MIT-0 */

bool buttons_demo_first;
bool buttons_demo_next;

void buttons_handle()
{
    buttons_demo_first = false;
    buttons_demo_next = false;
#if !PICO_NO_HARDWARE
    pico_vgaboard_buttons_handle_input();
    // Short A => next demo
    if (pico_vgaboard_buttons_states[0].event == PICO_VGABOARD_BUTTONS_EVENT_SHORT)
    {
#if PICO_VGABOARD_DEBUG
        printf("NEXT DEMO!\n");
#endif
        buttons_demo_next = true;
        pico_vgaboard_buttons_states[0].event = PICO_VGABOARD_BUTTONS_EVENT_NONE;
        return;
    }
    // Medium A => first demo
    if (pico_vgaboard_buttons_states[0].event == PICO_VGABOARD_BUTTONS_EVENT_MEDIUM)
    {
#if PICO_VGABOARD_DEBUG
        printf("FIRST DEMO!\n");
#endif
        buttons_demo_first = true;
        pico_vgaboard_buttons_states[0].event = PICO_VGABOARD_BUTTONS_EVENT_NONE;
        return;
    }
    // Short B => next palette
    if (pico_vgaboard_buttons_states[1].event == PICO_VGABOARD_BUTTONS_EVENT_SHORT)
    {
#if PICO_VGABOARD_DEBUG
        printf("NEXT PALETTE!\n");
#endif
        palette_index = (palette_index + 1) % palette_count;
        pico_vgaboard_set_palette(palette_table[palette_index].palette);
        palette_name = palette_table[palette_index].name;
        pico_vgaboard_buttons_states[1].event = PICO_VGABOARD_BUTTONS_EVENT_NONE;
    }
    // Medium B => first palette
    if (pico_vgaboard_buttons_states[1].event == PICO_VGABOARD_BUTTONS_EVENT_MEDIUM)
    {
#if PICO_VGABOARD_DEBUG
        printf("FIRST PALETTE!\n");
#endif
        palette_index = 0;
        pico_vgaboard_set_palette(palette_table[palette_index].palette);
        palette_name = palette_table[palette_index].name;
        pico_vgaboard_buttons_states[1].event = PICO_VGABOARD_BUTTONS_EVENT_NONE;
    }
    // Short C => change borders, if any
    if (pico_vgaboard_buttons_states[2].event == PICO_VGABOARD_BUTTONS_EVENT_SHORT)
    {
        if (pico_vgaboard->has_margins)
        {
#if PICO_VGABOARD_DEBUG
            printf("RANDOM BORDERS!\n");
#endif
            /* Random border colors in letterbox mode instead of default black ones */
            pico_vgaboard->border_color_top = (rand() % 65536) & ~PICO_SCANVIDEO_ALPHA_MASK;
            pico_vgaboard->border_color_left = (rand() % 65536) & ~PICO_SCANVIDEO_ALPHA_MASK;
            pico_vgaboard->border_color_bottom = (rand() % 65536) & ~PICO_SCANVIDEO_ALPHA_MASK;
            pico_vgaboard->border_color_right = (rand() % 65536) & ~PICO_SCANVIDEO_ALPHA_MASK;
        }
        pico_vgaboard_buttons_states[2].event = PICO_VGABOARD_BUTTONS_EVENT_NONE;
    }
    // Medium C => black borders, if any
    if (pico_vgaboard_buttons_states[2].event == PICO_VGABOARD_BUTTONS_EVENT_MEDIUM)
    {
        if (pico_vgaboard->has_margins)
        {
#if PICO_VGABOARD_DEBUG
            printf("BLACK BORDERS!\n");
#endif
            /* Random border colors in letterbox mode instead of default black ones */
            pico_vgaboard->border_color_top = 0;
            pico_vgaboard->border_color_left = 0;
            pico_vgaboard->border_color_bottom = 0;
            pico_vgaboard->border_color_right = 0;
        }
        pico_vgaboard_buttons_states[2].event = PICO_VGABOARD_BUTTONS_EVENT_NONE;
    }
#endif
}
