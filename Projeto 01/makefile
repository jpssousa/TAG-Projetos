CC = gcc
CCFLAGS = -g
#CCLIBS = 

PROG_NAME = amigos
REPO_VER = $(shell git rev-parse --short HEAD)
MAIN_RECIPE = programa limpa

ifeq ($(OS),Windows_NT) #source: http://stackoverflow.com/a/12099167
    CCFLAGS += -D WIN32
    PROG_NAME = amigos.exe
    MAIN_RECIPE = programa limpa_win32
else
	CCFLAGS += -ansi
endif

PRGMA_O = main.o #lista de arquivos objeto
TOOLS_O = Headers/graph.o Headers/list.o


all: $(MAIN_RECIPE)

programa: $(PRGMA_O) $(TOOLS_O)
	$(CC) $(CCFLAGS) $(PRGMA_O) $(TOOLS_O) -o $(PROG_NAME)

main.o: main.c
	$(CC) -c main.c


Headers/graph.o: Headers/graph.c
Headers/list.o: Headers/list.c
 
limpa:
	find . -type f -name '*.o' -delete
release_app:
	find ./ -name '*.c' -o -name '*.h' -o -name '*.txt' -o -name 'LEIAME' -o -name 'makefile' | tar -cf '$(PROG_NAME).tar' -T -

limpa_win32:
	del *.o
	del Headers/*.o
release_app_win32:
	echo 'Can\'t pack files'
