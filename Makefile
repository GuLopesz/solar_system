TARGET = solar_system

SRCS = main.cpp

CXX = g++

CXXFLAGS = -Wall -std=c++11

LIBS = -lGL -lGLU -lglfw -lGLEW

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)
