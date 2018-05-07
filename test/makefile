CXX = g++
CXXFLAGS = -std=c++0x -g -Wall

all: clean test

INC = -I../3rd/gtest/inclue \
	  -I../3rd/libevent-2.0.23/include \
      -I../include \

LIB = -lprotobuf \
	  -L../3rd/libevent-2.0.23/lib \
	  -L../3rd/gtest/lib -lgtest \
	  -levent -lrt -lpthread

OBJS = $(patsubst %.cc, %.o, $(wildcard *.cc))

test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f *.o test