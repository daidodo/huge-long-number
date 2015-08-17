TARGET = $(SRC:.cpp=)

CXXFLAGS := --std=c++14 -g -Wall -I.. #-O2 -DNDEBUG

SRC := $(wildcard *.cpp)
DEP := $(SRC:.cpp=.d)

CXXFLAGS += -MD

all: clean $(TARGET)

clean: 
	$(RM) $(TARGET) *.o *.d
	$(RM) -r *.dSYM

.PHONY: all clean
