#include "game.h"
#include "sem.h"
#include "sharedmem.h"
#include "networking.h"

void get_status(int len, int client_socket){
    int pid = getpid();
    char * buffer = (char *) calloc (BUFFER_SIZE, sizeof(char));
    char * message = (char *) calloc (BUFFER_SIZE, sizeof(char));
    int test;

    int shmid_guessing = shmget(GUESSING_ARRAY_KEY2, (sizeof(char) * 20),0600);
    char * guessing_array = get_shm(shmid_guessing);

    int shmid_wrong = shmget(WRONG_GUESSES_KEY2, (sizeof(int)),0600);
    int wrong_guesses = get_shm_int(shmid_wrong);

    //show the current player what other player accomplished
    //print the man
    char * hangman = (char *) calloc(BUFFER_SIZE,sizeof(char));
    strcpy(hangman,"man");
    strcat(hangman,generate_man(wrong_guesses));
    write(client_socket, hangman, BUFFER_SIZE);
    printf("[subserver %d] Sent %s\n", pid, hangman);
    test = read(client_socket, buffer, BUFFER_SIZE);
    if (test == -1 || strcmp(buffer, ACK)) {
        printf("Error 0.5!");
    }
    hangman = zero_heap(hangman, BUFFER_SIZE);
    buffer = zero_heap(buffer, BUFFER_SIZE);

    //print the blank spaces for the word, with correct guesses filled in
    if (guessing_array[0] != 0) {
        strcpy(message, "guessing");
        strcat(message, guessing_array);
        write(client_socket,message,BUFFER_SIZE);
        printf("[subserver %d] Sent %s\n", pid, message);
        message = zero_heap(message, BUFFER_SIZE);
        test = read(client_socket, buffer, BUFFER_SIZE);
        if (test == -1 || strcmp(buffer, ACK)) {
            printf("Error 2!");
        }
        buffer = zero_heap(buffer, BUFFER_SIZE);
    }

}

