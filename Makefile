CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror

DST = keyfilter
SRC = $(DST).c

all: $(DST)

$(DST): $(SRC)

clean:
	rm $(DST)
