CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Iinclude
LDFLAGS  = -pthread
SRCS     = $(wildcard src/*.cpp)
OBJS     = $(SRCS:src/%.cpp=build/%.o)
TARGET   = build/typeape

all: build $(TARGET)

build:
	mkdir -p build

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: all
	cp $(TARGET) /usr/local/bin/typeape
	mkdir -p /usr/local/share/typeape/data
	cp data/words.txt /usr/local/share/typeape/data/

uninstall:
	rm -f /usr/local/bin/typeape
	rm -rf /usr/local/share/typeape

clean:
	rm -rf build

.PHONY: all install uninstall clean build
