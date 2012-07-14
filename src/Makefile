OBJS = $(patsubst %.cc,%.o,$(wildcard *.cc))
TARGET = main
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

