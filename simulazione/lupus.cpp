#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>        // std::time

using namespace std;

#define lenght 30
#define len 30
#define hypno 14

//typedef basic_string<char> string;



typedef enum { CACCIATORE, CONTADINO, CUSTODE, ESORCISTA, GUARDIA, MAGO, STALKER, VEGGENTE, VOYEUR, LUPO, DIAVOLO, SEQUESTRATORE, INDEMONIATO, MEDIUM, IPNOTISTA, NEGROMANTE, SPETTRO, SPETTRO_MORTE, SPETTRO_OCCULTAMENTO} ruolo_t;

class personaggio {	
	public:
		personaggio( bool bnfigo=0, int faz=0, int mst=0, string s="null", int rlnum=-1 ) :appenaagito(0), buonofigo(bnfigo), custodito(0), esorcizzato(0), fazione(faz), mistico(mst), occultato(0), ruolo(s), ruolo_num(rlnum), sequestrato(0), spettro(0), vivo(1) {}; 
		int ruolo_num;
		string ruolo;
		int fazione; // 0 = buono, 1 = cattivo, 2 = non -morto 
		bool vivo;
		bool mistico;
		bool spettro;
		vector<personaggio*> gente;
		bool buonofigo;
		bool sequestrato;
		bool custodito;
		bool appenaagito;
		bool esorcizzato;
		bool occultato;
		virtual int notte (personaggio* personaggio){
		return 0;};
};

typedef personaggio p;

vector<p*> cattivinoti;
vector<p*> misticinoti;
vector<p*> notialdiavolo;
vector<p*> killerevittima;
p* uccisodallospettro;
bool appenaspettrificato;
int numerospettri = 0;
int vittorieneri = 0;
int vittoriebianchi = 0;
int vittoriegrigi = 0;


bool morto(p* persona){
		return !(persona->vivo);
		};

void reset(p* personaggi){
	for(int i=0; i<len; i++){
		(personaggi+i)->sequestrato = 0;
		(personaggi+i)->custodito = 0;
		(personaggi+i)->esorcizzato = 0;
		(personaggi+i)->occultato = 0;
		(personaggi+i)->gente.erase(remove_if((personaggi+i)->gente.begin(),(personaggi+i)->gente.end(), morto),(personaggi+i)->gente.end());
	};
	
	uccisodallospettro=NULL;
	killerevittima[0]=NULL;
	killerevittima[1]=NULL;
			
};


unsigned numerobianchi(p* personaggi){
	int counter=0;
	for(int i=0; i<len; i++)
		if((personaggi+i)->fazione == 0 && (personaggi+i)->vivo)
			counter++;
	return counter;
};

unsigned numeroneri(p* personaggi){
	int counter=0;
	for(int i=0; i<len; i++)
		if((personaggi+i)->fazione == 1 && (personaggi+i)->vivo){
			counter++;
		}
	return counter;
};		

unsigned numerogrigi(p* personaggi){
	int counter=0;
	for(int i=0; i<len; i++)
		if((personaggi+i)->fazione == 2 && (personaggi+i)->vivo)
			counter++;
	return counter;
};

void kill (p* ucciso) {
	ucciso->vivo = 0;
};


unsigned numerovivi(p* personaggi)
{int num=0;
for( int i=0; i<len; i++)
num += personaggi[i].vivo;
return num;
};

p* randtag(int ruolo, int fazione, int vivo, int mistico, p* pers) //i numeri negativi indicano "indifferente"
{
	p* per[len];
	for (int i=0; i<len; i++) per[i]=pers+i;
	// vector<p*> punt;
	// punt.assign (per, per + len);
	// random_shuffle(punt.begin(), punt.end());
	for (int i=0; i<len; i++)
	{
		int j=rand()%(len-i);
		swap(per[i],per[i+j]);
		p* pe=per[i];
		if( (pe->ruolo_num== ruolo || ruolo < 0) && (pe->fazione == fazione || fazione < 0) && (pe->mistico == mistico || mistico<0) && (pe->vivo==vivo || vivo <0))
			return pe;
	};
	return NULL;
};

class cacciatore: public personaggio {
	public:
	cacciatore() : personaggio(0,0,0,"cacciatore",CACCIATORE) {};
	
	
	int notte (p* personaggi)
		{
		if(!(this->vivo))
			randtag(-1, -1, 1, -1, personaggi)->vivo=0;
		return 0;
		};
};

class contadino: public personaggio {
	public:
			contadino(bool m=0) : personaggio(0,0,m,"contadino",CONTADINO) {};
	};

class custode: public personaggio {
	public:
	custode() : personaggio(0,0,0,"custode",CUSTODE) {};
	int notte (p* personaggi)
			{
			randtag(-1, -1, 0, -1, personaggi)->custodito=1;
			return 0;
			};
};

