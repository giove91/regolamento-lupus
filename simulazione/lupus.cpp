#include <stdlib.h>
#include <list>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>        // std::time

using namespace std;

#define lenght 31
#define len 31
#define hypno 14
#define numero_critico 31

//typedef basic_string<char> string;



typedef enum { CACCIATORE, CONTADINO, CUSTODE, ESORCISTA, GUARDIA, MAGO, STALKER, VEGGENTE, VOYEUR, LUPO, DIAVOLO, SEQUESTRATORE, INDEMONIATO, MEDIUM, IPNOTISTA, NEGROMANTE, SPETTRO, SPETTRO_MORTE, SPETTRO_OCCULTAMENTO} ruolo_t;

class personaggio {	
	public:
		personaggio( bool bnfigo=0, int faz=0, int mst=0, string s="null", int rlnum=-1 ) : ruolo_num(rlnum), fazione_iniziale(faz), fazione(faz), vivo(1), mistico(mst), spettro(0), buonofigo(bnfigo), sequestrato(0), custodito(0), appenaagito(0), esorcizzato (0), occultato (0) {ruolo.assign(s); gente.push_back(this);};
		int ruolo_num;
		string ruolo;
		int fazione_iniziale;
		int fazione;				// 0 = buono, 1 = cattivo, 2 = non -morto 
		bool vivo;
		bool mistico;
		bool spettro;
		list<personaggio*> gente;
		bool buonofigo;
		bool sequestrato;
		bool custodito;
		bool appenaagito;
		bool esorcizzato;
		bool occultato;
		virtual int notte (personaggio** personaggi){ cout << "Sono un tizio generico!\n";
		return 0;};
};

typedef personaggio p;

vector<p*> cattivinoti;
vector<p*> misticinoti;
vector<p*> notialdiavolo;
p* killerevittima[2];
p* uccisodallospettro;
bool appenaspettrificato = 0;
bool appenaucciso = 0;
int numerospettri = 0;
int uccisionisensate = 0;
int misticiuccisi = 0;
int turni = 0;
int vittorieneri = 0;
int vittoriebianchi = 0;
int vittoriegrigi = 0;


bool morto(p* persona){
		if(persona->vivo)
			return false;
		//cout << "è morto!\n";
		return true;
		};

void inizializza(p* pers)
{
	pers->gente.erase(pers->gente.begin(),pers->gente.end());
	pers->fazione=pers->fazione_iniziale;
	pers->appenaagito=0;
	pers->sequestrato=0;
	pers->custodito=0;
	pers->esorcizzato=0;
	pers->vivo=1;
	pers->spettro =0;
	pers->occultato=0;
};		

void resetsessione(){
	/*for (int i=0; i<len; i++)
		inizializza((*(punt_personaggi+i)));*/
	numerospettri=0;
	cattivinoti.erase(cattivinoti.begin(),cattivinoti.end());
	misticinoti.erase(misticinoti.begin(),misticinoti.end());
	notialdiavolo.erase(notialdiavolo.begin(),notialdiavolo.end());
	killerevittima[0]=NULL;
	killerevittima[1]=NULL;
	};
	
void distruggimorti(list<p*>* persone) {
	list<p*>::iterator it,jt;
	it=(*persone).begin();
	jt=it;
	while(it != (*persone).end()){
		it++;
		if(morto(*jt)){
		//	cout << "eliminato! \n";
			(*persone).erase(jt);
		};
	jt=it;
	};		
}	

void reset(p** punt_personaggi){
	
	for(int i=0; i<len; i++){
		((*(punt_personaggi+i)))->sequestrato = 0;
		((*(punt_personaggi+i)))->custodito = 0;
		((*(punt_personaggi+i)))->esorcizzato = 0;
		((*(punt_personaggi+i)))->occultato = 0;
		if(((*(punt_personaggi+i)))->ruolo_num==	VEGGENTE){
		//	cout << "sto resettando il veggente!\n";
		//	cout << "Reset: il veggente conosce " << ((*(punt_personaggi+i)))->gente.size() << " persone\n";
		}
		distruggimorti(&(((*(punt_personaggi+i)))->gente));
		if(((*(punt_personaggi+i)))->ruolo_num==	VEGGENTE){
		//	cout << "Reset: il veggente conosce " << ((*(punt_personaggi+i)))->gente.size() << " persone\n";
		}
	};
	
	uccisodallospettro=NULL;
	killerevittima[0]=NULL;
	killerevittima[1]=NULL;
			
};

