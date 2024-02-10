/* SPDX-License-Identifier: MIT-0 */

bool images_init()
{
    if (DEPTH == 8)
    {
        return images_8bpp_init();
    }
    return images_4bpp_init();
}

void images_draw()
{
    if (DEPTH == 8)
    {
        images_8bpp_draw();
        return;
    }
    images_4bpp_draw();
}

void images_done()
{
    if (DEPTH == 8)
    {
        images_8bpp_done();
        return;
    }
    images_4bpp_done();
}
