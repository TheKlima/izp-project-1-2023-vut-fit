CC = gcc
CFLAGS = -std=c11 -Wall -Wextra # -Werror I will use this flag only when I release for convenience

DST = keyfilter
SRC = $(DST).c
OBJ = $(DST).o

all: $(DST)

$(DST): $(OBJ)

$(OBJ): $(SRC)

clean:
	rm $(DST) $(OBJ)