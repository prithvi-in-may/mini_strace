CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic
TARGET := mini_strace
SRC := main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
