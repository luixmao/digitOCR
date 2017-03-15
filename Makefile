# Name: Mark Redekopp
# email: redekopp@usc.edu

CC = g++
CFLAGS = -g -Wall
LDFLAGS =
SRCS =  character.cpp test_ocr.cpp bmplib.cpp
OBJ = $(SRCS:.cpp=.o)


all: test_ocr

test_ocr: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o test_ocr

.cpp.o:
	$(CC) $(CFLAGS) -c $<

character.o test_ocr.o : character.h point.h

clean:
	rm -f *.o test_ocr *~

tarit:
	tar cvf ~/rel/maze_samples.tar maze?.in
	chmod 644 ~/rel/maze_samples.tar
