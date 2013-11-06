all:
	gcc main.c hardware.c parsing.c core.c
	./a.out swap_asm_1.txt swap_asm_2.txt

pushpop:
	gcc main.c hardware.c parsing.c core.c
	./a.out swap_asm_pushpop.txt

data:
	gcc main.c hardware.c parsing.c core.c
	./a.out swap_asm_data.txt

debug:
	gcc -g -ggdb main.c hardware.c parsing.c core.c
	gdb -c ./core ./a.out swap_asm.txt

clean:
	rm a.out
