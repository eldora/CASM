all:
	gcc main.c hardware.c parsing.c
	./a.out add_asm.txt
clean:
	rm a.out
