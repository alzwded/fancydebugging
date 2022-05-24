set trace-commands on
start
next
# or 0x102 instead of 2
call (void*)__libc_dlopen_mode("./libutils.so", 2)
define __dump
call (const char*)__Dump($arg0)
call (const char*)__Dump(o)
__dump 0
next
call (const char*)__Dump(o)
next
next
call (const char*)__Dump(o)
c