unsigned numerocustoditi(p** punt_personaggi) {
	int counter=0;
	for(int i=0; i<len; i++)
		if(((*(punt_personaggi+i)))->custodito)
			counter++;
	//cout << "Ci sono " << counter << " custoditi\n";
	return counter;
}

unsigned numerobianchi(p** punt_personaggi){
	int counter=0;
	for(int i=0; i<len; i++)
		if(((*(punt_personaggi+i)))->fazione == 0 && ((*(punt_personaggi+i)))->vivo)
			counter++;
	//cout << "Ci sono " << counter << " bianchi\n";
	return counter;
};

unsigned numeroneri(p** punt_personaggi){
	int counter=0;
	for(int i=0; i<len; i++)
		if(((*(punt_personaggi+i)))->fazione == 1 && ((*(punt_personaggi+i)))->vivo)
			counter++;
	//cout << "Ci sono " << counter << " neri\n";		
	return counter;
};		

unsigned numerogrigi(p** punt_personaggi){
	int counter=0;
	for(int i=0; i<len; i++)
		if(((*(punt_personaggi+i)))->fazione == 2 && ((*(punt_personaggi+i)))->vivo)
			counter++;
	//cout << "Ci sono " << counter << " grigi\n";		
	return counter;
};

void kill (p* ucciso) {
	ucciso->vivo = 0;
};


unsigned numerovivi(p** punt_personaggi)
{int num=0;
for( int i=0; i<len; i++)
num += (*((*(punt_personaggi+i)))).vivo;
return num;
};

p* randtag(int ruolo, int fazione, int vivo, int mistico, p** personaggi) //i numeri negativi indicano "indifferente"
{
	p* per[len];
	for (int i=0; i<len; i++) per[i]=*(personaggi+i);
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
	int notte (p** punt_personaggi)
		{
		if(!(this->vivo))
			randtag(-1, -1, 1, -1, punt_personaggi)->vivo=0;
		return 0;
		};
};

class contadino: public personaggio {
	public:
	contadino(bool m=0) : personaggio(0,0,m,"contadino",CONTADINO) {};	
	int notte (p** punt_personaggi){
		return 0;
		}
	};

class custode: public personaggio {
	public:
	custode() : personaggio(0,0,0,"custode",CUSTODE) {};
	int notte (p** punt_personaggi)
			{
			if(numerovivi(punt_personaggi)<len)
			randtag(-1, -1, 0, -1, punt_personaggi)->custodito=1;
			return 0;
			};
};

class esorcista: public personaggio {
	public:
	esorcista() : personaggio(0,0,1,"esorcista",ESORCISTA) {};
	int notte (p** punt_personaggi)
				{p* esorciz;
				esorciz = randtag(-1, -1, 1, -1, punt_personaggi);
				esorciz->esorcizzato=1;
				if(esorciz == uccisodallospettro)
					esorciz->vivo= 1;
				return 0;
				};
};

class guardia: public personaggio {
		public:
		guardia() : personaggio(1,0,0,"guardia",GUARDIA) {};
		int notte (p** punt_personaggi)
			{
			if(killerevittima[0]==NULL || killerevittima[1]==NULL)
			return 0;	
			p* protetto = randtag(-1, -1, 1, -1, punt_personaggi);
			if(killerevittima[1]==protetto)
				killerevittima[1]->vivo=1;
			return 1;
			};
};

class mago: public personaggio {
	public:
	mago() : personaggio(0,0,1,"mago",MAGO) {};
	int notte (p** punt_personaggi) {
			if(gente.size()+1 >= numerovivi(punt_personaggi))
				return 0;
			p* scrutato;
			do
			scrutato = randtag(-1, -1, 1, -1, punt_personaggi);
			while (find(gente.begin(), gente.end(),scrutato)!=gente.end() && scrutato!=this);
			if(scrutato->mistico){
				misticinoti.push_back(scrutato);
			}
				gente.push_back(scrutato);
			return 0;
			};
};

