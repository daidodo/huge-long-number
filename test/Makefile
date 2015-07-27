TARGET := main2

CXXFLAGS := --std=c++14 -g -Wall

$(TARGET): force
	$(CXX) $(CXXFLAGS) -o $@ main2.cpp

clean: 
	$(RM) $(TARGET) *.o *.d
	$(RM) -r *.dSYM

force:

.PHONY: force clean
