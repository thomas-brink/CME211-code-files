CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wconversion

main: main.cpp CGSolver.cpp CGSolver.hpp COO2CSR.cpp COO2CSR.hpp matvecops.cpp matvecops.hpp
	$(CXX) $(CXXFLAGS) -o main main.cpp CGSolver.cpp COO2CSR.cpp matvecops.cpp

.PHONY: clean
clean:
	$(RM) main main.o CGSolver.o COO2CSR.o matvecops.o solution1.txt solution2.txt
