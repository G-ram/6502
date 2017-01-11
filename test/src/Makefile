CXX = clang++

INCLUDES = -I./headers/

CXXFLAGS = -std=c++11 -g -Wall

LDFLAGS = -g -L.
LDLIBS  = -l6502

TARGET = 6502
SRC = $(wildcard *.cpp)
OBJECTS = $(SRC:.cpp=.o)
HEADERS = $(wildcard *.h)

.PHONY: test
test: $(TARGET)
	python ../runner.py ../asm

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LDLIBS)

$(OBJECTS): $(HEADERS)

.PHONY: clean
clean:
	rm -f *~ *.o core $(TARGET)

.PHONY: all
all: clean test
