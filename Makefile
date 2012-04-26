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

$(EXE): bin/main.o bin/Program.o bin/glew.o bin/FBO.o bin/Texture.o bin/VBO.o bin/Camera.o bin/Scene.o bin/Application.o bin/OpenGL.o bin/Mask.o bin/Timer.o
	@echo Linking $(EXE) with $^
	@$(CC) $(LDFLAGS) -o $(EXE) $^

bin/main.o : src/main.cpp
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@

bin/glew.o : glew/glew.c glew/glew.h
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@
    
bin/%.o : src/%.cpp src/%.hpp
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@
    
clean :
	@echo Cleaning $(EXE)
	@rm -rf bin/*.o $(EXE)