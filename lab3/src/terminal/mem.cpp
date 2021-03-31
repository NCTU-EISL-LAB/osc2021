#include <stdint.h>
extern "C"
void *memset(void *dst0, int c0, size_t length) {
    uint8_t c = c0;
    for (size_t i = 0; i < length; i++) {
        ((int8_t*)dst0)[i] = c;
    }
    return dst0;
}
