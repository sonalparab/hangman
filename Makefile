SERVER_FILES = src/main.c src/subserver.c src/game.c src/collab_game.c src/competitive_game.c \
		src/pipe_networking.c src/word_gen.c src/sem.c src/sharedmem.c

CLIENT_FILES = src/client.c src/pipe_networking.c

NETWORKING_SERVER =  src/main.c src/subserver.c src/game.c src/collab_game.c src/competitive_game.c \
		src/networking.c src/word_gen.c src/sem.c src/sharedmem.c

NETWORKING_CLIENT = src/client.c src/networking.c

all:
	gcc -o server $(NETWORKING_SERVER) -g
	gcc -o client $(NETWORKING_CLIENT) -g

server-valgrind:
	valgrind ./server --leak-check=full --tool=memcheck

client-valgrind:
	valgrind ./client --leak-check=full --tool=memcheck

clean:
	rm server
	rm client
	rm generated
