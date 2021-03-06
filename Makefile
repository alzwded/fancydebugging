showcase: all
	( cat /etc/os-release | grep alpine ) && gdb -x script_musl.gdb test.bin || gdb -x script.gdb test.bin
.PHONY: showcase

all: test.bin libutils.so libcommon.so

test.bin: test.cpp common.h libcommon.so
	g++ -o test.bin -g test.cpp -L. -lcommon -Wl,-rpath=.

libutils.so: utils.cpp common.h libcommon.so
	g++ -shared -o libutils.so -g -fPIC utils.cpp -L. -lcommon -Wl,-rpath=.

libcommon.so: common.cpp common.h
	g++ -shared -o libcommon.so -fPIC common.cpp

clean:
	rm -f test.bin libutils.so libcommon.so
