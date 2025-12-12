CXX := g++
CXXFLAGS := -Wall $(shell wx-config --cxxflags) -I/usr/include/mysql-cppconn/jdbc
LDFLAGS := $(shell wx-config --libs) -lmysqlcppconn 

# Sources and objects
SRCS := App.cc MainFrame.cc Database.cc
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