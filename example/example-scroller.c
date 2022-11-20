// /* Scroller */
// wchar_t *scroller_text = 
//     L"Yo lamers! "
//     L"This is CHiPs44 speaking through the awesome VGA demo board for the mighty Raspberry Pi Pico and the magnificent HAGL library... "
//     L"Hi to Tuupola, Kilograham, Memotech Bill, DarkElvenAngel & Rumbledethumps!"
//     L"                                        ";
// wchar_t scroller_buffer[40];
// uint16_t scroller_len = wcslen(scroller_text);
// uint16_t scroller_pos = 0;
// uint16_t scroller_x = 0;
// uint16_t scroller_y = HEIGHT / 2 - 13 - 4;

// /* Draw scroller */
// if (counter % 100 == 0) {
//     swprintf(scroller_buffer,sizeof(scroller_buffer),L"%40s", scroller_text);
//     wprintf(L"%d/%d %s\n", scroller_pos, scroller_len, scroller_buffer);
//     hagl_put_text(hagl_backend, scroller_buffer, scroller_x, scroller_y, 11, font8x13);
//     scroller_pos += 1;
//     // Wrap?
//     if (scroller_pos > scroller_len) {
//         scroller_pos = 0;
//     }
// }
