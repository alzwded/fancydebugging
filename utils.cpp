#include "common.h"
#include <cstdio>

char buf[256];
extern "C" const char* __Dump(Object* o)
{
    int* p = (int*)o->pImpl;
    buf[255] = '\0';
    snprintf(buf, 255, "Object{val=%d,cnt=%d}", p[0], p[1]);
    return buf;
}
