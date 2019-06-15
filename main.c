#include <stdio.h>
#include"TAD-gp.h"
#include"TAD-escalonador.h"

int main ( void )
{
   system("clear\n");
   char* filename = "input.txt";
   GP* gerenciador = newGP(filename);

   /* Menu */
   int escolha = 0;
   int rodando = 1;
   while(rodando){
      printf("\nEscolha uma Opcao:");
      printf("\n 1- Verificar Processos");
      printf("\n 2- Ordenar Por Tempo de Chegada");
      printf("\n 3- FIFO");
      printf("\n 4- Round Robin");
      printf("\n 5- SRTF");
      printf("\n 6- Fila com Prioridade");
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
         printf(" ------ Round Robin -------- ");
         break;
      case 5:
         printf(" ------ SRTF -------- ");
         break;
      case 6:
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