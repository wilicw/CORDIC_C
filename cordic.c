#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.141592653589793
#define IT 20
#define TEST_LOOP 1000000UL

const uint32_t rotation[] = {0x3f490fdb, 0x3eed6338, 0x3e7adbb0, 0x3dfeadd5,
                             0x3d7faade, 0x3cffeaae, 0x3c7ffaab, 0x3bfffeab,
                             0x3b7fffab, 0x3affffeb, 0x3a7ffffb, 0x39fffffe,
                             0x397ffffe, 0x38fffffe, 0x38800006, 0x38000006,
                             0x37800006, 0x37000006, 0x36800006, 0x35fffe54};

const float k = 0.6072529350092496;

typedef struct point {
  float x;
  float y;
} point_t;

point_t cordic(float);
float cordic_cos(float);
float cordic_sin(float);

int main() {
  double time_spent;
  clock_t begin, end;
  int i;

  srand(time(0));
  int shift = rand();

  {
    float rad = (float)(shift % 90) / 180.0f * PI;
    printf("rad             = %.10f\n", rad);
    printf("cordic_cos(rad) = %.10f\n", cordic_cos(rad));
    printf("cmath_cos(rad)  = %.10f\n", cos(rad));
    puts("-");
  }

  printf("loop: %ld\n", TEST_LOOP);

  begin = clock();

  i = TEST_LOOP;

  while (i--) {
    float rad = (float)((i + shift) % 90) / 180.0f * PI;
    float __x = cos(rad);
    float __y = sin(rad);
  }

  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("cmath : %.10lfs\n", time_spent);

  begin = clock();

  i = TEST_LOOP;

  while (i--) {
    float rad = (float)((i + shift) % 90) / 180.0f * PI;
    float __x = cordic_cos(rad);
    float __y = cordic_sin(rad);
  }

  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("cordic: %.10lfs\n", time_spent);

  return 0;
}

inline point_t cordic(float rad) {
  point_t __point = {.x = 1, .y = 0};
  uint32_t x, y, sign, __rad;

  for (uint8_t i = 0; i < IT; i++) {
    sign = rad >= 0 ? 0x00000000 : 0x80000000;

    x = *(uint32_t *)&__point.x;
    y = *(uint32_t *)&__point.y;

    x = sign ^ x & (((((x & 0x7f800000) >> 23) - i) << 23) | 0x807fffff);
    y = sign ^ y & (((((y & 0x7f800000) >> 23) - i) << 23) | 0x807fffff);

    __point.x -= (*(float *)&y);
    __point.y += (*(float *)&x);

    __rad = sign ^ rotation[i];
    rad -= *(float *)&__rad;
  }
  __point.x *= k;
  __point.y *= k;
  return __point;
}

inline float cordic_cos(float rad) { return cordic(rad).x; }

inline float cordic_sin(float rad) { return cordic(rad).y; }
