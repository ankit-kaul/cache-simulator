CXXFLAGS := -Og -g -Wall -std=c++11 -lm
BINARY := cachesim

all: $(BINARY)

cachesim: cachesim.o cachesim_driver.o
	$(CXX) -o cachesim -lm cachesim.o cachesim_driver.o

clean:
	rm -rf $(BINARY) *.o myoutput
