# fontx2-fonts

FONTX2 fonts to be used with HAGL

## font8x8: IBM VGA BIOS with Unicode additions

From: <https://github.com/dhepper/font8x8/>

Patched with 00-1f & 80-9f from BIOS.F08 from <https://github.com/viler-int10h/vga-text-mode-fonts/>

`BIOS.F08.h` generated from `BIOS.F08` with:

```bash
xxd -i -c8 BIOS.F08 BIOS.F08.h
```

`font8x8tofontx2.c` is an ugly piece of code, but it does the job.

Build it, run it then generate header file with:

```bash
gcc -Wall -I /usr/lib/gcc/x86_64-linux-gnu/11/include/ font8x8tofontx2.c -o font8x8tofontx2
./font8x8tofontx2
xxd -i -c16 font8x8.fnt font8x8.h
```

Voilà!
