all: carrera

carrera: carrera.cpp
		g++ -std=c++11 -o carrera carrera.cpp -lpthread

clean:
		rm -f carrera

