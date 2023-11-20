CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET_USER = user
TARGET_SERVER = server

all: 
	$(CXX) $(CXXFLAGS) user/user.cpp -o $(TARGET_USER)


clean:
	rm -f $(TARGET_USER) *.o
