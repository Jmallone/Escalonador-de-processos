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
