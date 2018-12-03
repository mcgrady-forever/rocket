CXX = g++
CXXFLAGS = -std=c++0x -g -Wall

INC = -I../3rd/gtest/inclue \
	  -I../3rd/libevent-2.0.23/include \
      -I../include \

LIB = -lprotobuf \
	  -L../3rd/libevent-2.0.23/lib \
	  -L../3rd/gtest/lib -lgtest \
	  -levent -lrt -lpthread

SRCS     = $(wildcard *.cc)
OBJS 	 = $(patsubst %.cc,%.o,$(SRCS))
TARGETS  = $(SRCS:%.cc=%)

$(info $(SRCS))
$(info $(OBJS))
$(info $(TARGETS))

all: clean $(TARGETS)

$(TARGETS): %: %.o
#$(TARGETS): %.o
#$(TARGETS): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGETS)
