CXX := g++
CXXFLAGS := -Wall -Werror $(shell wx-config --cxxflags)
LDFLAGS := $(shell wx-config --libs)

# Sources and objects
SRCS := App.cc MainFrame.cc
OBJS := $(SRCS:.cc=.o)

# Output program name
TARGET := program

default: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)