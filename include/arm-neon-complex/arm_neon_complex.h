#ifndef ARM_NEON_COMPLEX_H
#define ARM_NEON_COMPLEX_H

#ifdef __cplusplus
extern "C" {
#endif

void arm_neon_complex_multiply(float* result, float* x, float* y, unsigned int count);
void arm_neon_complex_multiply_accumulate(float* result, float* accum, float* x, float* y, unsigned int count);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: ARM_NEON_COMPLEX_H */
