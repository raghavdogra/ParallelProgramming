CC=icpc
CFLAGS=-O0

all:
	$(CC) $(CFLAGS) -o rssp rssp.cpp 
	$(CC) $(CFLAGS) -o RbsP RbsP.cpp
clean:
	rm -rf rssp
	rm -rf RbsP
