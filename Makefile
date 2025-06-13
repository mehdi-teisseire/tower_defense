CC = g++
CFLAGS = -Wall -std=c++11
# For Windows
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
# For Linux
#LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC = main.cpp Enemy.cpp Tower.cpp Game.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = tower_defense

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)