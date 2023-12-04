CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET_DIR = ..
TARGET = $(TARGET_DIR)/server.out

# SRCS = $(wildcard *.cpp command/*.cpp)
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
