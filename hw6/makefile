CXX := g++
CXXFLAGS := -O3 -std=c++11 -Wall -Wextra -Wconversion -pedantic

main: main.cpp hw6.cpp hw6.hpp image.cpp image.hpp
	$(CXX) $(CXXFLAGS) -o main main.cpp hw6.cpp image.cpp -ljpeg

.PHONY: clean
clean:
	$(RM) main main.o hw6.o image.o
