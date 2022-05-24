#ifndef COMMON_H
#define COMMON_H

class Object;
extern "C" const char* __Dump(Object*);

class  __attribute__((visibility("default"))) Object
{
    friend const char* __Dump(Object*);
    void* pImpl;
public:
    Object();
    ~Object();
    void Stuff();
    operator int() const;
};

#endif
