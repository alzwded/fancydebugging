#include <cstdio>
#ifndef WHICH
# define WHICH 0
#endif

extern "C" __attribute__((visibility("default"))) const char* LDR()
{
    static char buf[256];
    size_t n = 0;
    switch(WHICH) {
        case 0:
            {
                FILE* f = fopen("/proc/cpuinfo", "r");
                while(!feof(f) && n < 255) {
                    int c = fgetc(f);
                    if(c == EOF) break;
                    buf[n++] = (char)(c & 0xFF);
                }
                buf[255] = '\0';
            }
            return buf;
        default:
            {
                buf[0] = '4';
                buf[1] = '2';
                buf[2] = '\0';
            }
            return buf;
    }
}
