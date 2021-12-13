CXX := g++
CXXFLAGS := -O3 -std=c++11 -Wall -Wextra -Wconversion -pedantic



main: main.cpp matvecops.cpp matvecops.hpp CGSolver.cpp CGSolver.hpp COO2CSR.cpp COO2CSR.hpp heat.cpp heat.hpp sparse.cpp sparse.hpp
	$(CXX) $(CXXFLAGS) -o main main.cpp matvecops.cpp CGSolver.cpp COO2CSR.cpp heat.cpp sparse.cpp

.PHONY: clean
clean:
	$(RM) main main.o matvecops.o CGSolver.o COO2CSR.o heat.o sparse.o ~*
