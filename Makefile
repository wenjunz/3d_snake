all: 3d_snake

3d_snake: main.o Block.o MyCamera.o Geometry.o Snake.o
	g++ -o 3d_snake main.o Block.o MyCamera.o Geometry.o Snake.o -lglut -lglui -lGL

main.o: main.cpp
	g++ -c main.cpp

Block.o: Block.cpp Block.h
	g++ -c Block.cpp

MyCamera.o: MyCamera.cpp MyCamera.h
	g++ -c MyCamera.cpp

Geometry.o: Geometry.cpp Geometry.h
	g++ -c Geometry.cpp

Snake.o: Snake.cpp Snake.h
	g++ -c Snake.cpp
