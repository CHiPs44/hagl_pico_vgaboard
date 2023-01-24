#if HAGL_HAL_COLOR_BITS==8
#include "example-8bits.c"
#elif HAGL_HAL_COLOR_BITS==16
#include "example-16bits.c"
#else
#include "example-minimal.c"
#endif
