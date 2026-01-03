CCX64 := clang -target x86_64-w64-mingw32
CCX86 := clang -target i686-w64-mingw32
ASMCC := nasm

all: x64 x86

x64:
	@ $(CCX64) -c src/inject-explicit.cc -o bin/inject-explicit.x64.obj -Os -s -Qn
	@ $(CCX64) -c src/inject-spawn.cc -o bin/inject-spawn.x64.obj -Os -s -Qn

x86:
	@ $(CCX86) -c src/inject-explicit.cc -o bin/inject-explicit.x86.obj -Os -s -Qn
	@ $(CCX86) -c src/inject-spawn.cc -o bin/inject-spawn.x86.obj -Os -s -Qn

clean:
	@ rm *.obj
