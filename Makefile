TARGET = Rubic

SRC = glad.c *.cpp

LIBS = -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor

SHADERS = shaders/*

INCLUDE_FLAGS = -I./include -I/usr/include/ 

INCLUDE = ./include/*

FLAGS = -Wall -pedantic -Werror -std=c++11 $(INCLUDE_FLAGS)

CC = /usr/bin/g++

default: $(TARGET)

all: default

$(TARGET): $(SHADERS) $(SRC) $(INCLUDE) shaders/*/*
	$(CC) $(SRC) -o $(TARGET) $(FLAGS) $(LIBS)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)
