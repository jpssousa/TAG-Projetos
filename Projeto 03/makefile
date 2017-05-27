CC = g++
CCFLAGS = -std=c++11
#CCLIBS = 

PROG_NAME = psmatch
REPO_VER = $(shell git rev-parse --short HEAD)
MAIN_RECIPE = programa limpa

ifeq ($(OS),Windows_NT) #source: http://stackoverflow.com/a/12099167
    CCFLAGS += -D WIN32
    PROG_NAME = psmatch.exe
    MAIN_RECIPE = programa limpa_win32
else
	#CCFLAGS += 
endif

PRGMA_O = malloc-the-teachers.o #lista de arquivos objeto


all: $(MAIN_RECIPE)

programa: $(PRGMA_O)
	$(CC) -g $(CCFLAGS) $(PRGMA_O) -o $(PROG_NAME)

malloc-the-teachers.o: malloc-the-teachers.cpp
	$(CC) $(CCFLAGS) -c malloc-the-teachers.cpp
 
limpa:
	find . -type f -name '*.o' -delete
release_app:
	find ./ -name '*.cpp' -o -name '*.hpp' -o -name '*.txt' -o -name 'LEIAME' -o -name 'makefile' | tar -cf '$(PROG_NAME).tar' -T -

limpa_win32:
	del *.o
release_app_win32:
	echo 'Can\'t pack files'
