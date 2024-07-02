#include <immintrin.h>
#include <omp.h>
#include <stdio.h>

void impl(int N, int step, double *p) {
  for (int k = 0; k < step; ++k) {
#pragma omp parallel for schedule(dynamic, 64)
    for (int i = 1; i < N - 1; ++i) {
      int j = 1 + (i % 2 + k % 2) % 2;
      for (; j < N - 3; j += 4) {
        p[i * N + j] = (p[(i - 1) * N + j] + p[(i + 1) * N + j] +
                        p[i * N + j + 1] + p[i * N + j - 1]) /
                       4.0f;
        p[i * N + j + 2] = (p[(i - 1) * N + j + 2] + p[(i + 1) * N + j + 2] +
                            p[i * N + j + 3] + p[i * N + j + 1]) /
                           4.0f;
      }
      for (; j < N - 1; j += 2) {
        p[i * N + j] = (p[(i - 1) * N + j] + p[(i + 1) * N + j] +
                        p[i * N + j + 1] + p[i * N + j - 1]) /
                       4.0f;
      }
    }
  }
}

// void impl(int N, int step, double *p) {
//   int blockSize = 64;
//   for (int k = 0; k < step; ++k) {
// #pragma omp parallel for
//     for (int block_i = 1; block_i < N - 1; block_i += blockSize) {
//       for (int block_j = 1; block_j < N - 1; block_j += blockSize) {
//         int iMax = (block_i + blockSize < N - 1) ? block_i + blockSize : N -
//         1; int jMax = (block_j + blockSize < N - 1) ? block_j + blockSize : N
//         - 1;

//         for (int i = block_i; i < iMax; ++i) {
//           for (int j = block_j + (i % 2 + k % 2) % 2; j < jMax; j += 2) {
//             p[i * N + j] = (p[(i - 1) * N + j] + p[(i + 1) * N + j] +
//                             p[i * N + j + 1] + p[i * N + j - 1]) /
//                            4.0;
//           }
//         }
//       }
//     }
//   }
// }

// void impl(int N, int step, double *p) {
//   for (int k = 0; k < step; ++k) {
// #pragma omp parallel for
//     for (int i = 1; i < N - 1; ++i) {
//       int j = 1 + (i % 2 + k % 2) % 2;
//       for (; j < N - 4; j += 4) {
//         _mm256_storeu_pd(
//             &p[i * N + j],
//             _mm256_blend_pd(
//                 _mm256_loadu_pd(&p[i * N + j]),
//                 _mm256_mul_pd(
//                     _mm256_set1_pd(0.25),
//                     _mm256_add_pd(
//                         _mm256_add_pd(_mm256_loadu_pd(&p[(i - 1) * N + j]),
//                                       _mm256_loadu_pd(&p[(i + 1) * N + j])),
//                         _mm256_add_pd(_mm256_loadu_pd(&p[i * N + j - 1]),
//                                       _mm256_loadu_pd(&p[i * N + j + 1])))),
//                 0b1010));
//       }
//       for (; j < N - 1; j += 2) {
//         p[i * N + j] = (p[(i - 1) * N + j] + p[(i + 1) * N + j] +
//                         p[i * N + j + 1] + p[i * N + j - 1]) /
//                        4.0f;
//       }
//     }
//   }
// }