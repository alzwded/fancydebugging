#include <cstdio>
#include "common.h"

int main()
{
    Object* o = new Object();
    o->Stuff();
    printf("%d\n", static_cast<int>(*o));
    delete o;
    printf("%p\n", o);
}

