# Build

## Build with real hardware

```bash
mkdir build
cd build
cmake ..
reset && make -j$(nproc) && cp -v hagl_pico_vgaboard_example.uf2 /media/$USER/RPI-RP2/
```

## Build with `pico-host-sdl`

```bash
mkdir build2
cd build2
cmake -DPICO_PLATFORM=host -DPICO_SDK_PRE_LIST_DIRS=~/src/pico-host-sdl -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```
