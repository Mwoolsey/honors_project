# Makefile
CXX = g++
MAIN = test
CPP_FILES = character.cpp images.cpp textures.cpp
H_FILES = character.h images.h textures.h
O_FILES = character.o images.o textures.o
CXXFLAGS = -Wall -std=c++11 -c
PACKAGES = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

# Type 'make' to create the executable
${MAIN}: ${MAIN}.cpp ${H_FILES} ${CPP_FILES}
	${CXX} ${CXXFLAGS} ${MAIN}.cpp ${CPP_FILES} 
	${CXX} ${MAIN}.o ${O_FILES} -o $@ ${PACKAGES}
	@echo 'Use ./${MAIN}'

# Clean-Up the executable
clean:
	rm -rf ${MAIN} *.o
