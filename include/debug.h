#ifndef DBG_H
#define DBG_H
#include <stdio.h>

#define DEBUG(msg) printf("%s", msg);
#define DEBUG_TEST(msg,x,y) printf("[%s] expected %s got %s",msg, x, y)
#endif