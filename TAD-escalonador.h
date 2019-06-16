#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct escalonador { 
   int tempo;
}ESCALONADOR; 


void sortList(BCP* unidade); // Ordena uma Lista
int verificaFilaProcessos(GP* unidade, ESCALONADOR* escal); // Verfica se existem processos para entrar na Lista de Prontos
int verificaFilaBloqueado(GP* unidade, ESCALONADOR* escal); // Verifica se existe Processos para serem atualizados (RR)

int escalonar(ESCALONADOR* escal, BCP** origem, BCP** destino); // Passa um Processo de uma Lista para a Outra
void FIFO(GP* unidade); // Algoritmo de Escalonamento
void RR(GP* unidade); // Algoritmo de Escalonamento


/*Ordena Lista por tempo de Chegada*/
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

int verificaFilaBloqueado(GP* unidade, ESCALONADOR* escal){

    /*Se existir processos e o tempo de CPU for >= ao menor tempo na fila então escalona*/
     if((unidade->fila_bloqueado != NULL)){   
        BCP* atual = unidade->fila_bloqueado->prox;

        /* Percorro toda a lista de Bloqueados e vou atualizando o Tempo de I/O */
        while(atual->id != -10){
            atual->tempoIO = atual->tempoIO - 1;

            /* Se achou Tempo de I/O acabou ele volta para o Fim da Fila de Prontos */

            /*???? Sempre Vai estar em ordem qual processo vai acabar primeiro ? 
            
            Vou considerar por enquanto que sim*/
            if(atual->tempoIO <= 0){

                escalonar(escal,&unidade->fila_bloqueado,&unidade->fila_pronto);
            }


        }

        return 1;
    }
    return 0;
}

int verificaFilaProcessos(GP* unidade, ESCALONADOR* escal){

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

        /*Condição para evitar colocar um Processo que o 
        tempo de chegada é 5 e o tempo no Clock é 2
        */
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

            /* Adiciona o Processo a Lista destino Mas como se fosse Fila */
            addBCPLista(tmp,destino);
        
            return 1;
        }

    }

    return 0;
}

/* Verifica se o Processo Atual vai fazer I/0 */
int solicitaIO(BCP* processo, ESCALONADOR* escal){
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
    printf("\n\n+--- STEP-BY-STEP ---- \n");
    
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

            /*Enquanto o processo ainda tiver tempo de CPU continue*/
            while( (processo->tempo_cpu) - (processo->tempo_executado) > 0){
                printf("|Processo [%d] - Tempo Exec [%d]\n", processo->id,processo->tempo_executado);
                processo->tempo_executado++; //Processo Executou 1Clock
                escalonador->tempo++;

                /* Chamo a função para verificar se entrou algum processo no tempo Atual*/
                verificaFilaProcessos(unidade,escalonador);

                /* Verifica se o Processo Atual vai fazer I/0 */
                if(solicitaIO(processo,escalonador)){
                    printf("|Fez I/O no Tempo [%d]\n", processo->tempo_executado);
                    /* Quando Se faz I/0 demora 3 Clock */
                    escalonador->tempo = escalonador->tempo + 3;
                }
            }
                printf("+-----------------+\n\n");
                
                /*Depois que o Processo Termina ele vai para a Lista de Finalizados */
                escalonar(escalonador,&unidade->fila_pronto,&unidade->fila_finalizados); 
                unidade->fila_finalizados->ant->tempo_fim = escalonador->tempo;
                
                /*Ajuste para o Tempo Total Executado Caso o Processo fez I/O*/
                unidade->fila_finalizados->ant->tempo_executado =( unidade->fila_finalizados->ant->tempo_fim - unidade->fila_finalizados->ant->tempo_inicio);

        }else{
            /*Clock de Ociosidade*/
            escalonador->tempo++;
        }  
   } 
}

void RR(GP* unidade){
    int contador_debug = 0;
    /*Organiza por tempo de chegada*/
    sortList(unidade->fila_processos);

    printf("\n\n\n ------ FILAS ANTES ---- \n");
    verificar(unidade);
    printf("\n\n");
    
    printf("\n\n+--- STEP-BY-STEP ---- \n");
    /* Cria o escalonador para controlar o tempo */
    ESCALONADOR *escalonador = (ESCALONADOR*)malloc(sizeof(ESCALONADOR));
    escalonador->tempo = 0;



    /*Se ainda Existir Processos na Fila de Pronto e fila de Procesos */
    while( (unidade->fila_pronto != NULL) || (unidade->fila_processos != NULL) ){
        
        verificaFilaBloqueado(unidade,escalonador);
        verificaFilaProcessos(unidade,escalonador);
        
        
        /* Se existir alguem na Fila de Pronto Executar: Se não fica ocioso*/
        if(unidade->fila_pronto != NULL ){
            int quantun = 2;
            BCP* processo = unidade->fila_pronto->prox;
            processo->tempo_inicio = escalonador->tempo;

            /*Enquanto o processo ainda tiver tempo de CPU continue*/
            while( (processo->tempo_cpu) - (processo->tempo_executado) > 0 && quantun != 0){
                printf("|Processo [%d] - Tempo Exec [%d]\n", processo->id,processo->tempo_executado);
                processo->tempo_executado++; //Processo Executou 1Clock
                escalonador->tempo++;
                quantun--;
                printf("| Q [%d] ---\n",quantun);
                printf("| CLOCK TIME [%d] ---\n", escalonador->tempo);


                verificaFilaBloqueado(unidade,escalonador);
                /* Chamo a função para verificar se entrou algum processo no tempo Atual*/
                verificaFilaProcessos(unidade,escalonador);
                
                /* Verifica se o Processo Atual vai fazer I/0 */
                if(solicitaIO(processo,escalonador)){
                    printf("|Fez I/O no Tempo [%d]\n", processo->tempo_executado);
                    /* Quando Se faz I/0 demora 3 Clock 
                    processo->tempoIO = 3;
                    escalonar(escalonador,&unidade->fila_pronto,&unidade->fila_bloqueado);
                    quantun = 0;*/
                }
              
                
            }
                printf("+-----------------+\n\n");

            /*Se processo Terminou : Não existe tempo para executar*/
            if((processo->tempo_cpu) - (processo->tempo_executado) <= 0){
                /*Depois que o Processo Termina ele vai para a Lista de Finalizados */
                escalonar(escalonador,&unidade->fila_pronto,&unidade->fila_finalizados);
                
                unidade->fila_finalizados->ant->tempo_fim = escalonador->tempo;
                //quantun = 0;
            }

           

            /* Se o quantun acabou mas ainda existe Clock a ser executado no processo */
            if(quantun == 0 && (processo->tempo_cpu) - (processo->tempo_executado) > 0){

               /* Desvincula Processo Atual */
                delBCP(processo);

                /* Adiciona Processo Atual ao final da Lista(FILA) de Pronto */
                addBCPLista(processo,&unidade->fila_pronto);
                
            }

                printf("\n\n\n ------DEBUG DAS FILAS ---- \n");
                verificar(unidade);
                printf("\n\n");

        }else{
            /*Clock de Ociosidade*/
            escalonador->tempo++;
        }

        if(contador_debug > 3) exit(0);
        contador_debug++;  
   } 
}
