CXX = g++
CXXFLAGS = -g -Wall

all: clean test client

INC = -I. -I.. -I../proto -I../../include -I../../3rd/protobuf-2.6.1/include -I../../3rd/libevent-2.0.23/include

LIB = -L../../3rd/protobuf-2.6.1/lib -lprotobuf -L../../3rd/libevent-2.0.23/lib -levent -lrt

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
