CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET_DIR = ..
TARGET = $(TARGET_DIR)/test

SRCS := $(wildcard *.cpp ../src/user/protocol.cpp)
OBJ_DIR = bin
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/protocol.o: ../src/user/protocol.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)