class esorcista: public personaggio {
	public:
	esorcista() : personaggio(0,0,1,"esorcista",ESORCISTA) {};
	int notte (p* personaggi)
				{p* esorciz;
				esorciz = randtag(-1, -1, 1, -1, personaggi);
				esorciz->esorcizzato=1;
				if(esorciz == uccisodallospettro)
					esorciz->vivo= 1;
				return 0;
				};
};

class guardia: public personaggio {
		public:
		guardia() : personaggio(1,0,0,"guardia",GUARDIA) {};
		int notte (p* personaggi)
			{
			killerevittima[1]->vivo=1;	
			p* protetto = randtag(-1, -1, 1, -1, personaggi);
			if(killerevittima[1]!=protetto)
				killerevittima[1]->vivo=0;
			return 0;
			};
};

class mago: public personaggio {
	public:
	mago() : personaggio(0,0,1,"mago",MAGO) {};
	int notte (p* personaggi) {
			vector<p*>::iterator it;
			gente.erase(remove_if(gente.begin(),gente.end(), morto),gente.end());
			if(gente.size()+1 >= numerovivi(personaggi))
				return 0;
			p* scrutato;
			do
			scrutato = randtag(-1, -1, 1, -1, personaggi);
			while (find(gente.begin(), gente.end(),scrutato)!=gente.end() && scrutato!=this);
			if(scrutato->mistico)
			misticinoti.push_back(scrutato);
			gente.push_back(scrutato);
			return 0;
			};
};

class stalker: public personaggio {
	public:
	stalker() : personaggio(1,0,0,"stalker",STALKER) {};
	int notte (p* personaggi) {
			if (appenaagito) {
				appenaagito = 0;
				return 0;
			};
			p* stalkato = randtag(-1, -1, 1, -1, personaggi);
			if(stalkato == killerevittima[0])
				if(!killerevittima[1]->vivo)
					if(find(cattivinoti.begin(),cattivinoti.end(),killerevittima[0])==cattivinoti.end())
						cattivinoti.push_back(killerevittima[0]);
			return 0;
		};
};

class voyeur: public personaggio {
	public:
	voyeur() : personaggio(1,0,0,"voyeur",VOYEUR) {};
	int notte (p* personaggi) {
			if (appenaagito) {
				appenaagito = 0;
				return 0;
			};
			p* visto = randtag(-1, -1, 1, -1, personaggi);
			if(visto == killerevittima[1] && !killerevittima[1]->vivo)
				if(find(cattivinoti.begin(),cattivinoti.end(),killerevittima[0])==cattivinoti.end())
					cattivinoti.push_back(killerevittima[0]);
			return 0;
	};
};


class veggente: public personaggio {
	public:
	veggente() : personaggio(1, 0,1,"veggente",VEGGENTE) {};
	int notte (p* personaggi){
			vector<p*>::iterator it;
			gente.erase(remove_if(gente.begin(),gente.end(), morto),gente.end());
			if(gente.size()+1 >= numerovivi(personaggi))
				return 0;
			p* scrutato;
			do
				scrutato = randtag(-1, -1, 1, -1, personaggi);
			while (find(gente.begin(), gente.end(),scrutato)!=gente.end() && scrutato!=this);
			if(scrutato->fazione == 1)
			cattivinoti.push_back(scrutato);
			gente.push_back(scrutato);
			return 0;
			};
};

class lupo: public personaggio {
	public:
	lupo() : personaggio(0,1,0,"lupo",LUPO) {};
};

void lupi (p* personaggi) {
	if(randtag(-1,0,1,-1,personaggi)==NULL && randtag(-1,2,1,-1,personaggi)==NULL)
		return;
	p* killer = randtag(LUPO, -1, 1, -1, personaggi);
	if(killer == NULL)
		return;
	p* vittima;
	killerevittima.insert(killerevittima.begin(), killer);
	if(notialdiavolo.size()!=0) {
			vittima=notialdiavolo[0];
			kill(notialdiavolo[0]);
			notialdiavolo.erase(notialdiavolo.begin());
			}
	else {
	do			
		vittima = randtag(-1, -1,1,-1,personaggi);
	while(vittima -> fazione == 1);	
	kill(vittima);
	};
	killerevittima.insert(killerevittima.begin()+1, vittima);
	if(killerevittima[1]->ruolo_num == NEGROMANTE || (killerevittima[1]->ruolo_num == IPNOTISTA && (len-numerovivi(personaggi))>=3))
		killerevittima[1]->vivo = 1;
};

