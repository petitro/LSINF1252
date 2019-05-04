/*
*Pour conpiler écrire la ligne de code ci-dessous
*  gcc -Wall -o getoptplagiat getoptplagiat.c -lpthread
*
*
*
*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#define CONSONNE 1
#define VOYELLE 0
#define FILEERROR -1


struct fichierIn {
  int fichier ;
  struct fichierIn *next;
}t_fichierIn;

struct candidat {
  char candidat[16] ;
  struct candidat *next ;
}t_candidat;

struct candidat *candidatHead;
struct candidat *candidatH
struct fichierIn *head ;
struct fichierIn *courant ;
int max = 0 ;
pthread_mutex_t mutH ;
pthread_mutex_t mutMDP;
sem_t semHEmpty ;
sem_t semHFull;
sem_t semMDPEmpty;
sem_t semMDPFull;


void add(struct fichierIn *f){
  if (head == NULL){
    head = f;
    courant = f;
    courant->next = NULL ;
  }
  else {
    courant->next = f;
    courant = f ;
    courant->next = NULL;
  }
}

void *readFile(){
  printf("%s\n", "entrer dans readFile" );
  return(NULL);
}

void *reverse(){
  printf("Entrée dans reverse\n");
  return(NULL);
}

void *count (){
  printf("Entré dans count\n");
  return (NULL);
}

int main (int argc, char **argv) {
  int critere = VOYELLE;
  int nThread = 1;
  int c;
  int outfile = 1;
  int debutFichier = 2 ;
  int err =0 ;


  opterr = 0;

  while ((c = getopt (argc, argv, "t:co:")) != -1)
    switch (c)
      {
      case 't':
        debutFichier ++;
        nThread = atoi(optarg);
        break;
      case 'c':
        debutFichier ++;
        critere = CONSONNE;
        break;
      case 'o':
        debutFichier++;
        outfile = open(optarg, O_WRONLY, O_CREAT);
        if (outfile == FILEERROR){
          printf ("Vous avez rentrer un mauvais argument ou un mauvais nom de fichier");
          return (1);
        }
        break;
      case '?':
        max = 10;
      default:
        max = 10;
      }

  printf ("critere = %d, nThread = %d, test output = %d, max =%d \n",
          critere, nThread, outfile, max);

  for (int i = debutFichier; i < argc; i++){
    struct fichierIn *f ;
    f = (struct fichierIn *)malloc(sizeof(struct fichierIn));
    if (f == NULL){
      exit(EXIT_FAILURE);
    }
    f->fichier = open(argv[i], O_RDONLY);
    if (f->fichier == -1){
      printf("Impossible d'accéder au fichier : %s \n", argv[i]);
      free(f);
      exit(EXIT_FAILURE);
    }
    f->next = NULL;
    add(f);
  }

  err = sem_init(&semHEmpty, 0 , nThread);
  err =sem_init(&semHFull, 0 , 0);
  err =sem_init(&semMDPEmpty, 0 , nThread);
  err =sem_init(&semMDPFull, 0 , 0);

  err = pthread_mutex_init(&mutH, NULL);
  if (err != 0){
     printf("err = mutH\n");
     exit(EXIT_FAILURE);
  }

  err = pthread_mutex_init(&mutMDP, NULL);
  if (err != 0){
     printf("err = mutMDP\n");
     exit(EXIT_FAILURE);
  }
  pthread_t T_read ;
  pthread_t T_calcul[nThread];
  pthread_t T_compteur;

  err = pthread_create(&T_compteur, NULL, &count, NULL);
  if (err != 0){
     printf("err = T_count \n");
     exit(EXIT_FAILURE);
  }

  err = pthread_create(&T_read , NULL, &readFile, NULL);
  if (err != 0){
     printf("err = T_read\n");
     exit(EXIT_FAILURE);
  }

  for (int i=0 ; i<nThread; i++){
    err = pthread_create(&T_calcul[i] , NULL, &reverse, NULL);
    if (err != 0){
       printf("err = T_calcul %d\n" , i);
       exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i<10000000; i++){

  }

  err = pthread_join(T_read,NULL);
  if (err != 0){
     printf("err = T_read join  %d\n", err);
     exit(EXIT_FAILURE);
  }

  err = pthread_join(T_compteur,NULL);
  if (err != 0){
     printf("err = T_compteur join \n");
     fprintf(stderr, "%s: %s\n", "join de T_compteur", strerror(err));
     exit(EXIT_FAILURE);
  }

  for (int i=0 ; i<nThread; i++){
    err = pthread_join(T_calcul[i] , NULL);
    if (err != 0){
       printf("err = T_calcul %d join %d\n" , i, err);
       exit(EXIT_FAILURE);
    }
  }

  err = pthread_mutex_destroy (&mutH);
  if (err != 0){
     printf("err = mutH destroy \n");
     exit(EXIT_FAILURE);
  }

  err = pthread_mutex_destroy (&mutMDP);
  if (err != 0){
     printf("err = mutMDP destroy \n");
     exit(EXIT_FAILURE);
  }

  err = sem_destroy(&semMDPFull);
  err= sem_destroy(&semMDPEmpty);
  err =sem_destroy(&semHFull);
  err = sem_destroy(&semHEmpty);



  //close(outfile);
  while (head != NULL){
    printf("%d\n", head->fichier );
    courant = head->next ;
    free(head);
    head = courant;
  }
  printf("%s\n", "fini" );
  return 0;
}