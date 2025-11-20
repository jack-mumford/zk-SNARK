#include "field.h"
static inline uint64_t red(field_t F, __int128 z){return (uint64_t)(z % F.p);}
uint64_t f_add(field_t F, uint64_t x, uint64_t y){uint64_t s=x+y;return (s>=F.p||s<x)?s-F.p:s;}
uint64_t f_sub(field_t F, uint64_t x, uint64_t y){return (x>=y)?(x-y):(F.p-(y-x));}
uint64_t f_mul(field_t F, uint64_t x, uint64_t y){__int128 z=(__int128)x*y;return red(F,z);}
uint64_t f_pow(field_t F,uint64_t b,uint64_t e){uint64_t r=1%F.p;while(e){if(e&1)r=f_mul(F,r,b);b=f_mul(F,b,b);e>>=1;}return r;}
uint64_t f_inv(field_t F, uint64_t x){return f_pow(F,x,F.p-2);}