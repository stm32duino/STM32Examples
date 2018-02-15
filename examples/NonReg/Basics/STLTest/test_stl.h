#ifndef _STLTEST_H_
#define _STLTEST_H_

typedef enum _test {
  ABS,
  MIN,
  MAX,
  ROUND
} test;

#ifdef __cplusplus
extern "C" {
#endif
int test_c(int a, int b, test t);
#ifdef __cplusplus
}
#endif



#endif /*_STLTEST_H_*/
