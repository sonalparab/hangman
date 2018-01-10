#include "game.h"
#include "pipe_networking.h"

char * blank_array(int length){
    char * array = calloc(length,sizeof(char));
    int i = 0;
    for (;i < length; i++){
        array[i] = '_';
    }
    return array;
}

void run_game(char * word, int to_client, int from_client){
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
    // string containing hang man
    char * man;
    // for messages to be sent to client
    char * message = (char *) calloc(BUFFER_SIZE, sizeof(char));
    char * buffer = (char *) calloc (BUFFER_SIZE, sizeof(char));
    int test;
    int g = 0;
    printf("[subserver] running game...\n");
    while (1){

      
        //print the man
        // sorta dangerous to write size below?
        /* man = generate_man(wrong_guesses); */
        /* write(to_client, man, sizeof(char) * 100); */
        man = (char *) malloc (8);
        sprintf(man, "%d", wrong_guesses);
        write(to_client, man, sizeof(man));
        printf("[subserver] Sent %s\n", man);
        free(man);
        test = read(from_client, buffer, BUFFER_SIZE);
	

	//why is it -72?
	//the buffer is empty??
	printf("%s\n",buffer);
	
        if (test == -1 || strcmp(buffer, ACK)) {
            printf("Error 1!");
        }
        buffer = zero_heap(buffer, BUFFER_SIZE);
	
        //print the blank spaces for the word, with correct guesses filled in
        int i = 0;
        write(to_client, guessing_array, sizeof(guessing_array));
        printf("[subserver] Sent %s\n", guessing_array);
        test = read(from_client, buffer, BUFFER_SIZE);
        if (test == -1 || strcmp(buffer, ACK)) {
            printf("Error 2!");
        }
        buffer = zero_heap(buffer, BUFFER_SIZE);

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

        //if b is 0, there were no blank spaces
        // word was already guessed, break
        if (!b) {
            break;
        }

        //UPDATE TO CHECK FOR GUESSES NOT BEING LETTERS

        int k = 1;
        while (k){

            //print the letters guessed already, if guesses were made
            i = 0;
            if (g){
                write(to_client, guessed_letters, sizeof(guessed_letters));
                printf("[subserver] Sent %s\n", guessed_letters);
                test = read(from_client, buffer, BUFFER_SIZE);
                if (test == -1 || strcmp(buffer, ACK)) {
                    printf("Error 3!");
                }
                buffer = zero_heap(buffer, BUFFER_SIZE);
            }

            strcpy(message,PROMPT);
            write(to_client, message, BUFFER_SIZE);
            printf("[subserver] Sent %s\n", message);
            message = zero_heap(message, BUFFER_SIZE);
            printf("[subserver] waiting for input");
            //prompt input for a letter
            read(from_client, input, sizeof(input));
            printf("[subserver] received input {%s}\n", input);
            //only first character inputed will be counted as letter guess
            letter = input[0];
            printf("letter %c\n", letter);
            //update k because a guess was made
            k = 0;

            if (strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ",letter) != NULL){
                strcpy(message,"Please input a lowercase letter next time");
                write(to_client, message, BUFFER_SIZE);
                printf("[subserver] Sent %s\n", message);
                test = read(from_client, buffer, BUFFER_SIZE);
                if (test == -1 || strcmp(buffer, ACK)) {
                    printf("Error 4!");
                }
                buffer = zero_heap(buffer, BUFFER_SIZE);
                message = zero_heap(message, BUFFER_SIZE);
                letter = tolower(letter);
            }

            //if the guess was not a letter
            if (strchr("abcdefghijklmnopqrstuvwxyz",letter) == NULL){
                strcpy(message,"Not a valid letter");
                write(to_client, message, BUFFER_SIZE);
                printf("[subserver] Sent %s\n", message);
                test = read(from_client, buffer, BUFFER_SIZE);
                if (test == -1 || strcmp(buffer, ACK)) {
                    printf("Error 5!");
                }
                buffer = zero_heap(buffer, BUFFER_SIZE);
                message = zero_heap(message, BUFFER_SIZE);
                k = 1;
            }

            i = 0;
            if (!k){
                for (;i < g;i++){
                    //if the letter was already guessed
                    // set k to 1 to prompt guess again
                    if (guessed_letters[i] == letter) {
                        k = 1;
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
        for(;j < len;j++){
            if(word[j] == letter){
                t = 1;
                guessing_array[j] = letter;
            }
        }

        //update wrong guess count if needed
        if(!t){
            wrong_guesses++;
        }

        //check if player lost
        if(wrong_guesses == 6){
            man = generate_man(wrong_guesses);
            strcpy(message, "Sorry, you lose!");
            write(to_client, message, BUFFER_SIZE);
            printf("[subserver] Sent %s\n", message);
            test = read(from_client, buffer, BUFFER_SIZE);
            if (test == -1 || strcmp(buffer, ACK)) {
                printf("Error 6!");
            }
            buffer = zero_heap(buffer, BUFFER_SIZE);
            message = zero_heap(message, BUFFER_SIZE);
            return;
        }
    }

    strcpy(message,"You win!");
    write(to_client, message, BUFFER_SIZE);
    printf("[subserver] Sent %s\n", message);
    test = read(from_client, buffer, BUFFER_SIZE);
    if (test == -1 || strcmp(buffer, ACK)) {
        printf("Error 7!");
    }
    buffer = zero_heap(buffer, BUFFER_SIZE);
    message = zero_heap(message, BUFFER_SIZE);
    return;
}

char * generate_man(int n){
    printf("[subserver] generating man...\n");
    char * man = (char *)calloc(100, sizeof(char));
    size_t size = 100 * sizeof(char);
    size_t line_len = strlen("       \n");
    strcpy(man, "\n  ____ \n");
    strncat(man, " |    |\n", size -= line_len + 1);
    //    printf(" O    |\n");
    //    printf("\|/   |\n");
    //    printf(" |    |\n");
    //    printf("/ \   |\n");
    //    printf("      |\n");
    //    printf("______|_\n");
    if (n == 0) {
        strncat(man, "      |\n", size-= line_len);
        strncat(man, "      |\n", size-= line_len);
        strncat(man, "      |\n", size-= line_len);
        strncat(man, "      |\n", size-= line_len);
    }
    if (n == 1) {
        strncat(man, " O    |\n", size-= line_len);
        strncat(man, "      |\n", size-= line_len);
        strncat(man, "      |\n", size-= line_len);
        strncat(man, "      |\n", size-= line_len);
    }
    if (n == 2) {
        strncat(man, " O    |\n", size-= line_len);
        strncat(man, " |    |\n", size-= line_len);
        strncat(man, " |    |\n", size-= line_len);
        strncat(man, "      |\n", size-= line_len);
    }
    if (n == 3) {
        strncat(man, " O    |\n", size-= line_len);
        strncat(man, "\\|    |\n", size-= line_len);
        strncat(man, " |    |\n", size-= line_len);
        strncat(man, "      |\n", size-= line_len);
    }
    if (n == 4) {
        strncat(man, " O    |\n", size-= line_len);
        strncat(man, "\\|/   |\n", size-= line_len);
        strncat(man, " |    |\n", size-= line_len);
        strncat(man, "      |\n", size-= line_len);
    }
    if (n == 5) {
        strncat(man, " O    |\n", size-= line_len);
        strncat(man, "\\|/   |\n", size-= line_len);
        strncat(man, " |    |\n", size-= line_len);
        strncat(man, "/     |\n", size-= line_len);
    }
    if (n == 6) {
        strncat(man, " O    |\n", size-= line_len);
        strncat(man, "\\|/   |\n", size-= line_len);
        strncat(man, " |    |\n", size-= line_len);
        strncat(man, "/ \\   |\n", size-= line_len);
    }
    strncat(man, "      |\n", size-= line_len);
    strncat(man, "______|_\n", size-= line_len);
    /* printf("%s", man); */
    return man;
}


/*int main(){
/*int main (int argc, char *argv[]){

char letter;
printf("Guess a letter: ");
scanf("%c", letter);

print_man(0);
printf("\n");
print_man(1);
printf("\n");
print_man(2);
printf("\n");
print_man(3);
printf("\n");
print_man(4);
printf("\n");
print_man(5);
printf("\n");
print_man(6);
printf("\n");*/

//run_game("fabulous");

//}*/
