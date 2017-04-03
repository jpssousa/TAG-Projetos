REM @echo off
REM assuming gcc is define in env:PATH
del main.o
del list.o
del graph.o
del out.txt
del a.exe
gcc -c main.c
gcc -c Headers/list.c
gcc -c Headers/graph.c
gcc main.o graph.o list.o -o a.exe
a.exe >> out.txt