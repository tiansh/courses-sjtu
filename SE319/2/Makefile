CC=gcc
OPTIONS=-Wall
PROGRAM=dsdv
DEFINE=-DDEBUG=0

$(PROGRAM):%:%.c
	@$(CC) $(OPTIONS) -o $@ $^ $(DEFINE)

clean:
	@rm $(PROGRAM)
