# Makefile
CXX = g++
MAIN = driver
CPP_FILES = main_menu.cpp message_handler.cpp char_select.cpp character.cpp images.cpp textures.cpp match.cpp
H_FILES = main_menu.h message_handler.h char_select.h character.h images.h textures.h match.h
O_FILES = main_menu.o message_handler.o char_select.o character.o images.o textures.o match.o
CXXFLAGS = -Wall -std=c++11 -c
PACKAGES = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

# Type 'make' to create the executable
${MAIN}: ${MAIN}.cpp ${H_FILES} ${CPP_FILES}
	${CXX} ${CXXFLAGS} ${MAIN}.cpp ${CPP_FILES} 
	${CXX} ${MAIN}.o ${O_FILES} -o $@ ${PACKAGES}
	@echo 'Use ./${MAIN} ipaddress port# S/C'

# Clean-Up the executable
clean:
	rm -rf ${MAIN} *.o
