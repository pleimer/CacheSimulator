CC = g++
OPT = -g
WARN = -Wall
CFLAGS = $(OPT) $(WARN) 

# List corresponding compiled object files here (.o files)
SIM_OBJ = cache.o

TESTCASES = testcase0 testcase1 testcase2 testcase3 testcase4 testcase5
 
#################################

# default rule
all:	$(TESTCASES)

# generic rule for converting any .cc file to any .o file
.cc.o:
	$(CC) $(CFLAGS) -c *.cc

#rule for creating the object files for all the testcases in the "testcases" folder
testcase: 
	$(MAKE) -C testcases

# rules for making testcases
testcase0: .cc.o testcase 
	$(CC) -o bin/testcase0 $(CFLAGS) $(SIM_OBJ) testcases/testcase0.o

testcase1: .cc.o testcase 
	$(CC) -o bin/testcase1 $(CFLAGS) $(SIM_OBJ) testcases/testcase1.o

testcase2: .cc.o testcase
	$(CC) -o bin/testcase2 $(CFLAGS) $(SIM_OBJ) testcases/testcase2.o

testcase3: .cc.o testcase 
	$(CC) -o bin/testcase3 $(CFLAGS) $(SIM_OBJ) testcases/testcase3.o

testcase4: .cc.o testcase
	$(CC) -o bin/testcase4 $(CFLAGS) $(SIM_OBJ) testcases/testcase4.o

testcase5: .cc.o testcase 
	$(CC) -o bin/testcase5 $(CFLAGS) $(SIM_OBJ) testcases/testcase5.o

# type "make clean" to remove all .o files plus the sim binary
clean:
	rm -f testcases/*.o
	rm -f *.o 
	rm -f bin/*
