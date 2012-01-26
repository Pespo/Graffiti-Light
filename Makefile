EXE = GLight
CC = g++
MACHINE = $(shell uname -s)

ifeq ($(MACHINE),Darwin)
    LDFLAGS = -I/usr/local/include/opencv -lopencv_highgui -lopencv_core -lopencv_imgproc -framework OpenGL -framework GLUT
    CGFLAGS = -Wall -c -g
else
    LDFLAGS =
    CGFLAGS =
endif

all: $(EXE)

$(EXE): main.o functions.o Timer.o textfile.o
	@echo Linking $(EXE) with $^
	@$(CC) $(LDFLAGS) -o $(EXE) $^

main.o : main.cpp functions.hpp
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@

functions.o : functions.cpp functions.hpp
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@

Timer.o : Timer.cpp Timer.h
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@

textfile.o : textfile.cpp textfile.h
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@

.PHONY : clean

clean :
	@echo Cleaning $(EXE)
	@rm -rf *.o $(EXE)