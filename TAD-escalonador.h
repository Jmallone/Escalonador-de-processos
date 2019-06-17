#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct escalonador { 
   int tempo;
}ESCALONADOR; 

void sortList(BCP* unidade); // Ordena a Lista de Processos
void sortList_Tempo_CPU(BCP* unidade); //Ordena a Lista de Prontos pelo menor tempo a ser executado
int verificaFilaProcessos(GP* unidade, ESCALONADOR* escal); // Verfica se existem processos para entrar na Lista de Prontos
int verificaFilaBloqueado(GP* unidade, ESCALONADOR* escal); // Verifica se existe Processos para serem atualizados (RR)
int escalonar(ESCALONADOR* escal, BCP** origem, BCP** destino); // Passa um Processo de uma Lista para a Outra
int solicitaIO(BCP* processo, ESCALONADOR* escal); // Verifica se o Processo Atual vai fazer I/0
void FIFO(GP* unidade); // Algoritmo de Escalonamento
void RR(GP* unidade); // Algoritmo de Escalonamento
void SRTF(GP* unidade); // Algoritmo de Escalonamento


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

/*Ordena a Lista de Prontos pelo menor tempo a ser executado*/
void sortList_Tempo_CPU(BCP* unidade){ 
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
                if(atual->tempo_cpu - atual->tempo_executado > index->tempo_cpu - index->tempo_executado) {  
                    
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

    /*Se existir processos*/
     if((unidade->fila_bloqueado != NULL)){
         
        /*Pega o primeiro elemento da lista depois do Sentinela*/ 
        BCP* atual = unidade->fila_bloqueado->prox;

        /* Percorre toda a lista de Bloqueados e vai atualizando o Tempo de I/O */
        while(atual->id != -10){ // O ID do sentinela é (-10)
            atual->tempoIO = atual->tempoIO - 1;

            /* Se achou Tempo de I/O acabou ele volta para o Fim da Fila de Prontos */

            /*???? Sempre Vai estar em ordem qual processo vai acabar primeiro ? 
            
            Vou considerar por enquanto que sim*/
            if(atual->tempoIO <= 0){
                atual->tempoIO = 0;
                printf("\033[1;34m");
                printf("|    !Processo ID [%d] - CPU CLOCK[%d]\n",atual->id,escal->tempo);
                printf("|    !Saio de I/O e foi para FINAL de Pronto\n");
                printf("\033[0m");
                escalonar(escal,&unidade->fila_bloqueado,&unidade->fila_pronto);
            }

            atual = atual->prox;
        }

        return 1;
    }
    return 0;
}

int verificaFilaProcessos(GP* unidade, ESCALONADOR* escal){

    /*Se existir processos e o tempo de CPU for >= tempo na fila então escalona*/
     if((unidade->fila_processos != NULL) && (escal->tempo >= unidade->fila_processos->prox->tempo_chegada ) ){
        printf("\033[1;34m");   
        printf("|    !Processo ID [%d] entrou em Pronto - TIME CPU [%d]\n",unidade->fila_processos->prox->id,escal->tempo);
        printf("\033[0m");
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

            /*Desvincula o Processo daquela Lista*/
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

            /* Adiciona o Processo a Lista destino Mas Trato Lista como se fosse Fila */
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

        /* Se Existe um tempo de I/O Correspondendo ao Tempo Executado do processo*/
        if(processo->filaIO[i] == processo->tempo_executado){

            /*Deslocamento do Vetor para Excluir aquele tempo de I/0 */
            while(processo->filaIO[i] != -10){
                processo->filaIO[i] = processo->filaIO[i+1];
                i++;
            }
            printf("\033[1;33m");
            printf("! -Fez I/O - Tempo Exec [%d]\n", processo->tempo_executado);
            printf("\033[0m");
            printf("+-----------------+\n\n");
            return 1;
        }
        i++;
    }
    return 0;
}

/*

    Algoritmos de Escalonamento Abaixo:

*/

void FIFO(GP* unidade){
    printf("\n\n+--- STEP-BY-STEP ---- \n");
    
    /* Cria o escalonador para controlar o tempo */
    ESCALONADOR *escalonador = (ESCALONADOR*)malloc(sizeof(ESCALONADOR));
    escalonador->tempo = 0;

    /*Organiza os processos por tempo de chegada*/
    sortList(unidade->fila_processos);

    /*Se ainda Existir Processos na Fila de Pronto e na Fila de Processos */
    while( (unidade->fila_pronto != NULL) || (unidade->fila_processos != NULL) ){
        
        verificaFilaProcessos(unidade,escalonador);
        
        /* Se existir alguem na Fila de Pronto Executar: Se não fica ocioso*/
        if(unidade->fila_pronto != NULL ){

            /*  Atribui o processo em questão para uma Variavel e 
                atribui em qual tempo o processo começou na CPU */
            BCP* processo = unidade->fila_pronto->prox;
            processo->tempo_inicio = escalonador->tempo;

            /*Enquanto o processo ainda tiver tempo de CPU continue*/
            while( (processo->tempo_cpu) - (processo->tempo_executado) > 0){
                /* Executa 1Clock */
                processo->tempo_executado++; 
                escalonador->tempo++;

                printf("|Processo [%d] - Tempo Exec [%d/%d]\n", processo->id,processo->tempo_executado,processo->tempo_cpu);
                printf("| -TIME CPU [%d] ---\n", escalonador->tempo);

                /* Verifica se o Processo Atual vai fazer I/0 */
                if(solicitaIO(processo,escalonador)){
                    /* Quando Se faz I/0 demora 3 Clock */
                    escalonador->tempo = escalonador->tempo + 3;
                }
                
                /* Chamo a função para verificar se entrou algum processo no tempo Atual*/
                verificaFilaProcessos(unidade,escalonador);

            } /*O Processo Ficará nesse While até seu tempo de CPU for completado*/


                printf("+-----------------+\n\n");
                
                /*Depois que o Processo Termina ele vai para a Lista de Finalizados */
                escalonar(escalonador,&unidade->fila_pronto,&unidade->fila_finalizados); 
                unidade->fila_finalizados->ant->tempo_fim = escalonador->tempo;
                
                /*Ajuste para o Tempo Total Executado Caso o Processo faça I/O*/
                unidade->fila_finalizados->ant->tempo_executado =( unidade->fila_finalizados->ant->tempo_fim - unidade->fila_finalizados->ant->tempo_inicio);

        }else{
            /*Clock de Ociosidade*/
            escalonador->tempo++;
        }  
   }
    printf("\n\n ------------------- ESCOLONAMENTO FINALIZADO ------------------- \n\n"); 
    verificar(unidade);
    printf("\n\n"); 
}

void RR(GP* unidade){

    /*Organiza por tempo de chegada*/
    sortList(unidade->fila_processos);

    printf("\n\n\n ------ FILAS ANTES ---- \n");
    verificar(unidade);
    printf("\n\n");
    
    printf("\n\n+--- STEP-BY-STEP ---- \n");
    /* Cria o escalonador para controlar o tempo */
    ESCALONADOR *escalonador = (ESCALONADOR*)malloc(sizeof(ESCALONADOR));
    escalonador->tempo = 0;

    /*Se ainda Existir Processos na Fila de Pronto, Fila de Processos e Fila de Bloqueados */
    while( (unidade->fila_pronto != NULL) || (unidade->fila_processos != NULL) || (unidade->fila_bloqueado != NULL) ){
        
        verificaFilaBloqueado(unidade,escalonador);
        verificaFilaProcessos(unidade,escalonador);
        
        /* Se existir alguem na Fila de Pronto Executar: Se não fica ocioso*/
        if(unidade->fila_pronto != NULL ){

            int quantun = 2;
            BCP* processo = unidade->fila_pronto->prox;
            processo->tempo_inicio = escalonador->tempo;

            /*Enquanto o processo ainda tiver tempo de CPU e o quantun for valido continue*/
            while( (processo->tempo_cpu) - (processo->tempo_executado) > 0 && quantun != 0){
                processo->tempo_executado++; //Processo Executou 1Clock
                escalonador->tempo++;
                quantun--;
                printf("|Processo ID:[%d] - Tempo Exec [%d/%d]\n", processo->id,processo->tempo_executado, processo->tempo_cpu);
                printf("| -Quantun: [%d] ---\n",quantun);
                printf("| -TIME CPU [%d] ---\n", escalonador->tempo);

                verificaFilaBloqueado(unidade,escalonador);
                /* Chamo a função para verificar se entrou algum processo no tempo Atual*/
                verificaFilaProcessos(unidade,escalonador);
                
                /* Verifica se o Processo Atual vai fazer I/0 */
                if(solicitaIO(processo,escalonador)){
                    /* Quando Se faz I/0 demora 3 Clock */

                    /* Contador Interno do processo para saber quantos clock ficara em bloqueado*/
                    processo->tempoIO = 3;
                    escalonar(escalonador,&unidade->fila_pronto,&unidade->fila_bloqueado);
                    
                    verificar(unidade);
                    printf("\n\n");

                    quantun = 0;
                }
                
                printf("+-----------------+\n");
            }
            

            /*Se processo Terminou : Não existe tempo para executar*/
            if((processo->tempo_cpu) - (processo->tempo_executado) <= 0){
                /*Depois que o Processo Termina ele vai para a Lista de Finalizados */
                escalonar(escalonador,&unidade->fila_pronto,&unidade->fila_finalizados);
                unidade->fila_finalizados->ant->tempo_fim = escalonador->tempo;
            }

            /* Se o quantun acabou mas ainda existe Clock a ser executado no processo 
                E o processo não entrou em I/o        
            */
            if(quantun == 0 && (processo->tempo_cpu) - (processo->tempo_executado) > 0){
                
                if ((processo->tempoIO)<=0){
                    printf("\033[1;34m");   
                    printf("|    !Processo ID [%d] voltou para o FINAL de Pronto\n",processo->id);
                    printf("\033[0m");
                    /* Desvincula Processo Atual */
                    delBCP(processo);
                    /* Adiciona Processo Atual ao final da Lista(FILA) de Pronto */
                    addBCPLista(processo,&unidade->fila_pronto);
                }
            }

        }else{
            /*Clock de Ociosidade*/
            escalonador->tempo++;
            printf("\n| -- CPU OCIOSA!"); 
        }
        
   }

    printf("\n\n ------------------- ESCOLONAMENTO FINALIZADO ------------------- \n\n"); 
    verificar(unidade);
    printf("\n\n"); 
}

void SRTF(GP* unidade){

    printf("\n\n+--- STEP-BY-STEP ---- \n");
    
    /* Cria o escalonador para controlar o tempo */
    ESCALONADOR *escalonador = (ESCALONADOR*)malloc(sizeof(ESCALONADOR));
    escalonador->tempo = 0;

    /*Organiza os processos por tempo de chegada*/
    sortList(unidade->fila_processos);

    /*Se ainda Existir Processos na Fila de Pronto e na Fila de Processos */
    while( (unidade->fila_pronto != NULL) || (unidade->fila_processos != NULL) ){
        
        verificaFilaProcessos(unidade,escalonador);
        /*Organiza os processos pelo menor tempo na CPU */
        sortList_Tempo_CPU(unidade->fila_pronto);
        
        /* Se existir alguem na Fila de Pronto Executar: Se não fica ocioso*/
        if(unidade->fila_pronto != NULL ){

            /*  Atribui o processo em questão para uma Variavel e 
                atribui em qual tempo o processo começou na CPU */
            BCP* processo = unidade->fila_pronto->prox;
            processo->tempo_inicio = escalonador->tempo;

            int time = 1;
            /*Enquanto o processo ainda tiver tempo de CPU continue*/
            while( (processo->tempo_cpu) - (processo->tempo_executado) > 0 && time != 0){
                /* Executa 1Clock */
                processo->tempo_executado++; 
                escalonador->tempo++;

                printf("|Processo [%d] - Tempo Exec [%d/%d]\n", processo->id,processo->tempo_executado,processo->tempo_cpu);
                printf("| -TIME CPU [%d] ---\n", escalonador->tempo);

                /* Verifica se o Processo Atual vai fazer I/0 */
                if(solicitaIO(processo,escalonador)){
                    /* Quando Se faz I/0 demora 3 Clock */
//                    escalonador->tempo = escalonador->tempo + 3;
                }
                
                /* Chamo a função para verificar se entrou algum processo no tempo Atual*/
                verificaFilaProcessos(unidade,escalonador);
                /*Organiza os processos pelo menor tempo na CPU */
                sortList_Tempo_CPU(unidade->fila_pronto);

                /* Verifica se o tempo restante do processo que esta no Começo da fila é Menor que o processo atual */
                if(unidade->fila_pronto->prox->tempo_cpu - unidade->fila_pronto->prox->tempo_executado < processo->tempo_cpu - processo->tempo_executado ){

                        time = 0;

                        printf("\033[1;32m");
                        printf("|    !Processo ID [%d] - TEMPO CPU Restante[%d]\n",unidade->fila_pronto->prox->id,unidade->fila_pronto->prox->tempo_cpu - unidade->fila_pronto->prox->tempo_executado);
                        printf("|    !È Maior que o Processo Atual\n");
                        printf("|    !Processo ID [%d] - TEMPO CPU Restante[%d]\n",processo->id,processo->tempo_cpu - processo->tempo_executado);
                        printf("|    !Processo Atual Irá ser reorganizado na Fila de Prontos\n");
                        printf("\033[0m");
                
                         /*Desvincula o Processo daquela Lista*/
                        delBCP(processo);

                        /* Adiciona o Processo atual na Lista pronto */
                        addBCPLista(processo,&unidade->fila_pronto);
                        /* Quando o While Rodar Ele pegara o Processo com o menor tempo de CPU*/
                        

                }


            } /*O Processo Ficará nesse While até seu tempo de CPU for completado*/


                printf("+-----------------+\n\n");
                    if ((processo->tempo_cpu) - (processo->tempo_executado) <= 0){
                    /*Depois que o Processo Termina ele vai para a Lista de Finalizados */
                    escalonar(escalonador,&unidade->fila_pronto,&unidade->fila_finalizados); 
                    unidade->fila_finalizados->ant->tempo_fim = escalonador->tempo;
                    
                    /*Ajuste para o Tempo Total Executado Caso o Processo faça I/O*/
                    unidade->fila_finalizados->ant->tempo_executado =( unidade->fila_finalizados->ant->tempo_fim - unidade->fila_finalizados->ant->tempo_inicio);
                }

        }else{
            /*Clock de Ociosidade*/
            escalonador->tempo++;
        }  
   }
    printf("\n\n ------------------- ESCOLONAMENTO FINALIZADO ------------------- \n\n"); 
    verificar(unidade);
    printf("\n\n"); 


}
