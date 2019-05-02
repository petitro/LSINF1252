void* reverse(void){
	while(true){
		sem_wait(&semHFull);
		pthread_mutex_lock(&mutH);
		// prendre 1 élement buffer
		// reverse cet élément
		// stocker le mot reverse
		pthread_mutex_unlock(&mutH);
		sem_post(&semHEmpty);
	}
	// acces second buffer 
	while(true){
		sem_wait(&semMDPEmpty);
		pthread_mutex_lock(&mutMDP);
		//inserer mot 
		pthread_mutex_unlock(&mutMDP);
		sem_post(&semMDPFull);
	}
}