#include "printf.h"

static uint32_t printf_number(uint64_t num, uint8_t base){
    uint32_t len = 1;
    if(num >= base){
        len += printf_number(num / base, base);
    }
    uint64_t rem = num % base;
    miniuart_send_C(((rem > 9)? (rem - 10) + 'a' : rem + '0'));
    return len;
}

static uint32_t printf_d(const char *fmt, va_list args, uint32_t len){
    int32_t num = va_arg(args, int32_t);
    if(num < 0){
        len += miniuart_send_C('-');
        num *= -1;
    }
    len += printf_number(num, 10);
    return vprintf(fmt, args, len);
}

static uint32_t printf_ld(const char *fmt, va_list args, uint32_t len){
    int64_t num = va_arg(args, int64_t);
    if(num < 0){
        len += miniuart_send_C('-');
        num *= -1;
    }
    len += printf_number(num, 10);
    return vprintf(fmt, args, len);
}

static uint32_t printf_u(const char *fmt, va_list args, uint32_t len){
    uint32_t num = va_arg(args, uint32_t);
    len += printf_number(num, 10);
    return vprintf(fmt, args, len);
}

static uint32_t printf_lu(const char *fmt, va_list args, uint32_t len){
    uint64_t num = va_arg(args, uint64_t);
    len += printf_number(num, 10);
    return vprintf(fmt, args, len);
}

static uint32_t printf_x(const char *fmt, va_list args, uint32_t len){
    uint32_t num = va_arg(args, uint32_t);
    len += miniuart_send_S("0x");
    len += printf_number(num, 16);
    return vprintf(fmt, args, len);
}

static uint32_t printf_lx(const char *fmt, va_list args, uint32_t len){
    uint64_t num = va_arg(args, uint64_t);
    len += miniuart_send_S("0x");
    len += printf_number(num, 16);
    return vprintf(fmt, args, len);
}

static uint32_t printf_c(const char *fmt, va_list args, uint32_t len){
    int32_t c = va_arg(args, int32_t);
    len += miniuart_send_C(c);
    return vprintf(fmt, args, len);
}

static uint32_t printf_s(const char *fmt, va_list args, uint32_t len){
    const char *str = va_arg(args, const char *);
    len += miniuart_send_S(str);
    return vprintf(fmt, args, len);
}

static uint32_t vprintf(const char *fmt, va_list args, uint32_t len){
    char c, cc;
    while(*fmt){
        c = *fmt++;
        if(c != '%'){
            len += miniuart_send_C(c);
        }else{
            c = *fmt++;
            switch(c){
            case 'd':
                return printf_d(fmt, args, len);
            case 'u':
                return printf_u(fmt, args, len);
            case 'x':
                return printf_x(fmt, args, len);
            case 'c':
                return printf_c(fmt, args, len);
            case 's':
                return printf_s(fmt, args, len);
            case 'p':
                return printf_lx(fmt, args, len);
            case 'l':
                cc = *fmt++;
                switch(cc){
                case 'd':
                    return printf_ld(fmt, args, len);
                case 'u':
                    return printf_lu(fmt, args, len);
                case 'x':
                    return printf_lx(fmt, args, len);
                default:
                    va_arg(args, int32_t);
                    len += miniuart_send_C('%');
                    len += miniuart_send_C(c);
                    len += miniuart_send_C(cc);
                    continue;
                }
            default:
                va_arg(args, int32_t);
                len += miniuart_send_C('%');
                len += miniuart_send_C(c);
                break;
            }
        }
    }
    return len;
}

uint32_t printf(const char *fmt, ...){
    va_list args;
    uint32_t r;
    va_start(args, fmt);
    r = vprintf(fmt, args, 0);
    va_end(args);
    return r;
}
