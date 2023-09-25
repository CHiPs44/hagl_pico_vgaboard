# Images

Let's talk a little about the images provided with this example code.

## Sources

### Cat

- <https://commons.wikimedia.org/wiki/File:Cat_in_the_garden_1.JPG>
- CC-BY-SA <https://creativecommons.org/licenses/by-sa/4.0/deed.en>

### Cow

- <https://commons.wikimedia.org/wiki/File:Cow_female_black_white.jpg>
- Public domain

### Dog

- <https://commons.wikimedia.org/wiki/File:German-shepherd-4040871920.jpg>
- Public domain

## Howto make 160x120 PNG images

- open GIMP
- import Sweetie 16 palette (cf. <https://lospec.com/palette-list/sweetie-16>, get .gpl palette file)
- open original JPEG image
- make 4:3 selection
- copy it
- paste as new image
- save as `xxx.xcf`
- resize to 160x120
- Image -> Mode -> Indexed...
- select Sweetie 16 palette
- don't let "Remove unused and duplicate colors from colormap" checked
- set Dithering to none
- convert
- export as `xxx.png` (reset settings to defaults)
- close `xxx.xcf` without saving to be able to make other dimensions or palettes

NB: `cat-320x240.png` is different, it is 320x240 (!) and uses an optimized palette by changing the indexed mode settings

## How to make `xxx.h` header files

Even if i'm a bit ashamed about code quality, `convert.py` does the job...

- open `convert.py`
- change image name
- save it
- launch `./convert.py > xxx.h` from the shell
- edit `xxx.h` at your will and copy hagl_bitmap struct from another file

`EOF`
