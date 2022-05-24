showcase: all
	gdb -x script.gdb test.bin
.PHONY: showcase

all: test.bin libldr.so

test.bin: test.cpp
	g++ -o test.bin -g test.cpp

libldr.so: ldr.cpp
	g++ -shared -o libldr.so -g -fPIC -ldl ldr.cpp

debugger: scratch.cpp
	g++ -shared -o debugger -g -fPIC scratch.cpp

procversiondebugger: scratch.cpp
	g++ -shared -o debugger -g -fPIC -DWHICH=0 scratch.cpp
.PHONY: procversiondebugger

fortytwodebugger: scratch.cpp
	g++ -shared -o debugger -g -fPIC -DWHICH=1 scratch.cpp
.PHONY: fortytwodebugger

clean:
	rm -f test.bin libldr.so debugger
