#ifndef ARM_NEON_COMPLEX_H
#define ARM_NEON_COMPLEX_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Element-wise multiplies two complex input arrays, and stores the result in another array.
 *
 * @param result    Pointer to the output array.
 * @param x         Pointer to the first input array.
 * @param y         Pointer to the second input array.
 * @param count     The number of complex numbers in both input arrays, i.e. half the number of floats in each array. This should be a multiple of 8.
 */
void arm_neon_complex_multiply(float* result, float* x, float* y, unsigned int count);

/**
 * @brief Element-wise multiplies two complex input arrays, adds the product to another array, and stores the result in an output array.
 *
 * @param result    Pointer to the output array.
 * @param accum     Pointer to the input array which is to be added after multiplication.
 * @param x         Pointer to the first input array to be multiplied.
 * @param y         Pointer to the second input array to be multiplied.
 * @param count     The number of complex numbers in all input arrays, i.e. half the number of floats in each array. This should be a multiple of 8.
 */
void arm_neon_complex_multiply_accumulate(float* result, float* accum, float* x, float* y, unsigned int count);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: ARM_NEON_COMPLEX_H */
