# fancydebugging

I always forget what the magic libc function is, so here's a repo to remind myself :-)

## The premise

The premise is you have a C++ app, with lots of libraries, and you might not even have symbols for everything. Maybe you're using a lot of pImpls. Maybe it's not exactly trivial to "dump" an object and understand what it's about. Maybe the TU doesn't have enough definitions in it to allow you to see into an object. Maybe you just want to run some arbitrary debug code that you don't really want compiled in.

## How to do it

Compile a shared object with everything you need extern "C" (and make sure they are exported and present in the .so).

Leverage the fact that libc left `__libc_dlopen_mode` for us, and use it to load the debug lib. Note: remember its return value in case you want to unload it (e.g. to tweak it).

You can then call your magic debug symbols.

Make sure to **always** cast the return value when using GDB's `call`, it needs to know where (if any) return value goes.
