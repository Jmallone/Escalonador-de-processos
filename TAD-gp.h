#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"TAD-bcp.h"

typedef struct gerenciador { 
   BCP* fila_processos;
   BCP* fila_pronto;
   BCP* fila_bloqueado;
   BCP* fila_finalizados; 
}GP;

GP* newGP(char* nomeArquivo); //Cria o Gerenciador de Processos
void verificar(GP* gerenciador);

GP* newGP(char* nomeArquivo){

  GP *gerenciador = (GP*)malloc(sizeof(GP));
  gerenciador->fila_processos = newBCP();//Ja cria um Sentinela
  gerenciador->fila_pronto = NULL;
  gerenciador->fila_bloqueado = NULL;
  gerenciador->fila_finalizados = NULL;
  
   FILE *file = fopen ( nomeArquivo, "r" );
   if ( file != NULL )
   {
      char line [ 128 ]; /* or other suitable maximum line size */
      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      {
            /* Pega cada Linha NewLine */
            addBCP(gerenciador->fila_processos,line);
            /*Fim da line */
      }
      fclose (file);
   }
   
  return gerenciador;
}

/*Verificar Todos os Processos Criados*/
void verificar(GP* gerenciador){
   BCP* novo = NULL;
   printf("\033[1;31m");

   /* Começo da Fila de Processos */
   if (gerenciador->fila_processos != NULL){  
      novo = gerenciador->fila_processos->prox;

      printf("Fila de Processos: \n");
      while(novo->id != -10 && novo != NULL ){
         printf("+---------------------------");
         printf("\n|Id do processo: %d",novo->id);
         printf("\n|Tempo de CPU: %d",novo->tempo_cpu);
         printf("\n|Prioridade : %d",novo->prioridade);
         printf("\n|Tempo e Chegada: %d",novo->tempo_chegada);
         if (novo->filaIO[0] != -10) printf("\n|Tempo do I/O: ");
         
         int j = 0;
         while(novo->filaIO[j] != -10){
            printf(" %d ", novo->filaIO[j]);
            j++;
         }
         printf("\n+---------------------------\n\n");
         novo = novo->prox;
      }
   /* Fim da Fila de Processos */ 
   }else{ printf("-----Fila de Processos VAZIA -\n"); }


   /* Começo da Fila de Processos PRONTOS */ 
   if (gerenciador->fila_pronto != NULL){ 
      novo = gerenciador->fila_pronto->prox;
      printf("\033[1;35m");
      printf("Fila de Processos PRONTOS: \n");
      while(novo->id != -10 && novo != NULL ){
         printf("+---------------------------");
         printf("\n|Id do processo: %d",novo->id);
         printf("\n|Tempo de CPU: %d",novo->tempo_cpu);
         printf("\n|Prioridade : %d",novo->prioridade);
         printf("\n|Tempo e Chegada: %d",novo->tempo_chegada);
         if (novo->filaIO[0] != -10) printf("\n|Tempo do I/O: ");
         
         int j = 0;
         while(novo->filaIO[j] != -10){
            printf(" %d ", novo->filaIO[j]);
            j++;
         }
         printf("\n+---------------------------\n\n");
         novo = novo->prox;
      }
   }else{ printf("-----Fila de Processos PRONTOS VAZIA -\n"); }
   /* Fim da Fila de Processos PRONTOS*/ 

   /* Começo da Fila de Processos Finalizados */ 
   if (gerenciador->fila_finalizados != NULL){ 
      novo = gerenciador->fila_finalizados->prox;
      printf("\033[1;35m");
      printf("Fila de Processos Finalizados: \n");
      while(novo->id != -10 && novo != NULL ){
         printf("+---------------------------");
         printf("\n|Id do processo: %d",novo->id);
         printf("\n|Tempo de CPU: %d",novo->tempo_cpu);
         printf("\n|Tempo de Chegada: %d",novo->tempo_chegada);
         printf("\n|Tempo de Tempo Inicio: %d",novo->tempo_inicio);
         printf("\n|Tempo de Tempo Fim: %d",novo->tempo_fim);
         printf("\n|Tempo Executado: %d",novo->tempo_executado);
         printf("\n+---------------------------\n\n");
         novo = novo->prox;
      }
   }else{ printf("-----Fila de Processos Finalizados VAZIA -\n"); }
   /* Fim da Fila de Processos PRONTOS*/ 

   printf("\033[0m");
}