class stalker: public personaggio {
	public:
	stalker() : personaggio(1,0,0,"stalker",STALKER) {};
	int notte (p** punt_personaggi) {
			if (appenaagito) {
				appenaagito = 0;
				return 0;
			};
			appenaagito=1;
			p* stalkato = randtag(-1, -1, 1, -1, punt_personaggi);
			if(stalkato == killerevittima[0])
				if(!(killerevittima[1]->vivo)){
					//if(find(cattivinoti.begin(),cattivinoti.end(),killerevittima[0])==cattivinoti.end())
						cattivinoti.push_back(killerevittima[0]);
						//cout << "stalkato!\n";
					}
			return 0;
		};
};

class voyeur: public personaggio {
	public:
	voyeur() : personaggio(1,0,0,"voyeur",VOYEUR) {};
	int notte (p** punt_personaggi) {
			if (appenaagito) {
				appenaagito = 0;
				return 0;
			};
			appenaagito=1;
			p* visto = randtag(-1, -1, 1, -1, punt_personaggi);
			if(visto == killerevittima[1] && !(killerevittima[1]->vivo)){
				//if(find(cattivinoti.begin(),cattivinoti.end(),killerevittima[0])==cattivinoti.end())
					cattivinoti.push_back(killerevittima[0]);
					//cout << "voyeurato!\n";
				}
			return 0;
	};
};


class veggente: public personaggio {
	public:
	veggente() : personaggio(1,0,0,"veggente",VEGGENTE) {
		}
	
	int notte (p** punt_personaggi){ //cout << "Ciao!\n";
			if(gente.size() >= numerovivi(punt_personaggi))
				return 0;
			p* scrutato;
			do
				{scrutato = randtag(-1, -1, 1, -1, punt_personaggi);
				//cout << "Cerco fra " << numerovivi(punt_personaggi) << " personaggi\n";
				}
			while (find(gente.begin(), gente.end(),scrutato)!=gente.end());
			if(scrutato->fazione == 1){ 
				//cout << "cattivo veggato!\n";
				cattivinoti.push_back(scrutato);
			};
			gente.push_back(scrutato);
			/*list<p*>::iterator it;
			int dead=0;
			for(it=gente.begin();it!=gente.end();it++){
				dead+=morto(*it);
				cout << (*it)->ruolo << " " << (*it) << endl;
				
			}
			cout << "Il veggente ha scrutato " << dead << " morti!\n";
			cout << gente.size() << endl;*/
			return 0;
			};
};

class lupo: public personaggio {
	public:
	lupo() : personaggio(0,1,0,"lupo",LUPO) {};
	int notte (p** punt_personaggi) {
		return 0;
	}
};

void lupi (p** punt_personaggi) {
	if(randtag(-1,0,1,-1,punt_personaggi)==NULL && randtag(-1,2,1,-1,punt_personaggi)==NULL)
		return;
	p* killer = randtag(LUPO, -1, 1, -1, punt_personaggi);
	if(killer == NULL)
		return;
	p* vittima;
	killerevittima[0]= killer;
	if(notialdiavolo.size()!=0) {
			vittima=notialdiavolo[0];
			kill(notialdiavolo[0]);
			notialdiavolo.erase(notialdiavolo.begin());
			}
	else {
	do			
		vittima = randtag(-1, -1,1,-1,punt_personaggi);
	while(vittima -> fazione == 1);
	if((vittima->ruolo_num != NEGROMANTE) && (vittima->ruolo_num != IPNOTISTA || numerospettri==0))
	kill(vittima);
	};
	killerevittima[1]= vittima;
	
};

