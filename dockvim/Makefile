PROGRAM = dockvim
TARGET  = ./
OBJS    = dockvim.o
SRCS    = $(OBJS:%.o=%.c)
CC      = gcc
CFLGAS  = -g -Wall -Wextra

.PHONY: $(PROGRAM)
$(PROGRAM):$(OBJS)
	$(CC) $(CFLGAS) $(LDFLAGS) -o $(TARGET)/$(PROGRAM) $(OBJS) $(LDLIBS)

clean:
	rm -f *.o $(TARGET)/$(PROGRAM)

