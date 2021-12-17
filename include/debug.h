#ifndef DBG_H
#define DBG_H
#include <stdio.h>

#define STATIC_ASSERT(COND,MSG) static int static_assertion_##MSG[(COND)?1:-1]

#define DEBUG(msg) printf("%s", msg);
#define DEBUG_TEST(msg,x,y) printf("[%s] expected %s got %s",msg, x, y)
#endif