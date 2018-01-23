SERVER_FILES = src/main.c src/subserver.c src/game.c src/collab_game.c src/competitive_game.c \
		src/pipe_networking.c src/word_gen.c src/sem.c src/sharedmem.c

CLIENT_FILES = src/client.c src/pipe_networking.c

all:
	gcc -o server $(SERVER_FILES) -g
	gcc -o client $(CLIENT_FILES) -g

server-valgrind:
	valgrind ./server --leak-check=full --tool=memcheck

client-valgrind:
	valgrind ./client --leak-check=full --tool=memcheck

clean:
	rm server
	rm client
	rm generated
