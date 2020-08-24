.PHONY: all runprog clean
CXX = g++
CXXFLAGS = -Wall -Werror
LIB = -std=c++11
SFML = -lsfml-graphics -lsfml-window -lsfml-system
SFML2 = -I SFML-2.5.1/include -L SFML-2.5.1/lib
OUT = snake.exe
HEADERS = src/map.hpp src/player.hpp src/view.hpp
all: bin/$(OUT)

bin/$(OUT): build/src/main.o
		$(CXX) $< -o $@ $(SFML2) $(SFML) $(LIB)

build/src/main.o: src/main.cpp $(HEADERS)
		$(CXX) $(SFML2) $(CXXFLAGS) -I src -c $< -o $@ $(LIB)

runprog:
		./bin/$(OUT)

clean:
		rm -rf bin/$(OUT)
		rm -rf build/src/*.o

test: src/testSQL.cpp
		$(CXX) $(CXXFLAGS) -o bin/sqltest.exe $< -I /usr/include/mysql -L /usr/lib/mysql -lmysqlclient

runtest:
		./bin/sqltest.exe
