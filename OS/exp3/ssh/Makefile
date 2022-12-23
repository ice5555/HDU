all: shell cmd1 cmd2 cmd3

work2: shell.c
	gcc shell.c -o shell

cmd1: cmd1.c
	gcc cmd1.c -o cmd1

cmd2: cmd2.c
	gcc cmd2.c -o cmd2

cmd3: cmd3.c
	gcc cmd3.c -o cmd3
run:
	./shell
clean:
	rm -f shell cmd1 cmd2 cmd3

