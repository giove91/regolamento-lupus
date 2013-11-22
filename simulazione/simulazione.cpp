/*
	Simulatore molto semplice di Lupus
	Autore: Giove
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

#define BUONI 0
#define CATTIVI 1
#define NONMORTI 2

int const default_sim = 500000;	// Numero di simulazioni di default
int killer_night = 7;		// Notte dalla quale è disponibile lo spettro killerù

int somma_durate = 0;		// Somma delle durate delle partite

// Variabili di simulazione
int b,c,n;
bool veggente_vivo;

inline bool moneta (int p, int q) {
	// Restituisce 1 con probabilità p/q
	if ( rand() % q < p ) return 1;
	else return 0;
}

int check_victory() {
	if ( c==0 && n==0 ) return 0;
	if ( b==0 && n==0 ) return 1;
	if ( b==0 && c==0 ) return 2;
	return -1;
}

void uccidi(int fazione) {
	switch (fazione) {
		case 0:
			// Buoni
			if ( veggente_vivo ) {
				if ( moneta(1,b) ) veggente_vivo = 0;
			}
			b--;
			break;
		
		case 1:
			// Cattivi
			c--;
			break;
		
		case 2:
			// Non-morti
			n--;
			break;
	}
}

int simula(int bb, int cc, int nn) {
	
	// Inizializzazione variabili
	b=bb;
	c=cc;
	n=nn;
	
	veggente_vivo = 1;
	
	// Simulazione vera e propria
	int i=0;
	for (i=0; i<1000; ++i) {
		// Giorno
		if ( veggente_vivo && moneta(c,b+c+n) ) {
			// Il veggente scopre un cattivo!
			uccidi(CATTIVI);
		}
		else {
			if ( moneta(b,b+c+n) ) uccidi(BUONI);
			else if ( moneta(c,c+n) ) uccidi(CATTIVI);
			else uccidi(NONMORTI);
		}
		
		// printf("Composizione dopo il giorno %d: %d %d %d\n", i+1,b,c,n);
		
		// Check victory
		int cv = check_victory();
		if ( cv != -1 ) {
			somma_durate += i;
			return cv;
		}
		
		// Notte
		if ( c > 0 && b > 0 ) {
			// I lupi uccidono
			if ( moneta(b,b+n) ) uccidi(BUONI);
		}
		if ( n > 0 && b > 0 && i >= killer_night && (i%2 == killer_night%2) ) {
			// Lo spettro killer uccide
			if ( moneta(b,b+c) ) uccidi(BUONI);
		}
		
		// printf("Composizione dopo la notte %d: %d %d %d\n", i+1,b,c,n);

		// Check victory
		cv = check_victory();
		if ( cv != -1 ) {
			somma_durate += i;
			return cv;
		}
		
	}

	return 0;
}

int main(int argc, char *argv[]) {
	
	srand (time(NULL));

	if ( argc < 4 ) {
		printf("Inserire il numero di giocatori (Buoni, Cattivi, Non-morti).\n");
		return 1;
	}
	
	// Inizializzazione variabili di simulazione
	int risultati[] = {0,0,0};
	
	int numsimulazioni;
	int numgiocatori[3];
	
	for (int i=0; i<3; ++i) {
		numgiocatori[i] = atoi( argv[i+1] );
	}
	
	if ( argc < 5 ) {
		printf("Numero di simulazioni settato al default: %d\n", default_sim);
		numsimulazioni = default_sim;
	}
	else numsimulazioni = atoi( argv[4] );
	
	
	// Simulazione
	printf("--- Inizio le simulazioni ---\n");
	for (int i=0; i<numsimulazioni; ++i) {
		// printf("Simulazione %d\n", i+1);
		risultati[ simula(numgiocatori[0], numgiocatori[1], numgiocatori[2]) ]++;
	}
	
	double percentuali[3];
	for (int i=0; i<3; ++i) {
		percentuali[i] = ((double)(100*risultati[i]))/numsimulazioni;
	}
	
	// Stampa dei risultati
	printf("Numero di simulazioni: %d\n", numsimulazioni);
	printf("Numero di giocatori:\tBuoni %d\t\tCattivi %d\tNon-morti %d\t(totale: %d)\n", numgiocatori[0], numgiocatori[1], numgiocatori[2], numgiocatori[0]+numgiocatori[1]+numgiocatori[2]);
	printf("Risultati:\t\tBuoni %.2lf%%\tCattivi %.2lf%%\tNon-morti %.2lf%%\n", percentuali[0], percentuali[1], percentuali[2]);
	printf("Durata media della partita: %.1lf giorni\n", ((double)(somma_durate))/numsimulazioni );
	return 0;
}
