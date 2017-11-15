TARGET = Rubic

SRC = *.cpp

LIBS = -lGL -lglut -lGLEW -L./Lib

SHADERS = shaders/*

FLAGS = -Wall -pedantic -Werror -std=c++11

CC = /usr/bin/g++

default: $(TARGET)

all: default

$(TARGET): $(SHADERS)
	$(CC) $(SRC) -o $(TARGET) $(FLAGS) $(LIBS)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)