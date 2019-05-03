void* reverse(void){
	while(true){
		sem_wait(&semHFull);
		pthread_mutex_lock(&mutH);
		uint8_t *hash = bufH[positionH];
		bufH[positionH]=NULL;
		positionH=positionH-1;
		pthread_mutex_unlock(&mutH);
		sem_post(&semHEmpty);
		
		char mot[16] = malloc(sizeof(char)*16);
		bool=reversehash(hash,mot,16);
		sem_wait(&semMDPEmpty);
		pthread_mutex_lock(&mutMDP);
		buffMDP[positionMDP]=mot;
		positionMDP=positionMDP+1;
		pthread_mutex_unlock(&mutMDP);
		sem_post(&semMDPFull);
	}
}