/*
*programme : code.c
*cours : LSINF1252
*Enseignant : Olivier Bonaventure
*Nom : Romain Petit (48271700), Antoine Percy (485017000)
*Date : 10/05/19
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "reverse.h"
#include <getopt.h> // include de la fonction getopt

#define CONSONNE 1
#define VOYELLE 0
#define FILEERROR -1

// création d'une structure pour liste chainée des fichiers in
struct fichierIn {
  int fichier ;
  struct fichierIn *next;
}t_fichierIn;

// création d'une structure pour liste chainée mdp candidats
struct candidat {
  char *mdp ;
  struct candidat *next ;
}t_candidat;

struct candidat *candidatHead;
struct fichierIn *head ;
struct fichierIn *courant ;
int max = 0 ;
int critere = VOYELLE;
pthread_mutex_t mutH ;
pthread_mutex_t mutMDP;
sem_t semHEmpty ;
sem_t semHFull;
sem_t semMDPEmpty;
sem_t semMDPFull;
u_int8_t **bufH;
char **bufMDP;
int positionH =-1;
int positionMDP =-1;
int nThread = 1;
int StatusCount = 0;
int outfile = 1;




void addMDP(char *mdp){
  struct candidat *cand ;
  cand = malloc(sizeof(struct candidat));
  cand->mdp = mdp;
  cand->next = candidatHead;
  candidatHead = cand;
}

void delMDP (){
  struct candidat *courant;
  while (candidatHead != NULL){
    courant = candidatHead->next;
    candidatHead->next = NULL;
    free(candidatHead->mdp);
    free(candidatHead);
    candidatHead = courant;
  }
}

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

u_int8_t nbreCV (char *mdp,int consouvoye){
  u_int8_t voy = 0 ;
  u_int8_t cons = 0;
  int i = 0;
  while (*(mdp+i)){
    switch (*(mdp+i)){
      case 'a': case'e' : case'i' : case'o' : case'u' : case'y':
        voy++;
        break;
    }
    cons ++;
    i++;
  }if(consouvoye==0){
  return voy;
  }
  return (cons-voy) ;
}

void setH(u_int8_t *hash){
  sem_wait(&semHEmpty);
  pthread_mutex_lock(&mutH);
  *(bufH+(positionH+1)*sizeof(u_int8_t))= hash;
  positionH++;
  pthread_mutex_unlock(&mutH);
  sem_post(&semHFull);
}


u_int8_t* getH(){
  sem_wait(&semHFull);
  pthread_mutex_lock(&mutH);
  u_int8_t *hash;
  hash = *(bufH+positionH*sizeof(u_int8_t));
  positionH=positionH-1;
  pthread_mutex_unlock(&mutH);
  sem_post(&semHEmpty);
  return(hash);
}

void setMDP(char *mdp){

  sem_wait(&semMDPEmpty);
  pthread_mutex_lock(&mutMDP);
  *(bufMDP+(positionMDP+1)*sizeof(char))=mdp;
  positionMDP=positionMDP+1;
  pthread_mutex_unlock(&mutMDP);
  sem_post(&semMDPFull);
}

char* getMDP(){
  sem_wait(&semMDPFull);
  pthread_mutex_lock(&mutMDP);
  char *mdp;
  mdp = *(bufMDP+positionMDP*sizeof(char));
  positionMDP=positionMDP-1;
  pthread_mutex_unlock(&mutMDP);
  sem_post(&semMDPEmpty);
  return(mdp);
}

void printMDP(){
    struct candidat *runner;
    int z;
    char *n = "\n";
    runner=candidatHead;
    while(runner!=NULL){
    z=write(outfile,(void*) (runner->mdp),strlen(runner->mdp));
    if(z==-1){
        printf("erreur write");
    }
    z=write(outfile,(void*) n,strlen(n));
    if(z==-1){
        printf("erreur write");
    }
    runner=runner->next;
    }
}

void *readFile(void *arg){
  struct fichierIn *file = head;
  int i = 0;
  u_int8_t *hash;
  hash = (u_int8_t *) malloc (sizeof(u_int8_t)*32);
  while (file != NULL){
    while (read(file->fichier, (void *) &(*(hash+i)), sizeof(u_int8_t))>0){
      if (i==31){
        setH(hash);
        i=0;
        hash = (u_int8_t *) malloc (sizeof(u_int8_t)*32);
      }
      else {
        i++;
      }
    }
    file = file->next;
    free(head);
    head =file;
  }
  return(NULL);
}

void *reverse(void *arg){
  u_int8_t *hash = NULL;
  bool revHok;
	while((hash = getH())!= NULL){
    char *mdp;
    mdp = (char *)malloc(sizeof(char)*16);
		revHok=reversehash(hash,mdp,16);
    if (revHok == true){
      setMDP(mdp);
    }
    else free(mdp);
    free(hash);
  }
  return(NULL);
}

void *count (void *arg){
  char *mdp = NULL ;
  while ((mdp = getMDP())!= NULL){
    int n = nbreCV(mdp, critere );
    if (n == max){
      addMDP(mdp);
    }
    else if(n>max){
      delMDP();
      addMDP(mdp);
      max=n;
    }
    else {
      free(mdp);
    }
  }
  return(NULL);
}

int main (int argc, char **argv) {
  int c;
  int debutFichier = 1 ;
  int err =0 ;
  extern char * optarg;
  extern int opterr;


  opterr = 0;

  while ((c = getopt (argc, argv, "t:co:")) != -1)
    switch (c)
      {
      case 't':
        debutFichier ++;
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
        debutFichier ++;
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
    printf("%s\n", argv[i] );
    f->fichier = open(argv[i], O_RDONLY);
    if (f->fichier == -1){
      printf("Impossible d'accéder au fichier : %s \n", argv[i]);
      free(f);
      exit(EXIT_FAILURE);
    }
    f->next = NULL;
    add(f);
  }

  bufH =(u_int8_t **) malloc(nThread*sizeof(u_int8_t));
  bufMDP = (char **) malloc(nThread*sizeof(char));

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

  err = pthread_create(&T_read , NULL, readFile, NULL);
  if (err != 0){
     printf("err = T_read\n");
     exit(EXIT_FAILURE);
  }

  err = pthread_create(&T_compteur, NULL, count, NULL);
  if (err != 0){
     printf("err = T_count \n");
     exit(EXIT_FAILURE);
  }

  for (int i=0 ; i<nThread; i++){
    err = pthread_create(&T_calcul[i] , NULL, reverse, NULL);
    if (err != 0){
       printf("err = T_calcul %d\n" , i);
       exit(EXIT_FAILURE);
    }
  }
  printf("%s\n","ici" );
  err = pthread_join(T_read,NULL);
  if (err != 0){
     printf("err = T_read join  %d\n", err);
     exit(EXIT_FAILURE);
  }

  while (positionH != -1){}
  pthread_mutex_lock(&mutH);
  for (int i =0 ; i<nThread ; i++){
    *(bufH+i*sizeof(u_int8_t)) = NULL;
  }
  for (int i =0 ; i< nThread ; i++){
    sem_post(&semHFull);
    positionH++;
  }
  pthread_mutex_unlock(&mutH);

  for (int i=0 ; i<nThread; i++){
    err = pthread_join(T_calcul[i] , NULL);
    if (err != 0){
       printf("err = T_calcul %d join %d\n" , i, err);
       exit(EXIT_FAILURE);
    }
  }

  while(positionMDP != -1){}
  pthread_mutex_lock(&mutMDP);
  for (int i =0 ; i<nThread ; i++){
    *(bufH+i*sizeof(char)) = NULL;
  }
  sem_post(&semMDPFull);
  positionH++;
  pthread_mutex_unlock(&mutMDP);

  err = pthread_join(T_compteur,NULL);
  printf("ici1\n");
  if (err != 0){
     printf("err = T_compteur join \n");
     fprintf(stderr, "%s: %s\n", "join de T_compteur", strerror(err));
     exit(EXIT_FAILURE);
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


  printMDP();
  delMDP();
  printf("%s\n","ok" );
  if (outfile != 1){
    close(outfile);
  }
  printf("ok");
  while (head != NULL){
    printf("%d\n", head->fichier );
    courant = head->next ;
    free(head);
    head = courant;
  }
  printf("%s\n","ok" );
  printf("%s\n", "fini" );
  return 0;
}
