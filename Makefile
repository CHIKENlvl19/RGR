CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SRC = main.cpp menu.cpp
OBJ = main.o menu.o
TARGET = cryptographer

all: $(TARGET)

LDLIBS = -licuuc -licui18n

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDLIBS)


%.o: %.cpp header.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
