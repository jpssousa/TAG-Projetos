CC = g++
CCFLAGS = -std=c++11
#CCLIBS = 

PROG_NAME = mwguru
REPO_VER = $(shell git rev-parse --short HEAD)
MAIN_RECIPE = programa limpa

ifeq ($(OS),Windows_NT) #source: http://stackoverflow.com/a/12099167
    CCFLAGS += -D WIN32
    PROG_NAME = $(PROG_NAME).exe
    MAIN_RECIPE = programa limpa_win32
else
	#CCFLAGS += 
endif

PRGMA_O = main.o #lista de arquivos objeto
#TOOLS_O = 


all: $(MAIN_RECIPE)

programa: $(PRGMA_O)
	$(CC) -g $(CCFLAGS) $(PRGMA_O) -o $(PROG_NAME)

main.o: main.cpp
	$(CC) $(CCFLAGS) -c main.cpp
 
limpa:
	find . -type f -name '*.o' -delete
release_app:
	find ./ -name '*.c' -o -name '*.h' -o -name '*.txt' -o -name 'LEIAME' -o -name 'makefile' | tar -cf '$(PROG_NAME).tar' -T -

limpa_win32:
	del *.o
release_app_win32:
	echo 'Can\'t pack files'