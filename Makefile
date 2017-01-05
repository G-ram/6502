CXX = clang++

INCLUDES = -I./loader -I./peripheral

CXXFLAGS = -std=c++11 -g -Wall

LDFLAGS = -g
LDLIBS  =

SRC = $(wildcard *.cpp loader/*.cpp peripheral/*.cpp)
TARGET = 6502
OBJECTS = $(SRC:.cpp=.o)
HEADERS = $(wildcard *.h)

.PHONY: default
default: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LDLIBS)

$(OBJECTS): $(HEADERS)

.PHONY: clean
clean:
	rm -f *~ a.out core $(OBJECTS) $(TARGET)

.PHONY: all
all: clean default
