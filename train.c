#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

/* Les villes */
#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5



void* train_de_A_vers_D(void* arg);
void* train_de_A_vers_C(void* arg);
void* train_de_F_vers_A(void* arg);
void* train_de_E_vers_B(void* arg);

pthread_mutex_t mut_AB;
pthread_mutex_t mut_BC;
pthread_mutex_t mut_CD;
pthread_mutex_t mut_FA;
pthread_mutex_t mut_EB;


int tirage_aleatoire(double max)
{
        int j=(int) (max*rand()/(RAND_MAX+1.0));
        if(j<1)
                j=1;
        return j;
}



void attendre(int max)
{
        struct timespec delai;

        delai.tv_sec=tirage_aleatoire(max);
        delai.tv_nsec=0;
        nanosleep(&delai,NULL);
}


/* Gestion du chemin de fer  */
void utiliser_le_segment(int ville_depart, int ville_arrivee)
{
	if  ( (ville_depart == A) && (ville_arrivee == B) )
		pthread_mutex_lock(&mut_AB);
	else
		if  ( (ville_depart == B) && (ville_arrivee == C) )
			pthread_mutex_lock(&mut_BC);
		else
			if  ( (ville_depart == C) && (ville_arrivee == D) )
				pthread_mutex_lock(&mut_CD);
			else
				if  ( (ville_depart == E) && (ville_arrivee == B) )
					pthread_mutex_lock(&mut_EB);
				else
					pthread_mutex_lock(&mut_FA);
			
			
}


void liberer_le_segment(int ville_depart, int ville_arrivee)
{
	if  ( (ville_depart == A) && (ville_arrivee == B) )
		pthread_mutex_unlock(&mut_AB);
	else
		if  ( (ville_depart == B) && (ville_arrivee == C) )
			pthread_mutex_unlock(&mut_BC);
		else
			if  ( (ville_depart == C) && (ville_arrivee == D) )
				pthread_mutex_unlock(&mut_CD);
			else
				if  ( (ville_depart == E) && (ville_arrivee == B) )
					pthread_mutex_unlock(&mut_EB);
				else
					pthread_mutex_unlock(&mut_FA);
}


int main(int argc, char* argv[])
{
	int i;
	pthread_t id;


	pthread_mutex_init(&mut_AB,0);
	pthread_mutex_init(&mut_BC,0);
	pthread_mutex_init(&mut_CD,0);
	pthread_mutex_init(&mut_FA,0);
	pthread_mutex_init(&mut_EB,0);
	
	for(i =0; i< 3; i++)
		{
		pthread_create(&id, NULL, train_de_A_vers_D, NULL);
		pthread_create(&id, NULL, train_de_A_vers_C, NULL);
		pthread_create(&id, NULL, train_de_F_vers_A, NULL);
		pthread_create(&id, NULL, train_de_E_vers_B, NULL);
		}

	pthread_exit(NULL);
}


void* train_de_A_vers_D(void* arg)
{
	utiliser_le_segment(A, B);
	printf("Train %lu : utilise segment AB \n",pthread_self());
	attendre(6);
	liberer_le_segment(A, B);

	utiliser_le_segment(B, C);
	printf("Train %lu : utilise segment BC \n",pthread_self());
	attendre(6);
	liberer_le_segment(B, C);
	
	utiliser_le_segment(C, D);
	printf("Train %lu : utilise segment CD \n",pthread_self());
	attendre(6);
	liberer_le_segment(C, D);

	pthread_exit(NULL);
}

void* train_de_A_vers_C(void* arg)
{
	utiliser_le_segment(A, B);
	printf("Train %lu : utilise segment AB \n",pthread_self());
	attendre(6);
	liberer_le_segment(A, B);

	utiliser_le_segment(B, C);
	printf("Train %lu : utilise segment BC \n",pthread_self());
	attendre(6);
	liberer_le_segment(B, C);
	
	
	pthread_exit(NULL);
}
void* train_de_F_vers_A(void* arg)
{
	utiliser_le_segment(F, A);
	printf("Train %lu : utilise segment FA \n",pthread_self());
	attendre(6);
	liberer_le_segment(F, A);


	pthread_exit(NULL);
}
void* train_de_E_vers_B(void* arg)
{
	utiliser_le_segment(E, B);
	printf("Train %lu : utilise segment EB \n",pthread_self());
	attendre(6);
	liberer_le_segment(E, B);


	pthread_exit(NULL);
}






