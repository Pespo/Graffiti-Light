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

$(EXE): Program.o main.o glew.o FBO.o Texture.o VBO.o Camera.o Scene.o Application.o OpenGL.o Mask.o
	@echo Linking $(EXE) with $^
	@$(CC) $(LDFLAGS) -o $(EXE) $^

main.o : main.cpp
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@

glew.o : glew/glew.c glew/glew.h
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@

FBO.o : FBO.cpp
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@
    
%.o : %.cpp %.hpp
	@echo compiling $@
	@$(CC) $(CGFLAGS) $< -o $@
    
clean :
	@echo Cleaning $(EXE)
	@rm -rf *.o $(EXE)