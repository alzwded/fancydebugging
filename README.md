# fancydebugging

I always forget what the magic libc function is, so here's a repo to remind myself :-)

How to load C-linkage debug utilities in a GDB session when (for some reason) you can't fully debug your application due to missing symbols, missing defines in the TU, impracticality of including the debug utilities in your main app, etc.

## The premise

The premise is you have a C++ app, with lots of libraries, and you might not even have symbols for everything. Maybe you're using a lot of pImpls. Maybe it's not exactly trivial to "dump" an object and understand what it's about. Maybe the TU doesn't have enough definitions in it to allow you to see into an object. Maybe you just want to run some arbitrary debug code that you don't really want compiled in.

## How to do it

Compile a shared object with everything you need extern "C" (and make sure they are exported and present in the .so).

Leverage the fact that libc left `__libc_dlopen_mode` for us, and use it to load the debug lib. Note: remember its return value in case you want to unload it (e.g. to tweak it).

You can then call your magic debug symbols.

Make sure to **always** cast the return value when using GDB's `call`, it needs to know where (if any) return value goes.

You can use `define` to make these easier to use interactive. And put the `dlopen` part in a gdb script, like

```gdb
call (void*)__libc_dlopen_mode("./libutils.so", 2)
define __dump
  call (const char*)__Dump($arg0)
end
```

You can `source` it in your session when needed. Read up on `help define` to know more.

Use the appropriate libc builtin for dlopen. (e.g. on Alpine/musl it's actually `dlopen`)

I recommend that your debug routines have 0 net resource usage. You don't really want these to have memory leaks or to create unlockable locks or to cause deadlocks or anything like that 😃. If you must do multi-stage things that involve allocations and whatnot, it might (?) be possible to use the `$nn` stuff or fancy defines, or just plain keep track of `int` or `void*` return values and deallocate as needed. Couple this with gdb scripts or defines. But really, just stick to "0 apparent resource usage"; avoiding the main app's heap entirely is even better (i.e., if main app uses malloc, don't use malloc; if it uses some other thing, then you are probably safe to use malloc; you're probably always safe to use malloc, but you are shamelessly causing the probe effect at that point). Yeah, what I'm trying to say is to be super careful about the [Probe Effect](https://en.wikipedia.org/wiki/Probe_effect)

On glibc systems, you can leverage `(void)__libc_dlclose(void*)`. E.g. you can `call (void)__libc_dlclose($1)` to unload the library. You can then proceed to recompile/relink and load it back with new code. So if you have multi-step debugging stuff to do, just do it in a separate c/cpp file and load the whole shebang. You can update the script/and/or/define to `dlopen`, *the call* and `dlclose` all in one.

Note, `dlclose` on musl seems to be stubbed out (-- determined empyrically, and partially confirmed via a quick glance over source on github; I think what happens is that after `dlclose`, `dlsym` will give an error or some other uninteresting thing), so if you want unload-reload (or load-do-unload), you need to [daisy chain](./fancier/ldr.cpp) to a ["debug utilities loader"](./fancier) SO [linked with `-ldl`](./fancier/Makefile#L11) which does all of that. While you're there, you can have the loader run [`make debugger`](./fancier/ldr.cpp#L12) or something like that, plus a [gdb `define`](./fancier/script.gdb#L7) like `livedebug` that calls your entrypoint, to the effect that you're live editing a C source and GDB "magically" picks up what you're doing. C interpreter, ho! 😄

## Example

### CentOS

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
+start
Temporary breakpoint 1 at 0x400928: file test.cpp, line 6.

Temporary breakpoint 1, main () at test.cpp:6
6           Object* o = new Object();
+next
7           o->Stuff();
+call (void*)__libc_dlopen_mode("./libutils.so", 2)
$1 = (void *) 0x602090
+call (const char*)__Dump(o)
$2 = 0x7ffff6fea060 <buf> "Object{val=42,cnt=0}"
+next
8           printf("%d\n", static_cast<int>(*o));
+call (const char*)__Dump(o)
$3 = 0x7ffff6fea060 <buf> "Object{val=84,cnt=1}"
+next
84
9           delete o;
+next
10          printf("%p\n", o);
+call (const char*)__Dump(o)
$4 = 0x7ffff6fea060 <buf> "Object{val=0,cnt=0}"
+c
0x602010
[Inferior 1 (process 19606) exited normally]
```

### Alpine

```gdb
gdb -x script_musl.gdb test.bin
GNU gdb (GDB) 10.1
Copyright (C) 2020 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-alpine-linux-musl".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from test.bin...
+start
Temporary breakpoint 1 at 0x1270: file test.cpp, line 6.

Temporary breakpoint 1, main () at test.cpp:6
6           Object* o = new Object();
+next
7           o->Stuff();
+call (void*)dlopen("./libutils.so", 2)
$1 = (void *) 0x7fffff7448a0
+call (const char*)__Dump(o)
$2 = 0x7fffff544080 <buf> "Object{val=42,cnt=0}"
+next
8           printf("%d\n", static_cast<int>(*o));
+call (const char*)__Dump(o)
$3 = 0x7fffff544080 <buf> "Object{val=84,cnt=1}"
+next
84
9           delete o;
+next
10          printf("%p\n", o);
+call (const char*)__Dump(o)
$4 = 0x7fffff544080 <buf> "Object{val=-272716322,cnt=233885882}"
+c
0x7fffff7e7aa0
[Inferior 1 (process 1642) exited normally]
```
