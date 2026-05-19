CXX      = g++
CXXFLAGS = -std=c++17 -Wall
LIBS     = -lsfml-graphics -lsfml-window -lsfml-system -lpthread

all: mandelbrot

mandelbrot: main.o ComplexPlane.o
	$(CXX) $(CXXFLAGS) -o mandelbrot main.o ComplexPlane.o $(LIBS)

main.o: main.cpp ComplexPlane.h
	$(CXX) $(CXXFLAGS) -c main.cpp

ComplexPlane.o: ComplexPlane.cpp ComplexPlane.h
	$(CXX) $(CXXFLAGS) -c ComplexPlane.cpp

clean:
	rm -f *.o mandelbrot