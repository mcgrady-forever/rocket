CXX = g++
CXXFLAGS = -std=c++11 -g -O2 -Wall -fPIC -fno-threadsafe-statics

INC = -I./3rd/glog/include

LIB = -L./3rd/glog/lib -lglog \
      -ldl\
	  -lpthread\

OBJS = $(patsubst %.cc, %.o, $(wildcard *.cc))

BIN = rocket

all: clean $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f $(BIN)
	rm -f $(OBJS)

install:
	mkdir -p bin
	install $(BIN) bin
