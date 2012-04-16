#ifndef MAIN
#define MAIN "main.h"

#define MAX_LINHA 80
#define MAX_NOME 20
#define MAX_ELEM 50
#define MAX_SPOINTS 500
#define MAX_NOS 50
#define TOLG 1e-9
#define DEBUG

#include<stdio.h>
#include<stdlib.h>
#include "component.h"

typedef struct  con{ /* Elemento de controle do programa */
  char anal[3];  
  char mode[3];
  int pontos;
  double freqi,freqf;
  char unit[3] ;
} Control;

int menuFreq();
int menuEstado();
void open(char* filename);
void write(FILE *file,Complex Yn[MAX_NOS+1][MAX_NOS+2],int nv,int fist, double freq);
int numero(char *nome);
void printNetlist(elemento *netlist );
void OtimizarNetlist();
void printGraphics( );


#endif