int run_turn_competitive(int len,int *wrong_guessespointer, char* guessing_array,
        char* guessed_letters, int *index, char * word, int client_socket){
    int wrong_guesses = *wrong_guessespointer;
    int g = *index;
    //printf("LEN: %d\n\n",len);

    int pid = getpid();
    //input (guess) with potentially multiple characters and newline
    char input[100];
    //letter guessed
    char letter;
    // string containing hang man
    char * hangman;
    // to send number of wrong guesses
    char * man;
    // for messages to be sent to client
    char * message = (char *) calloc(BUFFER_SIZE, sizeof(char));
    char * buffer = (char *) calloc (BUFFER_SIZE, sizeof(char));
    int test;
    printf("[subserver %d] running turn...\n", pid);

    //print the man
    hangman = (char *) calloc(BUFFER_SIZE,sizeof(char));
    strcpy(hangman,"man");
    strcat(hangman,generate_man(wrong_guesses));
    write(client_socket, hangman, BUFFER_SIZE);
    printf("[subserver %d] Sent %s\n", pid, hangman);
    test = read(client_socket, buffer, BUFFER_SIZE);
    if (test == -1 || strcmp(buffer, ACK)) {
        printf("Error 0.5!");
    }
    hangman = zero_heap(hangman, BUFFER_SIZE);
    buffer = zero_heap(buffer, BUFFER_SIZE);

    //print the blank spaces for the word, with correct guesses filled in
    int i = 0;
    if (guessing_array[0] != 0) {
        strcpy(message, "guessing");
        strcat(message, guessing_array);
        write(client_socket,message,BUFFER_SIZE);
        printf("[subserver %d] Sent %s\n", pid, message);
        message = zero_heap(message, BUFFER_SIZE);
        test = read(client_socket, buffer, BUFFER_SIZE);
        if (test == -1 || strcmp(buffer, ACK)) {
            printf("Error 2!");
        }
        buffer = zero_heap(buffer, BUFFER_SIZE);
    }

    //check for blank spaces in guessing_array
    // to see if the word was fully guessed already
    i = 0;
    //boolean for checking blank spaces
    int b = 0;
    for (;i < len; i++){
        if (guessing_array[i] == '_') {
            b = 1;
            break;
        }
    }

    //for second client to exit turn early

    //if b is 0, there were no blank spaces
    // word was already guessed, other player won
    if (!b) {
        strcpy(message,"Your opponent guessed the word, you lose!");
        write(client_socket, message, BUFFER_SIZE);
        printf("[subserver %d] Sent %s\n", pid, message);
        test = read(client_socket, buffer, BUFFER_SIZE);
        if (test == -1 || strcmp(buffer, ACK)) {
            printf("Error 7!");
        }
        buffer = zero_heap(buffer, BUFFER_SIZE);
        message = zero_heap(message, BUFFER_SIZE);
        return -8;
    }

    //if wrong_guesses is 6, other player lost
    if(wrong_guesses == 6){
        strcpy(message, "You didn't guess the word, but you win!");
        write(client_socket, message, BUFFER_SIZE);
        printf("[subserver %d] Sent %s\n", pid, message);
        test = read(client_socket, buffer, BUFFER_SIZE);
        if (test == -1 || strcmp(buffer, ACK)) {
            printf("Error 6!");
        }
        buffer = zero_heap(buffer, BUFFER_SIZE);
        message = zero_heap(message, BUFFER_SIZE);
	strcpy(message,"The word was: ");
	strcat(message,word);
	write(client_socket, message, BUFFER_SIZE);
	printf("[subserver %d] Sent %s\n", pid, message);
	test = read(client_socket, buffer, BUFFER_SIZE);
	if (test == -1 || strcmp(buffer, ACK)) {
	    printf("Error 11!");
	}
	buffer = zero_heap(buffer, BUFFER_SIZE);
	message = zero_heap(message, BUFFER_SIZE);
        return -9;
    }

    int k = 1;
    while (k) {

        //print the letters guessed already, if guesses were made
        i = 0;
        if (g) {
            strcpy(message, "guessed");
            strcat(message, guessed_letters);
            write(client_socket,message,BUFFER_SIZE);
            printf("[subserver %d] Sent %s\n", pid, message);
            message = zero_heap(message, BUFFER_SIZE);
            test = read(client_socket, buffer, BUFFER_SIZE);
            if (test == -1 || strcmp(buffer, ACK)) {
                printf("Error 3!");
            }
            buffer = zero_heap(buffer, BUFFER_SIZE);
        }

        strcpy(message,"Pick a letter: ");
        write(client_socket, message, BUFFER_SIZE);
        printf("[subserver %d] Sent %s\n", pid, message);
        message = zero_heap(message, BUFFER_SIZE);

        printf("[subserver %d] waiting for input\n", pid);
        //prompt input for a letter
        test = read(client_socket, input, sizeof(input));
        printf("[subserver %d] received input {%s}\n", pid, input);

        //only first character inputed will be counted as letter guess
        letter = input[0];
        //update k because a guess was made
        k = 0;

	//if the input was empty
        if (letter == '\0'){
	    strcpy(message,"An empty character is not a letter");
	    write(client_socket, message, BUFFER_SIZE);
	    printf("[subserver %d] Sent %s\n", pid, message);
	    test = read(client_socket, buffer, BUFFER_SIZE);
	    if (test == -1 || strcmp(buffer, ACK)) {
	        printf("Error 3.7!");
	    }
	    buffer = zero_heap(buffer, BUFFER_SIZE);
	    message = zero_heap(message, BUFFER_SIZE);
	    k = 1;
	}
  
	//if the character inputted was uppercase
	else if (strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ",letter) != NULL) {
	    strcpy(message,"Please input a lowercase letter next time");
	    write(client_socket, message, BUFFER_SIZE);
	    printf("[subserver %d] Sent %s\n", pid, message);
	    test = read(client_socket, buffer, BUFFER_SIZE);
	    if (test == -1 || strcmp(buffer, ACK)) {
	        printf("Error 4!");
	    }
	    buffer = zero_heap(buffer, BUFFER_SIZE);
	    message = zero_heap(message, BUFFER_SIZE);
	    letter = tolower(letter);
	}

	//if the guess was not a letter
	if (strchr("abcdefghijklmnopqrstuvwxyz",letter) == NULL) {
	    strcpy(message,"Not a valid letter");
	    write(client_socket, message, BUFFER_SIZE);
	    printf("[subserver %d] Sent %s\n", pid, message);
	    test = read(client_socket, buffer, BUFFER_SIZE);
	    if (test == -1 || strcmp(buffer, ACK)) {
	        printf("Error 5!");
	    }
	    buffer = zero_heap(buffer, BUFFER_SIZE);
	    message = zero_heap(message, BUFFER_SIZE);
	    k = 1;
	}
	else{
	    i = 0;
	    for (;i < g;i++) {
	        //if the letter was already guessed
	        // set k to 1 to prompt guess again
	        if (guessed_letters[i] == letter) {
		    k = 1;
		    strcpy(message,"Letter was previously guessed. Guess again.");
		    write(client_socket, message, BUFFER_SIZE);
		    printf("[subserver %d] Sent %s\n", pid, message);
		    test = read(client_socket, buffer, BUFFER_SIZE);
		    if (test == -1 || strcmp(buffer, ACK)) {
		        printf("Error lost count");
		    }
		    buffer = zero_heap(buffer, BUFFER_SIZE);
		    message = zero_heap(message, BUFFER_SIZE);
		}
	    }
	}
    }

    //update guessed_letters array with new guess
    guessed_letters[g] = letter;
    g++;


    //compare letter to each letter in word
    int j = 0;
    //boolean for if letter guessed was in word
    int t = 0;
    for (;j < len;j++) {
        if (word[j] == letter) {
            t = 1;
            guessing_array[j] = letter;
        }
    }

    //update wrong guess count if needed
    // if t is 0, the letter guessed was not in the word
    if (!t) {
        wrong_guesses++;
    }

    //show the current player what they accomplished
    //print the man
    hangman = (char *) calloc(BUFFER_SIZE,sizeof(char));
    strcpy(hangman,"man");
    strcat(hangman,generate_man(wrong_guesses));
    write(client_socket, hangman, BUFFER_SIZE);
    printf("[subserver %d] Sent %s\n", pid, hangman);
    test = read(client_socket, buffer, BUFFER_SIZE);
    if (test == -1 || strcmp(buffer, ACK)) {
        printf("Error 0.5!");
    }
    hangman = zero_heap(hangman, BUFFER_SIZE);
    buffer = zero_heap(buffer, BUFFER_SIZE);

    //print the blank spaces for the word, with correct guesses filled in
    i = 0;
    if (guessing_array[0] != 0) {
        strcpy(message, "guessing");
        strcat(message, guessing_array);
        write(client_socket,message,BUFFER_SIZE);
        printf("[subserver %d] Sent %s\n", pid, message);
        message = zero_heap(message, BUFFER_SIZE);
        test = read(client_socket, buffer, BUFFER_SIZE);
        if (test == -1 || strcmp(buffer, ACK)) {
            printf("Error 2!");
        }
        buffer = zero_heap(buffer, BUFFER_SIZE);
    }

    //set all the shared memory stuff
    int shmid_guessing = shmget(GUESSING_ARRAY_KEY2, (sizeof(char) * 20),0600);
    if (shmid_guessing == -1) {
        printf("ERROR 10\n");
    } else {
        set_shm(guessing_array,shmid_guessing);
    }

    int shmid_guessed = shmget(GUESSED_LETTER_KEY2, (sizeof(char) * 26),0600);
    if (shmid_guessed == -1) {
        printf("ERROR 11\n");
    } else {
        set_shm_array(guessed_letters,shmid_guessed);
    }

    int shmid_g = shmget(G_KEY2, (sizeof(int)),0600);
    if (shmid_g == -1) {
        printf("ERROR 12\n");
    } else {
        set_shm_int(g,shmid_g);
    }

    int shmid_wrong = shmget(WRONG_GUESSES_KEY2, (sizeof(int)),0600);
    if (shmid_wrong == -1) {
        printf("ERROR 13\n");
    } else {
        set_shm_int(wrong_guesses,shmid_wrong);
    }

    *index = g;
    *wrong_guessespointer = wrong_guesses;

    //check for blank spaces in guessing_array
    // to see if the word was fully guessed already
    i = 0;
    //boolean for checking blank spaces
    b = 0;
    for (;i < len; i++) {
        if (guessing_array[i] == '_') {
            b = 1;
            break;
        }
    }

    //if b is 0, there were no blank spaces
    // word was already guessed, return -2
    // to indicate won
    if (!b) {
        return -2;
    }

    //if wrong_guesses is 6, man was hung
    // return -3 to indicate lost
    if (wrong_guesses == 6) {
        return -3;
    } 

    //if letter guessed was in word
    // return -4 to prompt turn again
    if (t) {
        return -4;
    }

    return wrong_guesses;
}

