CXX = clang++

INCLUDES = -I./headers

CXXFLAGS = -std=c++11 -g -Wall

LDFLAGS = -g
LDLIBS  = 

TARGET = lib6502nes.a
SRC = $(wildcard *.cpp)
OBJECTS = $(SRC:.cpp=.o)
HEADERS = $(wildcard *.h)

.PHONY: lib
lib: $(TARGET)

$(TARGET): $(OBJECTS)
	ar rvs $(TARGET) $(OBJECTS)

$(OBJECTS): $(HEADERS)

.PHONY: clean
clean:
	rm -f *~ *.o core $(TARGET)

.PHONY: all
all: clean lib
