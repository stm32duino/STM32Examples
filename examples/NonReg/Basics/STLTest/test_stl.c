#include "test_stl.h"
#include "Arduino.h"

int test_c(int a, int b, test t)
{
  switch(t) {
    default:
    case ABS:
      return abs(a);
    break;
    case MIN:
      return min(a,b);
    break;
    case MAX:
      return max(a,b);
    break;
    case ROUND:
      return round(a/b);
    break;
  }
}

