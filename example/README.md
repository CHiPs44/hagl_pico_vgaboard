# Build

## Build with real hardware

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

## Build with `pico-host-sdl`

```bash
mkdir build2
cd build2
cmake -DPICO_PLATFORM=host -DPICO_SDK_PRE_LIST_DIRS=~/src/pico-host-sdl ..
make -j$(nproc)
```
