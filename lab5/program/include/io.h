#pragma once

#include "syscall.h"

static inline void uart_put_1(const char *str) { uart_put(str, strlen(str)); }

class io {
public:
    io operator<<(uint64_t val) {
        char buffer[25];
        u64toa(val, buffer, sizeof(buffer));
        uart_put_1(buffer);
        return *this;
    }
    io operator<<(const char* str) {
        uart_put_1(str);
        return *this;
    }
    io operator<<(void* ptr) {
        char buffer[20];
        uint64_t number = (uint64_t) ptr;
        u64tohex(number, buffer, sizeof(buffer));
        uart_put_1(buffer);
        return *this;
    }
};
