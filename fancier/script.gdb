set trace-commands on
set pagination off
!make fortytwodebugger
start
call (void*)dlopen("./libldr.so", 2)
#call (void*)__libc_dlopen_mode("./libldr.so", 2)
define __debug
  if $argc > 0
    call (const char*)LDR((void*)$arg0)
  else
    call (const char*)LDR((void*)0)
  end
  printf "%s\n", $
end
__debug "Hello, World!\n"
!make procversiondebugger
__debug 0
echo You can modify the code in './scratch.cpp', then run __debug again\nand see what happens :-)\n
