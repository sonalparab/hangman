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
* Sometimes there is a delay between turn in competitive mode
