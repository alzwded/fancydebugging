set trace-commands on
start
next
# or 0x102 instead of 2
call (void*)dlopen("./libutils.so", 2)
define __dump
call (const char*)__Dump($arg0)
end
call (const char*)__Dump(o)
__dump o
next
call (const char*)__Dump(o)
next
next
call (const char*)__Dump(o)
c
