CXX = g++
SRC = src/*.cpp
TARGET = pwm
CXXFLAGS = -Wall -Wextra -g
LDFLAGS = -l sqlite3

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
