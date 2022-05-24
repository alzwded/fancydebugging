set trace-commands on
start
next
# or 0x102 instead of 2
call (void*)__libc_dlopen_mode("./libutils.so", 2)
call (const char*)__Dump(o)
next
call (const char*)__Dump(o)
next
next
call (const char*)__Dump(o)
c
