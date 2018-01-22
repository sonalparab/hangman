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

**TO BE WRITTEN**

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
