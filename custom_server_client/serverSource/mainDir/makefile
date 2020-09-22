#name: Justin Pank
#email: jtpank@g.ucla.edu
#bid: 204470307

CXX		:= g++
CXX_FLAGS	:= -Wextra -std=c++17 -ggdb

INPUTF		:= input14.txt
INFILES		:= inFiles
BIN		:= bin
SRC		:= src
INCLUDE		:= include
LIB		:= lib
LIBRARIES	:=
EXECUTABLE	:= main

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	@echo "Executing..."
	./$(BIN)/$(EXECUTABLE) < $(INFILES)/$(INPUTF)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	@echo "Building..."
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@echo "Cleaning..."
	rm $(BIN)/*

