CXX = g++
CXXFLAGS = -Wall -std=c++11 # -DSETTING -v

SRC = ./src
INC = ./include
OBJ = ./obj
EXE = analog

OBJS = $(OBJ)/Graph.o $(OBJ)/FileHandler.o $(OBJ)/main.o

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXE)

$(OBJ)/Graph.o: $(SRC)/Graph.cpp $(INC)/Graph.h
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(INC) -c $(SRC)/Graph.cpp -o $(OBJ)/Graph.o

$(OBJ)/FileHandler.o: $(SRC)/FileHandler.cpp $(INC)/FileHandler.h
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(INC) -c $(SRC)/FileHandler.cpp -o $(OBJ)/FileHandler.o

$(OBJ)/main.o: $(SRC)/main.cpp
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(INC) -c $(SRC)/main.cpp -o $(OBJ)/main.o

clean:
	rm -rf $(OBJ) ./analog

all: $(EXE)

.PHONY: clean all
