CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SRC = $(shell find . -name '*.cpp')

OBJ = $(patsubst ./%,%,$(SRC:.cpp=.o))

TARGET = cryptographer

all: $(TARGET)


$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp header.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
