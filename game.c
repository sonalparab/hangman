#include "game.h"

void run(char * word){
  int count = 0;
  while(count < 6){
    print_man(count);
  }
  if(count == 6){
    printf("Sorry you lose!\n");
    return;
  }
  else{
    printf("You win!\n");
    return;
  }
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
  printf("\n");
}