class diavolo: public personaggio {
	public:
	diavolo() : personaggio(0,1,1,"diavolo",DIAVOLO) {};
	int notte (p* personaggi)
			{for (unsigned i=0; i<gente.size(); i++){
				if (gente[i]->vivo == 0)
				gente.erase(gente.begin()+i);
				};
			if(gente.size()+1 >= numerovivi(personaggi))
				return 0;
			p* scrutato;
			do
			scrutato = randtag(-1, -1, 1, -1, personaggi);
			while (find(gente.begin(), gente.end(),scrutato)!=gente.end() && scrutato!=this);
			if(scrutato->buonofigo)
			notialdiavolo.push_back(scrutato);
			gente.push_back(scrutato);
			return 0;
			};
};

class sequestratore: public personaggio {
	public:
	sequestratore() : personaggio(0,1,0,"sequestratore", SEQUESTRATORE) {};
	int notte (p* personaggi){
		randtag(-1, -1, 1, -1, personaggi)->sequestrato=1;
		if (killerevittima[0]->sequestrato)
			killerevittima[1]->vivo=1;
		return 0;
	};
};

class indemoniato: public personaggio {
	public:
	indemoniato() : personaggio(0,1,0,"indemoniato",INDEMONIATO) {};
};

class medium: public personaggio {
	public:
	medium() : personaggio(0,2,1,"medium",MEDIUM) {};
};
	
	
void notte_ipnotista(p* personaggi, p* ipnotista, vector<p*>* ipnotizzati){
		p* ipnotizzato_nuovo;
		do
			ipnotizzato_nuovo=randtag(-1,-1,1,-1, personaggi);
		while(ipnotizzato_nuovo-> fazione ==2 || find((*ipnotizzati).begin(), (*ipnotizzati).end(), ipnotizzato_nuovo)!=(*ipnotizzati).end());
		(*ipnotizzati).push_back(ipnotizzato_nuovo);
};	

class ipnotista: public personaggio { /*WARNING: LA STRATEGIA DELL'IPNOTISTA E' SCRITTA QUI!*/
	public:
	ipnotista() : personaggio(0,2,0,"ipnotista",IPNOTISTA) {};
	int notte (p* personaggi){
		if(gente.size() > 1 && rand()%2 == 0 && vivo && find(cattivinoti.begin(),cattivinoti.end(),(personaggio*) this)==cattivinoti.end())
		cattivinoti.push_back(this);
		vector<p*>::iterator it;
		gente.erase(remove_if(gente.begin(),gente.end(), morto),gente.end());
		if (appenaagito) {
				appenaagito = 0;
				return 0;
			};	
		if(numerovivi(personaggi) < hypno)	
		notte_ipnotista(personaggi, this, &gente);	
	return 0; 	
	};		
};		

class negromante: public personaggio {
	public:
	negromante() : personaggio(0,2,1,"negromante",NEGROMANTE) {};
};		

void notte_negromanti(p* personaggi) {
if(numerospettri >= 4)
	return;	
if(len - numerovivi(personaggi) < 2)
	return;
if(appenaspettrificato){
	appenaspettrificato = 0;
	return;
	};
p* futuro_spettro;
do
		futuro_spettro=randtag(-1, -1, 0, -1, personaggi);
while(futuro_spettro->fazione == 2);
if(futuro_spettro->ruolo_num == LUPO || futuro_spettro->custodito)
			return;
switch ( numerospettri) {
	case 1 :
		futuro_spettro->fazione = 2;
		futuro_spettro -> ruolo_num = SPETTRO_OCCULTAMENTO;
		break;
	case 2 :
	case 3 :
		futuro_spettro->fazione=2;
		futuro_spettro -> ruolo_num = SPETTRO;
		break;
	case 4 :
		futuro_spettro->fazione=2;
		futuro_spettro->ruolo_num = SPETTRO_MORTE;
		break;
	default:
		return;
		break;
		};
return;
};

void spettro_morte (p* personaggi){
		if(randtag(-1, 0, 1, -1, personaggi)==NULL || randtag(-1,1,1,-1,personaggi)==NULL)
			return;
		p* vittima;	
		do			
			vittima = randtag(-1, -1,1,-1,personaggi);
		while(vittima -> fazione == 2);	
		if(vittima->ruolo_num!=LUPO){
			kill(vittima);
			uccisodallospettro = vittima;	
		};
};



