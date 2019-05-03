#include <stdio.h>
#include <stdlib.h>

u_int8_t max(u_int8_t a, u_int8_t b){
  if (a <= b){
    return (b);
  }
  else {
    return (a);
  }
}


u_int8_t compteur (char *mdp){
  u_int8_t voy = 0 ;
  u_int8_t compteur = 0;
  while (*mdp){
    switch (*mdp){
      case 'a': case'e' : case'i' : case'o' : case'u' : case'y':
        voy++;
        break;
    }
    *mdp++;
    compteur ++;
}
  return(max(voy , compteur-voy));
}

int main (int argc, char *argv[]){
  char *c = argv[1];  
  printf("%d\n", compteur(c) );
  return 0;
}
