EXE = GLight
CC = g++
LDFLAGS = -I/usr/local/include/opencv -lopencv_highgui -lopencv_core -lopencv_imgproc
CGFLAGS = -Wall -c -g

all: $(EXE)

$(EXE): main.o functions.o
	@echo Linking $(EXE) with $^
	@$(CC) $(LDFLAGS) -o $(EXE) $^

main.o : main.cpp functions.hpp
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@

functions.o : functions.cpp functions.hpp
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@

.PHONY : clean

clean :
	@echo Cleaning $(EXE)
	@rm -rf *.o $(EXE)