void run_game_competitive(char* word, int client_socket){
    //word for the game
    printf("WORD: %s\n\n",word);

    int test;
    int pid = getpid();
    char * message = (char *) calloc(BUFFER_SIZE, sizeof(char));
    char * buffer = (char *) calloc (BUFFER_SIZE, sizeof(char));
    char * hangman;

    int wrong_guesses = 0;
    int len = strlen(word);
    //array for guessing the word, intially blank
    char * guessing_array = blank_array(len);
    //input (guess) with potentially multiple characters and newline
    char input[100];
    //letter guessed
    char letter;
    //array and counter for guessed letters
    char * guessed_letters = calloc(26,sizeof(char));
    //index of guessed_letters array
    int g = 0;
    printf("[subserver %d] running game...\n", pid);
    int won = 0;

    //set the shared memory stuff
    int shmid_guessing = create_shm(GUESSING_ARRAY_KEY2);
    if (shmid_guessing == -1) {
        shmid_guessing = shmget(GUESSING_ARRAY_KEY2, (sizeof(char) * 20),0600);
    } else {
        set_shm(guessing_array,shmid_guessing);
    }

    int shmid_guessed = create_shm_array(GUESSED_LETTER_KEY2);
    if (shmid_guessed == -1) {
        shmid_guessed = shmget(GUESSED_LETTER_KEY2, (sizeof(char) * 26),0600);
    } else {
        set_shm_array(guessed_letters,shmid_guessed);
    }

    int shmid_g = create_shm_int(G_KEY2);
    if (shmid_g == -1) {
        shmid_g = shmget(G_KEY2, (sizeof(int)),0600);
    } else {
        set_shm_int(g,shmid_g);
    }

    int shmid_wrong = create_shm_int(WRONG_GUESSES_KEY2);
    if (shmid_wrong == -1) {
        shmid_wrong = shmget(WRONG_GUESSES_KEY2, (sizeof(int)),0600);
    } else {
        set_shm_int(wrong_guesses,shmid_wrong);
    }

    int semid = semget(KEY2,1,0600);

    int semval = view_sem(semid);
    //printf("semval is %d \n",semval);

    //blocking while semval is not 0
    // meaning there is not enough players
    while (semval) {
        semval = view_sem(semid);
        sleep(.5);
    }

    //semaphore for turns
    int competesemid = create_sem(COMPETEKEY,1);

    if (competesemid == -1) {
        //printf("semaphore error: %s\n",strerror(errno));
        //get the semid if semaphore already made
        competesemid = semget(COMPETEKEY,1,0600);
    }

    //semaphore for allowing the other client to
    // know what is happening between turns
    int turnsemid = create_sem(TURNKEY, 1);

    if (turnsemid == -1) {
        //printf("semaphore error: %s\n",strerror(errno));
        //get the semid if already made
        turnsemid = semget(TURNKEY,1,0600);
    }

    int competesemval;
    int turnsemval;

    while (1) {
        sleep(.1);
        competesemval = view_sem(competesemid);

        turnsemval = view_sem(turnsemid);

        //if other player took a turn and is going again
        // find out what happened
        if(competesemval == 0 && turnsemval == 1){
            get_status(len, client_socket);
            decrement_sem(turnsemid);
            sleep(.1);
        }

        if (competesemval) {
            //taking a turn
            decrement_sem(competesemid);

            //its possible turnsemid wasn't incremented yet
            // its possible it was decremented by other client
            // before turn was ran
            turnsemval = view_sem(turnsemid);
            if(turnsemval == 0){
                sleep(.1);
                increment_sem(turnsemid);
            }

            decrement_sem(turnsemid);

            won = -4;
            while(won == -4){
                //get all the shared memory stuff
                shmid_guessing = shmget(GUESSING_ARRAY_KEY2, (sizeof(char) * 20),0600);
                if (shmid_guessing == -1) {
                    printf("ERROR 10\n");
                } else {
                    guessing_array = get_shm(shmid_guessing);
                }

                shmid_guessed = shmget(GUESSED_LETTER_KEY2, (sizeof(char) * 26),0600);
                if (shmid_guessed == -1) {
                    printf("ERROR 11\n");
                } else {
                    guessed_letters = get_shm_array(shmid_guessed);
                }

                shmid_g = shmget(G_KEY2, (sizeof(int)),0600);
                if (shmid_g == -1) {
                    printf("ERROR 12\n");
                } else {
                    g = get_shm_int(shmid_g);
                }

                shmid_wrong = shmget(WRONG_GUESSES_KEY2, (sizeof(int)),0600);
                if (shmid_wrong == -1) {
                    printf("ERROR 13\n");
                } else {
                    wrong_guesses = get_shm_int(shmid_wrong);
                }

                //making the actual turn call
                won = run_turn_competitive(len,&wrong_guesses, guessing_array,
                        guessed_letters, &g, word, client_socket);

                //means player is going again, update other player
                if(won == -4){
                    increment_sem(turnsemid);
                    //wait for other player to finish update
                    sleep(.8);
                }
            }
            //check if player lost/won
            // -3 means lost, -2 means won
            // this allows the other player to know what happened
            if (won == -2 || won == -3) {
                increment_sem(competesemid);
                increment_sem(turnsemid);
                //wait for other player to know what happened
                sleep(.5);
            }
            //for second player exiting turn early
            if (won == -8 || won == -9) {
                increment_sem(competesemid);
                increment_sem(turnsemid);
                return;
            }
            if (won == -3) {
                strcpy(message, "Sorry, you lose!");
                write(client_socket, message, BUFFER_SIZE);
                printf("[subserver %d] Sent %s\n", pid, message);
                test = read(client_socket, buffer, BUFFER_SIZE);
                if (test == -1 || strcmp(buffer, ACK)) {
                    printf("Error 6!");
                }
                buffer = zero_heap(buffer, BUFFER_SIZE);
                message = zero_heap(message, BUFFER_SIZE);
		strcpy(message,"The word was: ");
		strcat(message,word);
		write(client_socket, message, BUFFER_SIZE);
		printf("[subserver %d] Sent %s\n", pid, message);
		test = read(client_socket, buffer, BUFFER_SIZE);
		if (test == -1 || strcmp(buffer, ACK)) {
		    printf("Error 11!");
		}
		buffer = zero_heap(buffer, BUFFER_SIZE);
		message = zero_heap(message, BUFFER_SIZE);
		
                //wait for other client to finish
                while(!view_sem(competesemid)){
                    sleep(.1);
                }

                remove_shm(shmid_guessing);
                remove_shm(shmid_guessed);
                remove_shm(shmid_g);
                remove_shm(shmid_wrong);
                return;
            }
            if (won == -2) {
                strcpy(message,"You win!");
                write(client_socket, message, BUFFER_SIZE);
                printf("[subserver %d] Sent %s\n", pid, message);
                test = read(client_socket, buffer, BUFFER_SIZE);
                if (test == -1 || strcmp(buffer, ACK)) {
                    printf("Error 7!");
                }
                buffer = zero_heap(buffer, BUFFER_SIZE);
                message = zero_heap(message, BUFFER_SIZE);

                //wait for other client to finish
                while(!view_sem(competesemid)){
                    sleep(.1);
                }

                remove_shm(shmid_guessing);
                remove_shm(shmid_guessed);
                remove_shm(shmid_g);
                remove_shm(shmid_wrong);
                return;
            }

            //increment the sem to give the other player a chance
            increment_sem(competesemid);
            increment_sem(turnsemid);
            sleep(.5);
        }

    }

}

