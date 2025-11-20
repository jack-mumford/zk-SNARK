#include "snark_sim.h"

/*
 * Task 1: Build the witness values for the circuit.
 *
 * Goal: compute three field elements (c, s, y) from inputs (a, b, w).
 * - Use ONLY the field operations provided in field.c:
 *     f_add(F, x, y), f_sub(F, x, y), f_mul(F, x, y)
 * - Do not use raw +, -, * for field math.
 * - All results must be reduced modulo p via the field ops.
 *
 * Steps (high-level):
 *   1) Derive an intermediate representing a combination of the inputs.
 *   2) Derive a second intermediate representing another combination.
 *   3) Use the selector to choose/blend between the two as required.
 *   4) Store the three final witness values in *pc, *ps, *py.
 *
 * Replace the temporary zeros with your computed values.
 */
void build_witness(field_t F, uint64_t a, uint64_t b, uint64_t w,
                   uint64_t* pc, uint64_t* ps, uint64_t* py) {
    /* TODO: implement using f_add/f_sub/f_mul; write into *pc, *ps, *py */
    *pc = 0;
    *ps = 0;
    *py = 0;
}

/*
 * Task 2: Linear combination at the hidden evaluation point (tau).
 *
 * Goal: given:
 *   - evals[7]: field values that correspond to basis evaluations at tau,
 *   - z[7]:     the witness vector (constant term first),
 * compute the field inner product of these two length-7 vectors.
 *
 * Requirements:
 *   - Initialize an accumulator to the additive identity in the field.
 *   - For each position j in 0..6:
 *       * multiply the corresponding entries (field multiplication),
 *       * add into the accumulator (field addition).
 *   - Return the accumulated field value.
 *
 * Replace the temporary return with your accumulated result.
 */
uint64_t lincombo(field_t F, const uint64_t evals[7], const uint64_t z[7]) {
    /* TODO: implement looped field multiply-and-accumulate */
    return 0;
}

/*
 * Task 3: Compute the quotient evaluation H at the hidden point.
 *
 * Goal: form a value that, when multiplied by a provided divisor evaluation,
 *       reproduces a specific difference of two terms built from inputs.
 *
 * High-level plan:
 *   1) Form a product from two provided inputs (field multiplication).
 *   2) Subtract a third provided input from that product (field subtraction).
 *   3) Compute the multiplicative inverse of the provided divisor
 *      using the field inverse helper.
 *   4) Multiply the difference by that inverse to obtain the result.
 *
 * Replace the temporary return with your computed value.
 */
uint64_t compute_H_tau(field_t F, uint64_t A_tau, uint64_t B_tau,
                       uint64_t C_tau, uint64_t t_tau) {
    /* TODO: implement product-minus-term, inverse, and final multiply */
    return 0;
}
