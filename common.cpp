#include "common.h"
#include <cstdio>

Object::Object()
    : pImpl(new int[2])
{
    int* p = (int*)pImpl;
    p[0] = 42;
    p[1] = 0;
}

Object::~Object()
{
    int* p = (int*)pImpl;
    p[0] = 0xEFBEADDE;
    p[1] = 0x0DF0D0BA;
    delete ((int*)pImpl);
}

void Object::Stuff()
{
    int* p = (int*)pImpl;
    *p *= 2;
    p[1]++;
}

Object::operator int() const
{
    int* p = (int*)pImpl;
    return *p;
}
