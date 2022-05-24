#include <dlfcn.h>
#include <cstdlib>
#include <cstring>

extern "C" __attribute__((visibility("default"))) const char* LDR()
{
    static char buf[256];
    void* h;
    const char* (*fn)(); 
    const char* stuff;

    int hr = system("make debugger > /dev/null 2>&1");
    if(hr) {
        strcpy(buf, "Failed to make debugger\n");
        goto END;
    }

    h = dlopen("./debugger", 2);
    if(!h) {
        strcpy(buf, "Failed to load ./debugger");
        goto END;
    }
    *(void**)(&fn) = dlsym(h, "LDR");
    if(!fn) {
        strcpy(buf, "Failed to load LDR symbol\n");
        goto err_SYM;
    }

    stuff = fn();
    strncpy(buf, stuff, 255);
    buf[255] = '\0';
    fn = NULL;
err_SYM:
    dlclose(h);
END:
    return buf;

}
