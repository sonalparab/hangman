#include "game.h"

char * blank_array(int length){
  char * array = calloc(length,sizeof(char));
  int i = 0;
  for(;i < length; i++){
    array[i] = '_';
  }
  return array;
}

void run(char * word){
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
  int g = 0;
  while(1){
      
    //print the man
    print_man(wrong_guesses);
    printf("\n\n");

    //print the blank spaces for the word, with correct guesses filled in
    int i = 0;
    for(;i < len; i++)
      printf(" %c",guessing_array[i]);
    printf("\n");


    //check for blank spaces in guessing_array
    // to see if the word was fully guessed already
    i = 0;
    //boolean for checking blank spaces
    int b = 0;
    for(;i < len; i++){
      if(guessing_array[i] == '_')
	b = 1;
    }

    //if b is 0, there were no blank spaces
    // word was already guessed, break
    if(!b)
      break;


    //UPDATE TO CHECK FOR GUESSES NOT BEING LETTERS

    int k = 1;
    while(k){

      //print the letters guessed already, if guesses were made
      i = 0;
      if(g){
	printf("Guessed letters: ");
	for(;i < g;i++)
	  printf("%c ",guessed_letters[i]);
	printf("\n");
      }
          
      //prompt input for a letter
      printf("Pick a new letter: ");
      scanf("%s",input);
      //only first character inputed will be counted as letter guess
      letter = input[0];
      //update k because a guess was made
      k = 0;
      printf("\n");

      i = 0;
      for(;i < g;i++){
	//if the letter was already guessed
	// set k to 1 to prompt guess again
	if(guessed_letters[i] == letter)
	  k = 1;
      }

      //if letter was new guess
      // update guessed array
      if(!k){
	guessed_letters[g] = letter;
	g++;
      }
    }
    
    
    
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
      print_man(wrong_guesses);
      printf("Sorry you lose!\n");
      return;
    }
    
  }
  printf("You win!\n");
  return;

}



  void print_man(int n){
    printf("  ____ \n");
    printf(" |    |\n");
    //printf(" O    |\n");
    //printf("\|/   |\n");
    //printf(" |    |\n");
    //printf("/ \   |\n");
    //printf("      |\n");
    //printf("______|_\n");
    if (n == 0){
      printf("      |\n");
      printf("      |\n");
      printf("      |\n");
      printf("      |\n");
    }
    if (n == 1){
      printf(" O    |\n");
      printf("      |\n");
      printf("      |\n");
      printf("      |\n");
    }
    if (n == 2){
      printf(" O    |\n");
      printf(" |    |\n");
      printf(" |    |\n");
      printf("      |\n");
    }
    if (n == 3){
      printf(" O    |\n");
      printf("\\|    |\n");
      printf(" |    |\n");
      printf("      |\n");
    }
    if (n == 4){
      printf(" O    |\n");
      printf("\\|/   |\n");
      printf(" |    |\n");
      printf("      |\n");
    }
    if (n == 5){
      printf(" O    |\n");
      printf("\\|/   |\n");
      printf(" |    |\n");
      printf("/     |\n");
    }
    if (n == 6){
      printf(" O    |\n");
      printf("\\|/   |\n");
      printf(" |    |\n");
      printf("/ \\   |\n");
    }
    printf("      |\n");
    printf("______|_\n");
  }


  int main(){
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

    run("fabulous");
  

 
  }
