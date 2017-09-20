CXXFLAGS := -Og -g -Wall -std=c++11 -lm
BINARY := test

all: $(BINARY)

test: test.o
	$(CXX) -o test -lm test.o 

clean:
	rm -rf $(BINARY) *.o myoutput
