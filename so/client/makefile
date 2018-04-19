CXX = g++
CXXFLAGS = -std=c++0x -g -Wall

all: clean test client

INC = -I. -I.. -I../proto -I../../include -I../../3rd/libevent-2.0.23/include

LIB = -lprotobuf -L../../3rd/libevent-2.0.23/lib -levent -lrt

test_OBJS = test.o sock.o ../msg.o ../proto/echo.pb.o

client_OBJS = client.o ../msg.o ../proto/echo.pb.o

test: $(test_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

client: $(client_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f *.o test client
