CC=g++
CFLAGS=-c
LDFLAGS=-lwiringPi -lwiringPiDev -lsqlite3 -lpthread
SRC=$(wildcard *.cpp)
OBJS=$(SRC:.cpp=.o)

# App name
EXEC=app-name

all: $(SRC) $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
