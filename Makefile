LDFLAGS = -lSDL2 -lGL -lGLEW -Llib/assimp-3.3.1/lib/ -lassimp -Wl,-rpath=lib/assimp-3.3.1/lib/
CXXFLAGS = -Wall -Iinclude/ -Ilib/assimp-3.3.1/include/ -std=c++11 -fdiagnostics-color

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
	@mkdir -p bin
	g++ -o bin/dm $^ $(LDFLAGS)

obj/%.o: src/%.cpp
	g++ $(CXXFLAGS) -c -o $@ $<
	
clean:
	rm -f obj/*
	rm -f bin/*

re: clean all
