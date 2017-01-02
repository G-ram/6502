CXX = clang++

INCLUDES =

CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g
LDLIBS  =

SRC = $(wildcard *.cpp)
EXECUTABLES = 6502
OBJECTS = $(SRC:.cpp=.o)
HEADERS = $(wildcard *.h)

.PHONY: default
default: $(EXECUTABLES)

$(EXECUTABLES): $(OBJECTS)

$(OBJECTS): $(HEADERS)

.PHONY: clean
clean:
	rm -f *~ a.out core $(OBJECTS) $(EXECUTABLES)

.PHONY: all
all: clean default
