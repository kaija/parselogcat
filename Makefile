
OBJS=parselog.o
RM=rm -rf
EXEC=plog

all: $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(EXEC) $(OBJS)
