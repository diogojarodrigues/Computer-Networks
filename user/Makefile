CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET_DIR = ..
TARGET = $(TARGET_DIR)/user.out

all:
	$(CXX) $(CXXFLAGS) *.cpp command/*.cpp -o $(TARGET)

clean:
	rm -f $(TARGET) *.o
