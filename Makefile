CXX = g++                              
CXXFLAGS = -Wall -std=c++11 # -DSETTING -v

OBJ = ./bin
EXE = ./analog

# Object files
OBJS = $(OBJ)/Graph.o $(OBJ)/FileHandler.o $(OBJ)/main.o

# build the final executable
$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXE)

# compile Graph.cpp
$(OBJ)/Graph.o: ./Graph/Graph.cpp ./Graph/Graph.h
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c Graph/Graph.cpp -o $(OBJ)/Graph.o

# compile FileHandler.cpp
$(OBJ)/FileHandler.o: ./FileHandler/FileHandler.cpp ./FileHandler/FileHandler.h
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c FileHandler/FileHandler.cpp -o $(OBJ)/FileHandler.o

# compile main.cpp
$(OBJ)/main.o: main.cpp
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c main.cpp -o $(OBJ)/main.o

clean:
	rm -rf $(OBJ) $(EXE)

all: $(EXE)

.PHONY: clean all