CC=icpc
CFLAGS=-O3
all: 
	$(CC) $(CFLAGS) -o a -g readMatrix.cpp
clean:
	rm -rf a  core*
