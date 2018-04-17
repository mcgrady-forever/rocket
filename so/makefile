CXX = g++
CXXFLAGS = -g -O2 -Wall -fPIC

SO = app.so

LIB = -L../3rd/libevent-2.0.23/lib -levent \
      -L../3rd/protobuf-2.6.1/lib -lprotobuf \
      -L../3rd/ringlog/lib -lringlog \
      -lpthread -lrt

INC = -I../3rd/libevent-2.0.23/include \
      -I../3rd/protobuf-2.6.1/include \
      -I../3rd/ringlog/include \
      -I../include \
      -I. -I./proto

OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp)) \
       $(patsubst %.cc,  %.o, $(wildcard proto/*.cc)) \

all: clean $(SO)

$(SO): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB) -shared

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJS) $(SO)
