#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include<stdio.h>


#define NB_CAMIONS	5
#define NB_VOITURES	5
#define NB_VEHICULES  (NB_CAMIONS+NB_VOITURES)



void attendre(double max);
int tirage_aleatoire(double max);


/* Code des threads */
void* voiture(void* arg);
void* camion(void* arg);


sem_t sem_compteur;

pthread_mutex_t camion_sc;



void acceder_au_pont(int tonnes) 
{

	if (tonnes==15)
		{
		pthread_mutex_lock(&camion_sc);
		while(tonnes>0)
			{
			sem_wait(&sem_compteur);
			tonnes=tonnes-5;
			};
		pthread_mutex_unlock(&camion_sc);
		}
	else  
		sem_wait(&sem_compteur);
}



void quitter_le_pont(int tonnes)
{
	while(tonnes>0)
		{
		sem_post(&sem_compteur);
		tonnes=tonnes-5;
		};
}





int main(int argc, char* argv[])
{
	int i;
	pthread_t id;


	sem_init(&sem_compteur,0,3);
	
	for(i=0; i<NB_VEHICULES;i++)
			{
			int* j=(int*)malloc(sizeof(int));
			*j=i;
			if (i<NB_CAMIONS)
				pthread_create(&id,NULL,camion,j);
			else	
				pthread_create(&id,NULL,voiture,j);
			}


	pthread_exit(NULL);
};



void* voiture(void* arg)
{
	int pid=*((int*)arg);

	attendre(5.0);
	acceder_au_pont(5);
	printf("Voiture %d : je traverse le pont\n",pid);
	attendre(5.0);
	printf("Voiture %d : je quitte le pont\n",pid);
	quitter_le_pont(5);

	pthread_exit(NULL);
}

void* camion(void* arg)
{
	int pid=*((int*)arg);

	attendre(5.0);
	acceder_au_pont(15);
	printf("Camion %d : je traverse le pont\n",pid);
	attendre(5.0);
	printf("Camion %d : je quitte le pont\n",pid);
	quitter_le_pont(15);

	pthread_exit(NULL);
}





int tirage_aleatoire(double max)
{
        int j=(int) (max*rand()/(RAND_MAX+1.0));
        if(j<1)
                j=1;
        return j;
}



void attendre(double max)
{
        struct timespec delai;

        delai.tv_sec=tirage_aleatoire(max);
        delai.tv_nsec=0;
        nanosleep(&delai,NULL);
}