class diavolo: public personaggio {
	public:
	diavolo() : personaggio(0,1,1,"diavolo",DIAVOLO) {};
	int notte (p** punt_personaggi) {
			if(gente.size()>= numerovivi(punt_personaggi))
				return 0;
			p* scrutato;
			do
			scrutato = randtag(-1, -1, 1, -1, punt_personaggi);
			while (find(gente.begin(), gente.end(),scrutato)!=gente.end() && scrutato!=this);
			if(scrutato->buonofigo){
				notialdiavolo.push_back(scrutato);
			};
				gente.push_back(scrutato);
			return 0;
			};
};

class sequestratore: public personaggio {
	public:
	sequestratore() : personaggio(0,1,0,"sequestratore", SEQUESTRATORE) {};
	int notte (p** punt_personaggi){
		randtag(-1, -1, 1, -1, punt_personaggi)->sequestrato=1;
		if (killerevittima[0]->sequestrato)
			killerevittima[1]->vivo=1;
		return 0;
	};
};

class indemoniato: public personaggio {
	public:
	indemoniato() : personaggio(0,1,0,"indemoniato",INDEMONIATO) {};
	int notte (p** punt_personaggi){
		return 0;
	};
};

class medium: public personaggio {
	public:
	medium() : personaggio(0,2,1,"medium",MEDIUM) {};
	int notte (p** punt_personaggi){
		return 0;
	};
};
	
	
void notte_ipnotista(p** punt_personaggi, p* ipnotista, list<p*>* ipnotizzati){
		p* ipnotizzato_nuovo;
		//do
			ipnotizzato_nuovo=randtag(-1,-1,1,-1, punt_personaggi);
		//while(ipnotizzato_nuovo-> fazione ==2 || find((*ipnotizzati).begin(), (*ipnotizzati).end(), ipnotizzato_nuovo)!=(*ipnotizzati).end());
		(*ipnotizzati).push_back(ipnotizzato_nuovo);
};	

class ipnotista: public personaggio { /*WARNING: LA STRATEGIA DELL'IPNOTISTA E' SCRITTA QUI!*/
	public:
	ipnotista() : personaggio(0,2,0,"ipnotista",IPNOTISTA) {};
	int notte (p** punt_personaggi){
		if(gente.size() > 1 && rand()%2 == 0 && vivo)
			cattivinoti.push_back(this);
		if (appenaagito) {
				appenaagito = 0;
				return 0;
			};	
		if(numerovivi(punt_personaggi) < hypno)	
			notte_ipnotista(punt_personaggi, this, &gente);	
		appenaagito=1;
	return 0; 	
	};		
};		

class negromante: public personaggio {
	public:
	negromante() : personaggio(0,2,1,"negromante",NEGROMANTE) {};
	int notte (p** punt_personaggi) {
		return 0;
	};
};		

void notte_negromanti(p** punt_personaggi) {
if(numerospettri >= 4){
	//cout << "I negromanti non stanno agendo!\n";
	return;	
};
if(len - numerovivi(punt_personaggi) < 2){
	//cout << "I negromanti non stanno agendo!\n";
	return;
};
if(appenaspettrificato){
	//cout << "I negromanti non stanno agendo!\n";
	appenaspettrificato = 0;
	return;
	};
//cout << "I negromanti stanno agendo!\n";
//cout << "Ci sono " << numerocustoditi(punt_personaggi) << " custoditi\n";
p* futuro_spettro;
if(randtag(-1,0,0,-1,punt_personaggi)==NULL || randtag(-1,1,0,-1,punt_personaggi) == NULL){
	//cout << "I negromanti non stanno agendo senza nessun motivo!\n";
	return;
};
do
		futuro_spettro=randtag(-1, -1, 0, -1, punt_personaggi);
while(futuro_spettro->fazione == 2);
if(futuro_spettro->ruolo_num == LUPO || futuro_spettro->custodito)
			return;
//cout << "I negromanti stanno agendo davvero!\n";
numerospettri++;			
appenaspettrificato=1;
switch ( numerospettri) {
	case 1 :
		futuro_spettro->fazione = 2;
		//futuro_spettro -> ruolo_num = SPETTRO_OCCULTAMENTO;
		break;
	case 2 :
	case 3 :
		futuro_spettro->fazione=2;
		//futuro_spettro -> ruolo_num = SPETTRO;
		break;
	case 4 :
		futuro_spettro->fazione=2;
		//futuro_spettro->ruolo_num = SPETTRO_MORTE;
		break;
	default:
		return;
		break;
		};
return;
};

