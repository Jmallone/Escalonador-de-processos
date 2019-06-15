#include <stdio.h>
#include"TAD-gp.h"
#include"TAD-escalonador.h"

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
   }
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
   }
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
   }
   /* Fim da Fila de Processos PRONTOS*/ 

   printf("\033[0m");
}

int main ( void )
{
   system("clear\n");
   char* filename = "input.txt";
   GP* gerenciador = newGP(filename);

   int escolha = 0;
   int rodando = 1;
   while(rodando){
      printf("\nEscolha uma Opcao:");
      printf("\n 1- Verificar Processos");
      printf("\n 2- Ordenar Por Tempo de Chegada");
      printf("\n 3- FIFO");
      printf("\n 4- Round Robin");
      printf("\n 5- Fila com Prioridade");
      printf("\n 0- Sair\n");

      scanf("%d",&escolha);
      system("clear\n");
      switch ( escolha ) {
      case 1:
         verificar(gerenciador);
         break;
      case 2:
         printf(" ------ Ordenação -------- ");
         sortList(gerenciador->fila_processos);
         break;
      case 3:
         printf(" ------ FIFO -------- ");
         FIFO(gerenciador);
         break;
      case 4:
         printf(" ------ RR -------- ");
         break;
      case 5:
         printf(" ------ Prioridade -------- ");
         break;
      case 0:
         rodando = 0;
         break;
      default:
         printf("\n ESCOLHA UMA OPCAO VALIDA! \n");
      break;
      }
   }
   return 0;
}