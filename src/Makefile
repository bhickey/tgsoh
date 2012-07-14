TARGET = main
OBJS = $(patsubst %.cc,%.o,$(wildcard *.cc))
HEADERS = $(wildcard *.h)
CXXFLAGS = -Wall -g -O3
LDFLAGS =
CXX = g++

all:: $(TARGET)

clean::
	rm -f $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $(OBJS)

.cc.o:
	$(CXX) -c -o $@ $(CXXFLAGS) $<

# update below with makedepend *.cc
# DO NOT DELETE

map.o: map.h enums.h
solve.o: map.h enums.h
