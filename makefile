CXX = g++
CXXFLAGS = -g -O2 -Wall -fPIC -lpthread

INC = -I./3rd/glog/include \

LIB = -L./3rd/glog/lib -lglog \
	  -lboost_system\
      -lboost_thread\
      -ldl\

OBJS = $(patsubst %.cc, %.o, $(wildcard *.cc))

BIN = rocket

all: clean $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f $(BIN)
	rm -f $(OBJS)

install:
	mkdir -p bin
	install $(BIN) bin
