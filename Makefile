all:
	gcc main.c hardware.c parsing.c core.c
	./a.out swap_asm.txt

debug:
	gcc -g -ggdb main.c hardware.c parsing.c core.c
	gdb -c ./core ./a.out swap_asm.txt

clean:
	rm a.out
