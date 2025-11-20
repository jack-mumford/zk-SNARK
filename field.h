#ifndef FIELD_H
#define FIELD_H
#include <stdint.h>
typedef struct { uint64_t p; } field_t;
uint64_t f_add(field_t F, uint64_t x, uint64_t y);
uint64_t f_sub(field_t F, uint64_t x, uint64_t y);
uint64_t f_mul(field_t F, uint64_t x, uint64_t y);
uint64_t f_inv(field_t F, uint64_t x);
uint64_t f_pow(field_t F, uint64_t base, uint64_t exp);
#endif
