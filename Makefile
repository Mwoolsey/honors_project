# Makefile
CXX = g++
MAIN = driver
CPP_FILES = main_menu.cpp message_handler.cpp
H_FILES = main_menu.h message_handler.h
O_FILES = main_menu.o message_handler.o
CXXFLAGS = -Wall -std=c++11 -c
PACKAGES = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

# Type 'make' to create the executable
${MAIN}: ${MAIN}.cpp ${H_FILES} ${CPP_FILES}
	${CXX} ${CXXFLAGS} ${MAIN}.cpp ${CPP_FILES} 
	${CXX} ${MAIN}.o ${O_FILES} -o $@ ${PACKAGES}
	@echo 'Use ./${MAIN} ipaddress port#'

# Clean-Up the executable
clean:
	rm -rf ${MAIN} *.o
