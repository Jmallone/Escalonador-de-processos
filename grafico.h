#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define GNUPLOT "gnuplot -persist"


int insertoText(BCP* bcp); // Ordena a Lista de Processos

int gerarGrafico(); // Gera Grafico a partir de um Arquivo

int insertoText(BCP* bcp){


    //fprintf(gp, "P2   37   3   8    0\n");
    //#CPU_CLOCK(Tempo que entrou na CPU)  ID_PROCESSO TEMPO_EXECUTADO 0

    char str_tmp[10];
    char str[100] = "P";

    sprintf(str_tmp,"%d ",bcp->id);
    strcat(str,str_tmp);
    /*P4*/

    sprintf(str_tmp,"%d ",bcp->tempo_inicio);
    strcat(str,str_tmp);
    /*P4 4*/

    sprintf(str_tmp,"%d ",bcp->id);
    strcat(str,str_tmp);
    /*P4 4 2*/

    sprintf(str_tmp,"%d ",bcp->tempo_executado);
    strcat(str,str_tmp);
    /*P4 4 2 5 */

    strcat(str,"0\n");
    /*P4 4 2 5 0*/


    FILE * fptr;	
    fptr = fopen("plot_grafico.txt", "a");
    fputs(str, fptr);

    fclose (fptr);

    return 1;
}

int gerarGrafico(char * tmp){

    	FILE *gp;
	gp = popen(GNUPLOT, "w");	/* 'gp' is the pipe descriptor */
	if (gp == NULL) {
		printf("Error opening pipe to GNU plot.\n"
			"Install with 'sudo apt-get install gnuplot'\n");
		exit(0);
	}
	// write your gnu-plot commands here
	/*
    fprintf(gp, "$DATA << EOD\n");
	fprintf(gp, "P0   0   1   19    0\n");
	fprintf(gp, "P1   19   2   5    0\n");
	fprintf(gp, "P2   24   3   8    0\n");
	fprintf(gp, "P2   37   3   8    0\n");
	fprintf(gp, "EOD\n");
    */
	//#CPU_CLOCK (Tempo que entrou na CPU)  ID_PROCESSO TEMPO_EXECUTADO 0


	fprintf(gp, "set yrange [5:-5]\n");
	fprintf(gp, "set xtics scale 2, 0.5\n");
	fprintf(gp, "set ytics nomirror\n");
	fprintf(gp, "set xtics nomirror\n");

	fprintf(gp, "set grid x y\n");
	fprintf(gp, "unset key\n");
	fprintf(gp, "set title '{/=15 Escalanador - %s}'\n", tmp);
	fprintf(gp, "set xlabel 'Tempo de Clock'\n");
	fprintf(gp, "set ylabel 'Processos'\n");
	fprintf(gp, "set border 3\n");
	//fprintf(gp, "plot $DATA using 2:3:4:5: yticlabel(1) with vectors filled head lw 3\n");
	fprintf(gp, "plot 'plot_grafico.txt' using 2:3:4:5: yticlabel(1) with vectors filled head lw 7\n");
	
		
	fclose(gp);


    FILE * fptr;	
    fptr = fopen("plot_grafico.txt", "w");
    fputs("", fptr);

    fclose (fptr);
}