# Hangman by Sonal Parab and Helen Ye (Period 4)

This is hangman in the terminal, with networking to support multiplayer options.
There are 3 modes of play:
* Single-player
* Cooperative
* Competitive

### Single-player

To play alone, run the client and enter `1` as the game mode.
The game will begin, and you can play for as long as you want.
`Ctrl-c` will quit the game.

### Cooperative

To play cooperatively with another player, run the client and enter `2` as the game mode.
If there is another client waiting, you will automatically join that game.
Turns will alternate until you win or lose.

### Competitive

To play competitively with another player, run the client and enter `3` as the game mode.
If there is another cilent waiting, you will automatically join that game.
If a player guesses a letter right, they will get to go again.
If not, the other player guesses until the word is guessed or the man is hanged.

## Instructions

Clone the repository.

```
$ git clone https://github.com/yhelen-stuy/hangman.git && cd hangman
```

### Build

To compile this project, run

```
$ make
```

### Setting Up the Server

To set up the server, run

```
$ ./server
```

The server must be running whenever the game is played.

### Play!

To connect to the server and play the game, run

```
$ ./client
```

If the game is being run on a server through a network, connect with the server's IP address:

```
$ ./client 127.0.0.1
```

Replace `127.0.0.1` with the server's IPv4 address.

## Bugs

* Only one game of collab and competitive mode can be run at the same time, meaning multiple collab or multiple competitive game can not be run at once
* Sometimes there is a delay between turns in competitive mode
* Client can quit at anytime (Tried signal handler to fix this, but introduced a lot of problems.) However, if playing collab or competitive mode, the player left can finish that game, but must wait for another client to connect with the same mode before a new game is started.
* Some words in the dictionary have special unicode characters such as é that we don't accept.
* When playing in a multiplayer mode, when you input to stdin while it isn't your turn, it reads it as the next input for when it is your turn.
* The word prints in the server (this was intentional to ease testing).
* Client leaks memory.
* When you run out of words (although there are about 60,000 words per subserver), and then you quit the main server, you have to wait ~1 minute before the port is no longer in use and run the server again.
