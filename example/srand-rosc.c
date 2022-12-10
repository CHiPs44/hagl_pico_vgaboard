// cf. https://forums.raspberrypi.com/viewtopic.php?t=302960#p1815162
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"
// Von Neumann extractor: From the input stream, his extractor took bits, two at a time (first and second, then third and fourth, and so on). If the two bits matched, no output was generated. If the bits differed, the value of the first bit was output. 
void srand_rosc()
{
  uint32_t random = 0;
  uint32_t random_bit;
  volatile uint32_t *rnd_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
  for (int k = 0; k < 32; k++) {
    while (1) {
      random_bit = (*rnd_reg) & 1;
      if (random_bit != ((*rnd_reg) & 1)) break;
    }
    random = (random << 1) | random_bit;
  }
  srand(random);
}
