#include <stdio.h>
#include <stdlib.h>
#define CONSONNE 1
#define VOYELLE 0

u_int8_t compteur (char *mdp,int consouvoye){
  u_int8_t voy = 0 ;
  u_int8_t cons = 0;
  while (*mdp){
    switch (*mdp){
      case 'a': case'e' : case'i' : case'o' : case'u' : case'y':
        voy++;
        break;
    }
    *mdp++;
    cons ++;
}if(consouvoye==0){
  return voy;
}
  return (cons-voy) ;
}
//u_int8_t max(u_int8_t a, u_int8_t b){
  //if (a <= b){
    //return (b);
  //}
  //else {
    //return (a);
  //}
//}
//int main (int argc, char *argv[]){
  //char *c = argv[1];
  //char* mot= "eeebc";
  //printf("%d\n", compteur(mot,1) );
  //return 0;
//}


