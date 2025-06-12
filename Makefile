CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic -Iinclude -fPIC -O3 -march=native
LDFLAGS = -ldl -lgmpxx -lgmp

LIB_ELGAMAL = libelgamal.so
LIB_VERNAM = libvernam.so
LIB_VIGINER = libviginer.so

MAIN = cryptographer

.PHONY: all clean

all: $(MAIN) $(LIB_ELGAMAL) $(LIB_VERNAM) $(LIB_VIGINER)

$(MAIN): src/main.cpp src/menu.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(LIB_ELGAMAL): algorithms/ElGamal/ElGamal_functions.cpp
	$(CXX) -shared $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -lcrypto

$(LIB_VERNAM): algorithms/Vernam/Vernam_main.cpp
	$(CXX) -shared $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(LIB_VIGINER): algorithms/Viginer/Viginer_main.cpp
	$(CXX) -shared $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(MAIN) *.so