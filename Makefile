EXE = GLight
CC = g++
MACHINE = $(shell uname -s)

ifeq ($(MACHINE),Darwin)
    LDFLAGS = -I/usr/local/include/opencv -lopencv_highgui -lopencv_core -lopencv_imgproc -framework OpenGL -I/usr/local/include/SDL -lSDL -lSDLmain -framework cocoa
    CGFLAGS = -Wall -c -g
else
    LDFLAGS =
    CGFLAGS =
endif

all: $(EXE)

$(EXE): main.o glew.o textfile.o
	@echo Linking $(EXE) with $^
	@$(CC) $(LDFLAGS) -o $(EXE) $^

main.o : main.cpp
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@

glew.o : glew/glew.c glew/glew.h
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@

textfile.o : textfile.cpp textfile.h
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@

.PHONY : clean

clean :
	@echo Cleaning $(EXE)
	@rm -rf *.o $(EXE)