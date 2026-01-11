CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET = hello-epoll

all: $(TARGET)

$(TARGET): hello-epoll.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) hello-epoll.cpp

clean:
	rm -f $(TARGET)

.PHONY: all clean
