all: gdbstub

gdbstub: gdbstub.c hex.c
	gcc $^ -o $@

clean:
	@rm -f gdbstub
