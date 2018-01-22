all:
	gcc -o server main.c game.c collab_game.c pipe_networking.c word_gen.c sem.c sharedmem.c -g
	gcc -o client client.c pipe_networking.c -g

server-valgrind:
	valgrind ./server --leak-check=full --tool=memcheck

client-valgrind:
	valgrind ./client --leak-check=full --tool=memcheck

clean:
	rm server
	rm client
	rm generated
