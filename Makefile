




# Compiler
CC = g++

CFLAGS = -std=c++11 -Wall -Wextra -lssl -lcrypto
SRC = customer_information/debugging.cpp US_citizen_database/us_citizen.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = customer_information_executable

$(EXEC): $(OBJ)
	@mkdir -p bin
	$(CC) -o bin/$(EXEC) $(OBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) bin/$(EXEC)


