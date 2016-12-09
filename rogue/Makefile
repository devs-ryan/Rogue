cc=gcc
flags= -std=c99 -Wall
libs= -lncurses
inc= -Iinclude
src= src/*.c
out=run

all: $(out)

$(out): $(src)
	$(cc) $(flags) $(inc) $(libs) $(src) -o $(out)

clean:
	rm $(out)
test:
	./run ./assets/testFiles/data.txt


