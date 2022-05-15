CC = gcc
  # compiler flags:
  #  -g     - this flag adds debugging information to the executable file
  #  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall -std=gnu99
 
  # The build target 
MASTER = oss
SLAVE = process
HEADER = config
 
all: $(MASTER) $(SLAVE) $(HEADER)
 
#$(MASTER): $(MASTER).o
#	$(CC) $(CFLAGS) $(MASTER).c -o $(MASTER)

#$(SLAVE): $(SLAVE).c
#	$(CC) $(CFLAGS) $(SLAVE).c -o $(SLAVE)

.SUFFIXES: .c .o .h
.PHONY: all clean

clean:
	/bin/rm -f *.o $(MASTER) $(SLAVE)