void spettro_morte (p** punt_personaggi){
		if(randtag(NEGROMANTE, -1, 1, -1, punt_personaggi)==NULL)
			return;
		if(randtag(-1, 0, 1, -1, punt_personaggi)==NULL && randtag(-1,1,1,-1,punt_personaggi)==NULL)
			return;
		if(appenaucciso){
			appenaucciso=0;
			return;
		};
		/*if(uccisodallospettro !=NULL)
			{uccisodallospettro=NULL;
			return;
			};*/
		p* vittima;	
		do			
			vittima = randtag(-1, -1,1,-1,punt_personaggi);
		while(vittima -> fazione == 2);	
		if(vittima->ruolo_num!=LUPO){
			kill(vittima);
			uccisodallospettro = vittima;	
		};
};



int vittoria(p** punt_personaggi){ //ritorna la fazione che ha vinto, -1 se non ha vinto nessuno
	if(randtag(NEGROMANTE, -1, 1, -1, punt_personaggi)== NULL)
		for(int i=0; i<len; i++)
			if(((*(punt_personaggi+i)))->fazione== 2)
				((*(punt_personaggi+i)))->vivo=0;
	if(randtag(LUPO, -1, 1, -1, punt_personaggi)==NULL)
		for(int i=0; i<len; i++)
			if(((*(punt_personaggi+i)))->fazione== 1)
				((*(punt_personaggi+i)))->vivo=0;
	if(randtag(NEGROMANTE, -1, 1, -1, punt_personaggi)== NULL && randtag(LUPO, -1, 1, -1, punt_personaggi)==NULL){
		//cout << "Hanno vinto i popolani!\n";
		vittoriebianchi++;
		return 0;
	};
	if(numeroneri(punt_personaggi)*2>numerovivi(punt_personaggi)) {
		//cout << "Hanno vinto i cattivi!\n";
		vittorieneri++;
		return 1;
	};
	{/* CONDIZIONI DI VITTORIA DEI NON MORTI! */
	int ipnotizzati = 0;
	for(int i=0;i<len;i++)
		if(((*(punt_personaggi+i)))->ruolo_num==IPNOTISTA && ((*(punt_personaggi+i)))->vivo)
			ipnotizzati += ((*(punt_personaggi+i)))->gente.size();
	if((numerogrigi(punt_personaggi)+ipnotizzati)*2>numerovivi(punt_personaggi)){
		//cout << "Hanno vinto i non-morti!\n";
		vittoriegrigi++;
		return 2;
	};
	};
	return -1;	
}

int nottevillagio(p** punt_personaggi){
	if(numerospettri>=4)
		spettro_morte(punt_personaggi);
	notte_negromanti(punt_personaggi);
	//cout << "numero spettri " << numerospettri << endl;
	lupi(punt_personaggi);
	for(int i=0; i<len; i++)
		if(((*(punt_personaggi+i)))->vivo && !(((*(punt_personaggi+i)))->sequestrato)) {
			(*(punt_personaggi+i))->notte(punt_personaggi);
		};
	reset(punt_personaggi);	
	return vittoria(punt_personaggi);		
};

int giornovillaggio(p** punt_personaggi){
	if(cattivinoti.size()!=0){
		//cout << "Ucciso un cattivo! \n";
		//cout << numeroneri(punt_personaggi) << endl;
		uccisionisensate ++;
		kill(cattivinoti[0]);
		//cout << numeroneri(punt_personaggi) << endl;
		cattivinoti.erase(cattivinoti.begin());
	}
	else if(misticinoti.size()!=0){
		int ran=rand()%5;
		if(ran==0)
			kill(randtag(-1,-1,1,-1,punt_personaggi));
		else {
			misticiuccisi++;
			kill(misticinoti[0]);
			misticinoti.erase(misticinoti.begin());
		}
	}
	else if(numerovivi(punt_personaggi)<numero_critico)
		kill(randtag(-1,-1,1,-1,punt_personaggi));
	reset(punt_personaggi);
	return vittoria(punt_personaggi);
};	
			

