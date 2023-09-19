#!/usr/bin/env python3

from PIL import Image
import numpy as np

# cf. https://stackoverflow.com/questions/52307290/what-is-the-difference-between-images-in-p-and-l-mode-in-pil

# name = "cat"
# name = "cow"
# name = "dog"
# name = "cat-8bpp-256x192"
# name = "cow-8bpp-256x192"
# name = "dog-8bpp-256x192"
identifier = name.replace("-", "_")
image = Image.open(name + ".png")
# print("/*")
# print(image)
# print("*/")
# print("")

# # Spit out palette even if most images use Sweetie 16 palette
# palette = np.array(image.getpalette()).reshape(256, 3)
# print("const uint16_t pico_vgaboard_palette_4bpp_{0}[16] = {{".format(identifier))
# for i in range(16):
#     print(
#         "    /* {0:02} */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x{1:02x}, 0x{2:02x}, 0x{3:02x}),".format(
#             i, palette[i][0], palette[i][1], palette[i][2])
#     )
# print("};")
# print("")

# Now the pixels
pixels = np.array(image.getchannel(0))
# print(pixels)
width = image.width
height = image.height
print("const hagl_color_t {0}_pixels[{1}] = {{".format(
    identifier, width * height))
for y in range(height):
    for x in range(width):
        c = pixels[y, x]
        print("0x{0:x}, ".format(c), end='')
    print("")
print("};")
print("")

# EOF
