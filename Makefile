all:
	gcc main.c hardware.c parsing.c core.c
	./a.out add_asm.txt

debug:
	gcc -g -ggdb main.c hardware.c parsing.c core.c
	gdb -c ./core ./a.out

clean:
	rm a.out
