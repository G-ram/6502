CXX = clang++

INCLUDES = -I./loader -I./peripheral

CXXFLAGS = -std=c++11 -g -Wall

LDFLAGS = -g
LDLIBS  =

SRC = $(wildcard *.cpp loader/*.cpp peripheral/*.cpp)
COMMON=$(patsubst cpu%,,${SRC})
TARGET = 6502
CPU = cpu.cpp
OBJECTS = $(COMMON:.cpp=.o) $(CPU)
HEADERS = $(wildcard *.h)

.PHONY: default
default: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LDLIBS)

$(OBJECTS): $(HEADERS)

.PHONY: test
test: $(TARGET)
	python test/runner.py test/asm

.PHONY: clean
clean:
	rm -f *~ a.out core $(OBJECTS) $(TARGET)

.PHONY: all
all: clean default
