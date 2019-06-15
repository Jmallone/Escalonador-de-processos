#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct escalonador { 
   int tempo;
}ESCALONADOR; 

void FIFO(GP* unidade);
void sortList(BCP* unidade);
int verificaFilaProcessos(GP* unidade, ESCALONADOR* escal);
int escalonar(ESCALONADOR* escal, BCP** origem, BCP** destino);

/*Ordena por tempo de Chegada*/
void sortList(BCP* unidade){ 
    BCP* atual = unidade;
    BCP* index = NULL; 
    BCP* temp = NULL;
    BCP* var = NULL;
    
    if(unidade == NULL) {
      printf("\n Unidade Vazia!");
        return;  
    }else{ 
 
        for(atual = unidade->prox; atual->id != -10; atual = atual->prox) {  
            for(index = atual->prox; index->id != -10; index = index->prox) {  
                if(atual->tempo_chegada > index->tempo_chegada) {  
                    
                    temp = atual;  
                    atual = index;                 
                    temp->ant->prox = atual;
                    atual->ant->prox = temp;
                    var = temp->ant;
                    temp->ant = atual->ant;
                    atual->ant = var;
                    var =  atual->prox;
                    atual->prox = temp->prox;
                    temp->prox = var;
                    atual->prox->ant = atual;
                    var->ant = temp;
                    
                }
                
            } 
        }   
    }
} 

int verificaFilaProcessos(GP* unidade, ESCALONADOR* escal){

    //printf("Tempo CPU [%d] : P[%d]-Tempo de Chegada[%d]\n\n",escal->tempo,unidade->fila_processos->prox->id,unidade->fila_processos->prox->tempo_chegada);
    /*Se existir processos e o tempo de CPU for >= ao menor tempo na fila então escalona*/
     if((unidade->fila_processos != NULL) && (escal->tempo >= unidade->fila_processos->prox->tempo_chegada ) ){   
        escalonar(escal,&unidade->fila_processos,&unidade->fila_pronto);
        return 1;
    }
    return 0;
}

int escalonar(ESCALONADOR* escal, BCP** origem, BCP** destino){

    /*Coloca o processo em alguma lista destino */
    if((*origem) != NULL){
        if(escal->tempo >= (*origem)->prox->tempo_chegada){
            BCP* tmp = (*origem)->prox;
            delBCP((*origem)->prox);

            /*
                Se Origem->prox->id == -10 && origem->ant->id == -10:
                    Origem = NULL;

                    Representação:
                    
                            origem
                    +--->  +-----+  <---+
        origem->ant |      + -10 +      | origem->prox
                    +----- +-----+ -----+

            */

           /* Se encontrar a lista só com o Sentinela, deixa ela NULA como na representação acima*/
            if(( (*origem)->prox->id == -10) && ((*origem)->ant->id == -10)){
               free((*origem)->prox);
               (*origem) = NULL;
            
            }

            /* Adiciona o Processo a Lista destino */
            addBCPLista(tmp,destino);
        
            return 1;
        }

    }

    return 0;
}

int solicitaIO(BCP* processo, ESCALONADOR* escal){
    printf("Verifica I/O : Tempo[%d] ", processo->tempo_executado);
    int i = 0;
    while(processo->filaIO[i] != -10){
        if(processo->filaIO[i] == processo->tempo_executado){

            /*Deslocamento do Vetor para Excluir aquele tempo de I/0 */
            while(processo->filaIO[i] != -10){
                processo->filaIO[i] = processo->filaIO[i+1];
                i++;
            }
            return 1;
        }
        i++;
    }
    return 0;
}


void FIFO(GP* unidade){
    printf("\n Entrei no FIFO \n");
    /* Cria o escalonador para controlar o tempo */
    ESCALONADOR *escalonador = (ESCALONADOR*)malloc(sizeof(ESCALONADOR));
    escalonador->tempo = 0;
    /*Organiza por tempo de chegada*/
    sortList(unidade->fila_processos);
    /*Se ainda Existir Processos na Fila de Pronto e fila de Procesos */
    while( (unidade->fila_pronto != NULL) || (unidade->fila_processos != NULL) ){
        
        verificaFilaProcessos(unidade,escalonador);
        
        /* Se existir alguem na Fila de Pronto Executar: Se não fica ocioso*/
        if(unidade->fila_pronto != NULL ){
            BCP* processo = unidade->fila_pronto->prox;
            processo->tempo_inicio = escalonador->tempo;
        
            while( (processo->tempo_cpu) - (processo->tempo_executado) > 0){
                printf("\nProcesso [%d] - Executado [%d]\n", processo->id,processo->tempo_executado);
                processo->tempo_executado++; //Processo Executou 1Clock
                escalonador->tempo++;
                verificaFilaProcessos(unidade,escalonador);

                /* Quando Se faz I/0 demora 3 Clock */
                if(solicitaIO(processo,escalonador)){
                    //processo->tempo_executado = processo->tempo_executado + 3;
                    escalonador->tempo = escalonador->tempo + 3;
                    
                }
            }
                escalonar(escalonador,&unidade->fila_pronto,&unidade->fila_finalizados); 
                unidade->fila_finalizados->ant->tempo_fim = escalonador->tempo;
                unidade->fila_finalizados->ant->tempo_executado =( unidade->fila_finalizados->ant->tempo_fim - unidade->fila_finalizados->ant->tempo_inicio);
                escalonador->tempo++;

        }else{
            escalonador->tempo++;
        }  
   } 
}



//void FIFO(GP* unidade){
//    printf("\n Entrei no FIFO \n");
//    /* Cria o escalonador para controlar o tempo */
//    ESCALONADOR *escalonador = (ESCALONADOR*)malloc(sizeof(ESCALONADOR));
//    escalonador->tempo = 0;
//
//    /*Organiza por tempo de chegada*/
//    sortList(unidade->fila_processos);
//
//    /*Se ainda Existir Processos na Fila de Pronto e fila de Procesos */
//    while( (unidade->fila_pronto != NULL) || (unidade->fila_processos != NULL) ){
//        
//        verificaFilaProcessos(unidade,escalonador);
//        
//        /* Se existir alguem na Fila de Pronto Executar: Se não fica ocioso*/
//        if(unidade->fila_pronto != NULL ){
//            BCP* processo = unidade->fila_pronto->prox;
//            processo->tempo_inicio = escalonador->tempo;   
// //           while(1){
//
//                verificaFilaProcessos(unidade,escalonador);
//                if( (processo->tempo_cpu) - (processo->tempo_executado) > 0){
//                    
//                    processo->tempo_executado++; //Processo Executou 1Clock
//                    escalonador->tempo++; //Clock +1
// //                    if( (processo->tempo_cpu) - (processo->tempo_executado) <= 0){
//                        
//                         
////                        escalonar(escalonador,&unidade->fila_pronto,&unidade->fila_finalizados);
////                        unidade->fila_finalizados->ant->tempo_fim = escalonador->tempo;
//  
// //                       break;
//                     }
//
////                     if(solicitaIO(processo,escalonador)){
////                        escalonador->tempo = escalonador->tempo + 3;
////                        break;
////                     }
//
//                }else{
//                    escalonar(escalonador,&unidade->fila_pronto,&unidade->fila_finalizados); 
//                    unidade->fila_finalizados->ant->tempo_fim = escalonador->tempo;
//                }
// //while_end           }
//        }
//        
//       escalonador->tempo++;
//   }
//    
//}
//