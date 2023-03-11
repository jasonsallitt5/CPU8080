CXX = g++
CXXFLAGS = -Wall


driver: CPU8080.o Bus.o Driver.cpp
	$(CXX) $(CXXFLAGS) CPU8080.o Bus.o Driver.cpp -o driver


CPU8080.o: CPU8080.cpp CPU8080.h
	$(CXX) $(CXXFLAGS) -c CPU8080.cpp



Bus.o: Bus.cpp Bus.h 
	$(CXX) $(CXXFLAGS) -c Bus.cpp


clean:
	rm *.o*




