# fancydebugging

I always forget what the magic libc function is, so here's a repo to remind myself :-)

## The premise

The premise is you have a C++ app, with lots of libraries, and you might not even have symbols for everything. Maybe you're using a lot of pImpls. Maybe it's not exactly trivial to "dump" an object and understand what it's about. Maybe the TU doesn't have enough definitions in it to allow you to see into an object. Maybe you just want to run some arbitrary debug code that you don't really want compiled in.

## How to do it

Compile a shared object with everything you need extern "C" (and make sure they are exported and present in the .so).

Leverage the fact that libc left `__libc_dlopen_mode` for us, and use it to load the debug lib. Note: remember its return value in case you want to unload it (e.g. to tweak it).

You can then call your magic debug symbols.

Make sure to **always** cast the return value when using GDB's `call`, it needs to know where (if any) return value goes.

## Example

```gdb
gdb -x script.gdb test.bin
GNU gdb (GDB) Red Hat Enterprise Linux 7.6.1-120.el7
Copyright (C) 2013 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-redhat-linux-gnu".
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>...
Reading symbols from /work/testgdb/test.bin...done.
Temporary breakpoint 1 at 0x400928: file test.cpp, line 6.

Temporary breakpoint 1, main () at test.cpp:6
6           Object* o = new Object();
7           o->Stuff();
$1 = (void *) 0x602090
$2 = 0x7ffff6fea060 <buf> "Object{val=42,cnt=0}"
8           printf("%d\n", static_cast<int>(*o));
$3 = 0x7ffff6fea060 <buf> "Object{val=84,cnt=1}"
84
9           delete o;
10          printf("%p\n", o);
$4 = 0x7ffff6fea060 <buf> "Object{val=0,cnt=0}"
0x602010
[Inferior 1 (process 10121) exited normally]
```
