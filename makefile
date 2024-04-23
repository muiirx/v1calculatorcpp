CXX=g++
CXXFLAGS=-Wall -I/usr/include/muparser
LDFLAGS=-lmpfr -lgmp
TARGET=calculator

all: $(TARGET)

$(TARGET): calculator.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
