all: gdbstub

gdbstub: gdbstub.c
	gcc $< -o $@

clean:
	@rm -f gdbstub
