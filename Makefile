all: gdbstub

gdbstub: gdbstub.c hex.c
	gcc -Wall $^ -o $@

clean:
	@rm -f gdbstub
