#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include "candidat.c"
#include "printMDP.c"
#include "addMDP.c"
#include "nbreCV.c"


int main(int argc, char const *argv[]) {
  // initialise 3 strings pour tester
  char* t1="coucou";
  char* t2="antoine";
  char* t3="romain";
  // compte les voyelles ou consonnes des strings tests
  int test1=nbreCV(t1,0);
  int test2=nbreCV(t2,1);
  int test3=nbreCV(t3,0);
  printf("\nTest de nbreCV:\n \n");
  printf("   Avec %s, \tnombre voyelles \t=4, \tfonction retourne: %d \n",t1,test1);
  printf("   Avec %s, \tnombre consonnes \t=3, \tfonction retourne: %d \n",t2,test2);
  printf("   Avec %s, \tnombre voyelles \t=3, \tfonction retourne: %d \n\n",t3,test3);
  printf("Test de addMDP:\n\n");
  // ajoute 3 candidats
  addMDP(t1);
  addMDP(t2);
  addMDP(t3);
  printf("  Voici la liste crée par addMDP: \n \n ");
  struct candidat *runner=candidatHead;
  while(runner!=NULL){
	  printf("\t%s\n",runner->mdp);
	  runner=runner->next;
  }
  printf("\n");
  printf("  La liste attendue était:\n\n \t%s\n \t%s\n \t%s\n\n",t3,t2,t1);
  printf("\nTest de printMDP:\n \n");
  printf("  Voici ce que printMDP affiche: \n \n ");
  printMDP();
  printf("\n");
  printf("  Voici ce que printMDP devrait afficher: \n\n%s\n%s\n%s\n\n",t3,t2,t1);
  return 0;
}
