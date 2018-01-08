all:
	gcc -o server main.c game.c pipe_networking.c word_gen.c -g
	gcc -o client client.c pipe_networking.c -g

server:
	./server

client:
	./client

clean:
	rm server
	rm client
	rm generated
