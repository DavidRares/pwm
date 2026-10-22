CXX = g++
SRC = src/*.cpp
TARGET = pwm
CXXFLAGS = -std=c++17 -Wall -Wextra -g

LIBSODIUM_IPATH = /opt/homebrew/opt/libsodium/include
LIBSODIUM_LPATH = /opt/homebrew/opt/libsodium/lib
LDFLAGS =  -I$(LIBSODIUM_IPATH) -L$(LIBSODIUM_LPATH) -lsqlite3 -lsodium

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
