LDFLAGS = -lSDL2 -lGL -lGLEW
CXXFLAGS = -Wall -Iinclude/ -std=c++11 -fdiagnostics-color

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=obj/%.o)

.PHONY: lib

all: linking

lib:
	@# build ASSIMP
	cd lib/assimp-3.3.1/ && \
	cmake CMakeLists.txt -G 'Unix Makefiles' && \
	make && \
	cd ../../
	
linking: $(OBJ)
	g++ -o bin/dm $^ $(LDFLAGS)

obj/%.o: src/%.cpp
	g++ $(CXXFLAGS) -c -o $@ $<
	
clean:
	rm obj/*
	rm bin/*

re: clean all