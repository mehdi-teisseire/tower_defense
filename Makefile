CXX = g++
CXXFLAGS = -Wall -std=c++11
#Linux
#LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

#Windows
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm
SOURCES = main.cpp Game.cpp Enemy.cpp Tower.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = tower_defense

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)