int vittoria(p* personaggi){ //ritorna la fazione che ha vinto, -1 se non ha vinto nessuno
	if(randtag(NEGROMANTE, -1, 1, -1, personaggi)== NULL)
		for(int i=0; i<len; i++)
			if((personaggi+i)->fazione== 2)
				(personaggi+i)->vivo=0;
	if(randtag(LUPO, -1, 1, -1, personaggi)==NULL)
		for(int i=0; i<len; i++)
			if((personaggi+i)->fazione== 1)
				(personaggi+i)->vivo=0;
	if(randtag(NEGROMANTE, -1, 1, -1, personaggi)== NULL && randtag(LUPO, -1, 1, -1, personaggi)==NULL){
		//cout << "Hanno vinto i popolani!" << endl;
		vittoriebianchi++;
		return 0;
	};
	if(numeroneri(personaggi)*2>numerovivi(personaggi)) {
		//cout << "Hanno vinto i cattivi!" << endl;
		vittorieneri++;
		return 1;
	};
	{/* CONDIZIONI DI VITTORIA DEI NON MORTI! */
	int ipnotizzati = 0;
	for(int i=0;i<len;i++)
		if((personaggi+i)->ruolo_num==IPNOTISTA && (personaggi+i)->vivo)
			ipnotizzati += (personaggi+i)->gente.size();
	if((numerogrigi(personaggi)+ipnotizzati)*2>numerovivi(personaggi)){
		//cout << "Hanno vinto i non-morti!" << endl;
		vittoriegrigi++;
		return 2;
	};
	};
	return -1;	
}

int nottevillagio(p* personaggi){
	if(numerospettri>=4)
		spettro_morte(personaggi);
	lupi(personaggi);
	for(int i=0; i<len; i++)
		if((personaggi+i)->vivo)
			(personaggi+i)->notte(personaggi);
	reset(personaggi);	
	return vittoria(personaggi);		
};

int giornovillaggio(p* personaggi){
	if(cattivinoti.size()!=0){
		kill(cattivinoti[0]);
		cattivinoti.erase(cattivinoti.begin());
	}
	else if(misticinoti.size()!=0){
		int ran=rand()%5;
		if(ran==0)
			kill(randtag(-1,-1,1,-1,personaggi));
		else {
			kill(misticinoti[0]);
			misticinoti.erase(misticinoti.begin());
		}
	}
	else
		kill(randtag(-1,-1,1,-1,personaggi));
	reset(personaggi);
	return vittoria(personaggi);
};	
			
void inizializza(int ruol, string s, int fazione, bool mistico, int numero,bool ruolofigo,personaggio* pers)
{for (int i=0; i<numero; i++)
	{
	(pers+i)->appenaagito=0;
	(pers+i)->buonofigo=ruolofigo;
	(pers+i)->custodito=0;
	(pers+i)->esorcizzato=0;
	(pers+i)->vivo=1;
	(pers+i)->spettro =0;
	};
};		

void listainiziale(personaggio *personaggi)
{int cacciatori= 1, contadini= 12, contadini_mistici= 2, custodi= 1, esorcisti= 1, guardie= 1, maghi= 1, stalkers= 1, veggenti= 1, voyeurs= 1, lupi= 3, diavoli= 1, sequestratori= 0, indemoniati= 0, mediums= 1, ipnotisti= 1, negromanti= 2;
int i=0;
int j=0;
for (i=0;i<sequestratori;i++){
	sequestratore temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<guardie;i++){
	guardia temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<cacciatori;i++){
	cacciatore temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<contadini;i++){
	contadino temp(0);
	personaggi[j]=temp;
	j++;
};
for (i=0;i<contadini_mistici;i++){
	contadino temp(1);
	personaggi[j]=temp;
	j++;
};
for (i=0;i<custodi;i++){
	custode temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<esorcisti;i++){
	esorcista temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<maghi;i++){
	mago temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<stalkers;i++){
	stalker temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<veggenti;i++){
	veggente temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<voyeurs;i++){
	voyeur temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<lupi;i++){
	lupo temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<diavoli;i++){
	diavolo temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<indemoniati;i++){
	indemoniato temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<mediums;i++){
	medium temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<ipnotisti;i++){
	ipnotista temp;
	personaggi[j]=temp;
	j++;
};
for (i=0;i<negromanti;i++){
	negromante temp;
	personaggi[j]=temp;
	j++;
};
}



int main()
{
killerevittima.resize(2);
std::srand ( unsigned ( std::time(0) ) );

	for(int i=0; i<10000; i++){
		personaggio personaggi[len];
		listainiziale(personaggi);
		while(true){	
			if(giornovillaggio(personaggi)>=0)
			break;
			if(nottevillagio(personaggi)>=0)
			break;
		};
	};
cout << "I buoni hanno vinto " << vittoriebianchi << " volte" << endl;
cout << "I cattivi hanno vinto " << vittorieneri << " volte" << endl;
cout << "I non-morti hanno vinto " << vittoriegrigi << " volte" << endl; 
return 0;	
}


