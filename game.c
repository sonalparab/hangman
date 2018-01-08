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
    char * message = (char *) malloc(64 * sizeof(char));
    int g = 0;
    while (1){

        //print the man
        man = generate_man(wrong_guesses);
        // sorta dangerous to write size below?
        write(to_client, man, sizeof(char) * 100);
        printf("[subserver] Sent man\n");

        //print the blank spaces for the word, with correct guesses filled in
        int i = 0;
        write(to_client, guessing_array, sizeof(guessing_array));
        printf("[subserver] Sent guessing_array\n");


        //check for blank spaces in guessing_array
        // to see if the word was fully guessed already
        i = 0;
        //boolean for checking blank spaces
        int b = 0;
        for (;i < len; i++){
            if (guessing_array[i] == '_') {
                b = 1;
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
                printf("[subserver] Sent guessed letters\n");
            }

            message = PROMPT;
            write(to_client, PROMPT, sizeof(message));
            //prompt input for a letter
            read(from_client ,input, sizeof(input));
            printf("[subserver] received input {%s}\n", input);
            //only first character inputed will be counted as letter guess
            letter = input[0];
            //update k because a guess was made
            k = 0;

            if (strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ",letter) != NULL){
                message = "Please input a lowercase letter next time\n";
                write(to_client, message, sizeof(message));
                letter = tolower(letter);
            }

            //if the guess was not a letter
            if (strchr("abcdefghijklmnopqrstuvwxyz",letter) == NULL){
                message = "Not a valid letter\n";
                write(to_client, message, sizeof(message));
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
            message = "Sorry, you lose!\n";
            write(to_client, message, sizeof(message));
            free(message);
            return;
        }

    }
    message = "You win!\n";
    write(to_client, message, sizeof(message));
    free(message);
    return;
}

char * generate_man(int n){
    char * man = (char *)calloc(100, sizeof(char));
    man =        "  ____ \n";
    size_t size = 100 * sizeof(char);
    size_t line_len = strlen("       \n");
    strncat(man, " |    |\n", size -= line_len);
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
    printf("%s", man);
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
