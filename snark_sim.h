#ifndef SNARK_SIM_H
#define SNARK_SIM_H
#include <stdint.h>
#include "field.h"
void build_witness(field_t F,uint64_t a,uint64_t b,uint64_t w,uint64_t* pc,uint64_t* ps,uint64_t* py);
uint64_t lincombo(field_t F,const uint64_t evals[7],const uint64_t z[7]);
uint64_t compute_H_tau(field_t F,uint64_t A_tau,uint64_t B_tau,uint64_t C_tau,uint64_t t_tau);
#endif
