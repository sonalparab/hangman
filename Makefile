all:
	gcc -o temp main.c word_gen.c -g

run:
	./temp

clean:
	rm temp
	rm generated

valgrind:
	valgrind ./temp --leak-check=yes
