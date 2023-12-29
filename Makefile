	CC = gcc
CFLAGS = -m64 -fPIC
CFLAGS_STRESS = -m64 
LDFLAGS_STRESS = -m64

EXEC = ./bin/malloc.so
EXEC_STRESS = ./bin/stress.out
BINARY = firefox

# __version__ 1
v1: HeapMManager_x1.o
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

HeapMManager_x1.o: src/HeapMManager_x1.c 
	$(CC) -c $< $(CFLAGS)


# __version__ 2
v2: HeapMManager_x2.o
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

HeapMManager_x2.o: src/HeapMManager_x2.c 
	$(CC) -c $< $(CFLAGS)


# __version__ 3
v3: HeapMManager_x3.o
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

HeapMManager_x3.o: src/HeapMManager_x3.c 
	$(CC) -c $< $(CFLAGS)

# __stress_malloc__
stress: stress.o
	$(CC) -o $(EXEC_STRESS) $^ $(LDFLAGS_STRESS)

stress.o: src/stress.c
	$(CC) -c $< $(CFLAGS_STRESS)

# __helpers__
clean:
	rm *.o 

mrproper:
	clean rm -f $(EXEC)

launch_firefox:
	LD_PRELOAD=$(EXEC) firefox

launch_gedit:
	LD_PRELOAD=$(EXEC) gedit