void listainiziale(personaggio** punt_personaggi)
{int cacciatori= 1, contadini= 9, contadini_mistici= 2, custodi= 1, esorcisti= 1, guardie= 1, maghi= 2, stalkers= 1, veggenti= 2, voyeurs= 1, lupi= 3, diavoli= 2, sequestratori= 1, indemoniati= 1, mediums= 0, ipnotisti= 1, negromanti= 2;
int i=0;
int j=0;
for (i=0;i<sequestratori;i++){
	punt_personaggi[j]=new sequestratore;
	j++;
};
for (i=0;i<guardie;i++){
	punt_personaggi[j]=new guardia;
	j++;
};
for (i=0;i<cacciatori;i++){
	punt_personaggi[j]=new cacciatore;
	j++;
};
for (i=0;i<contadini;i++){
	punt_personaggi[j]=new contadino(0);
	j++;
};
for (i=0;i<contadini_mistici;i++){
	punt_personaggi[j]=new contadino(1);
	j++;
};
for (i=0;i<custodi;i++){
	punt_personaggi[j]=new custode;
	j++;
};
for (i=0;i<esorcisti;i++){
	punt_personaggi[j]=new esorcista;
	j++;
};
for (i=0;i<maghi;i++){
	punt_personaggi[j]=new mago;
	j++;
};
for (i=0;i<stalkers;i++){
	punt_personaggi[j]=new stalker;
	j++;
};
for (i=0;i<veggenti;i++){
	punt_personaggi[j]=new veggente;
	j++;
};
for (i=0;i<voyeurs;i++){
	punt_personaggi[j]=new voyeur;
	j++;
};
for (i=0;i<lupi;i++){
	punt_personaggi[j]=new lupo;
	j++;
};
for (i=0;i<diavoli;i++){
	punt_personaggi[j]=new diavolo;
	j++;
};
for (i=0;i<indemoniati;i++){
	punt_personaggi[j]=new indemoniato;
	j++;
};
for (i=0;i<mediums;i++){
	punt_personaggi[j]=new medium;
	j++;
};
for (i=0;i<ipnotisti;i++){
	punt_personaggi[j]=new ipnotista;
	j++;
};
for (i=0;i<negromanti;i++){
	punt_personaggi[j]=new negromante;
	j++;
};
}



int main()
{
std::srand ( unsigned ( std::time(0) ) );
int i=0;
	for(i=0; i<10000; i++){
		resetsessione();
		p* punt_personaggi[len];
		listainiziale(punt_personaggi);
		while(true){	
			if(giornovillaggio(punt_personaggi)>=0)
			break;
			/*cout << "Ci sono " << numerobianchi(punt_personaggi) << " bianchi vivi\n";
			cout << "Ci sono " << numeroneri(punt_personaggi) << " neri vivi\n";
			cout << "Ci sono " << numerogrigi(punt_personaggi) << " grigi vivi\n";*/
			if(nottevillagio(punt_personaggi)>=0)
			break;
			/*cout << "Ci sono " << numerobianchi(punt_personaggi) << " bianchi vivi\n";
			cout << "Ci sono " << numeroneri(punt_personaggi) << " neri vivi\n";
			cout << "Ci sono " << numerogrigi(punt_personaggi) << " grigi vivi\n";
			cout << "Ci sono " << numerospettri << " spettri\n";*/
			turni++;
			//cout << i << endl;
		};
	};

cout << "La partita è durata in media " << (float) turni/i << " turni.\n";
cout << "I buoni hanno vinto " << vittoriebianchi << " volte.\n";
cout << "I cattivi hanno vinto " << vittorieneri << " volte.\n";
cout << "I non-morti hanno vinto " << vittoriegrigi << " volte.\n"; 
cout << "In media, sono stati uccisi " << (float) uccisionisensate/i << " cattivi noti.\n";
cout << "In media, sono stati uccisi " << (float) misticiuccisi/i << " mistici noti.\n";
return 0;	
}


