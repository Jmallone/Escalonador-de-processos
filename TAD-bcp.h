#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct bcp{ 
   int id;
   int tempo_cpu; 
   int prioridade; 
   int tempo_chegada;
   int filaIO[100];

   int tempo_inicio; 
   int tempo_executado; 
   int tempoIO;
   int tempo_fim;
   int tempo_espera;
   
   struct bcp* prox;
   struct bcp* ant;

}BCP;

/*typedef struct nosetor{
    unsigned long inicio;
    unsigned long fim;
    struct nosetor* prox;
    struct nosetor* ant;
}NoSetor;*/

BCP* newBCP();
void addBCP(BCP* unidade ,char* newline);
void addBCPLista(BCP* processo,BCP** lista);
void delBCP(BCP* p);

/*
void adicionar_NoSetor(NoSetor* Setores, unsigned long inicio, unsigned long fim);
void editar_NoSetor(NoSetor* Setores, unsigned long inicio, unsigned long fim);
void apagar_NoSetor(NoSetor* Setores);
NoSetor* setorTolivre(NoSetor* au2, NoSetor* au); //Deus me Perdoe */

/* Sentinela */
BCP* newBCP(){ 

	BCP* bcp = (BCP*)malloc(sizeof(BCP));
  bcp->id = -10;
	bcp->prox = bcp;
	bcp->ant = bcp;
	return bcp;	
}


/* Pega informações do TXT */
void addBCP(BCP* unidade ,char* newline){

    /* Insere em um vetor as informações da linha */
    int tmp[100];
    int i = 0;
    char delim[] = " ";
    char *ptr = strtok(newline, delim);
     while(ptr != NULL){
        tmp[i] = atoi (ptr);
        //printf("'%s'\n", ptr);
        ptr = strtok(NULL, delim);
        i++;
    }
    
    i--;
  BCP* novo = (BCP*)malloc(sizeof(BCP));
/*Começo do Parsing do new Line*/
  novo->id = tmp[0];
  novo->tempo_cpu = tmp[1];
  novo->prioridade = tmp[2];
  novo->tempo_chegada = tmp[3];

  for (int j=0 ; j< 100;j++) novo->filaIO[j] = -10;

/* Se existir Lista de Eventos de I/O */
  if (i > 3){
    for(int j = 0; j<(i-3);j++){
        novo->filaIO[j] = tmp[(i-2)+j];
    }
  }
/*Fim do Parsing*/

   novo->tempo_inicio = 0;
   novo->tempo_executado = 0;
   novo->tempoIO = 0;
   novo->tempo_fim = 0;
   novo->tempo_espera = 0;


/* Inserção da Lista Duplamente encadeada */
  novo->ant = unidade->prox->ant;
  novo->prox = unidade->prox;
  unidade->prox->ant = novo;
  unidade->prox = novo;
}

/*Adiciona um processo em uma Lista*/
void addBCPLista(BCP* processo, BCP** lista){

  if((*lista) == NULL) *lista = newBCP();

  processo->prox = (*lista)->ant->prox;
  processo->ant = (*lista)->ant;
  (*lista)->ant->prox = processo;
  (*lista)->ant = processo;

}

/*Deleta o Processo daquela Lista*/
void delBCP(BCP* p){

    p->ant->prox = p->prox;
    p->prox->ant = p->ant;

}

/*
void adicionar_NoSetor(NoSetor* Setores, unsigned long inicio, unsigned long fim){
	
  NoSetor *Novo1 = (NoSetor*)malloc(sizeof(NoSetor));
  Novo1->inicio = inicio ;
  Novo1->fim = fim ;

  
  Novo1->ant = Setores->prox->ant;
  Novo1->prox = Setores->prox;
  Setores->prox->ant = Novo1;
  Setores->prox = Novo1;
  
	
}

void editar_NoSetor(NoSetor* Setores, unsigned long inicio, unsigned long fim){

  Setores->prox->inicio = inicio ;
  Setores->prox->fim = fim ;

}

void apagar_NoSetor(NoSetor* Setores){
  NoSetor *Aux = Setores->prox;
  Setores->prox->prox->ant = Setores->prox->ant; 
  Setores->prox = Setores->prox->prox;
  free(Aux);
}

/* Usado Pelo Programador para DEBUG -NO OFFICIAL-*/

/*
void ver_NoSetor(NoSetor* Setores){
  NoSetor *Aux = Setores->prox;

  while( !(Aux == Setores) ){
    printf(" [ %d, %d ],", Aux->inicio, Aux->fim);
    Aux = Aux->prox;
  }
 
}

NoSetor* setorTolivre(NoSetor* au2, NoSetor* au){
  NoSetor* aux2 = au2->prox;
  NoSetor* aux = au;
  NoSetor* auxReturn = aux->ant;

  while( aux2->inicio < aux->inicio ){

    aux2 = aux2->prox;

  }

  aux->prox = aux2;
  aux->ant = aux2->ant;
  aux2->ant->prox = aux;
  aux2->ant = aux;

  if(aux->ant->fim == aux->inicio && !(aux->ant == au2) ){
    
    aux->inicio = aux->ant->inicio;
    apagar_NoSetor(aux->ant->ant);
  }

  if(aux->prox->inicio == aux->fim && !(aux->prox == au2)){ //Verifica se nao esta Olhando o Sentinela
    aux->fim = aux->prox->fim;
    apagar_NoSetor(aux);

  }
  return auxReturn;
}

*/