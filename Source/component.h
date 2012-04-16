#ifndef COMPONENT
#define COMPONENT
#include "LinearAlgebra/algebra.h"

typedef struct elemento { /* Elemento do netlist */
  char nome[MAX_NOME];
  double valor1,valor2,valor3;
  int a,b,c,d,x,y;
} elemento;

void montarMOL (int MOL[MAX_NOS+1][MAX_NOS+2], elemento *netlist,int ne, int nv);
void montarMOC (int MOC[MAX_NOS+1][MAX_NOS+2], elemento *netlist,int ne, int nv);
int montarTOL (int MOL[MAX_NOS+1][MAX_NOS+2], int TOL[MAX_NOS+1], int nv);
int montarTOC (int MOC[MAX_NOS+1][MAX_NOS+2], int TOC[MAX_NOS+1], int nv);

void adicionarEstampa(Complex Yn[MAX_NOS+1][MAX_NOS+2], int TOL[MAX_NOS+1], int TOC[MAX_NOS+1], elemento *netlist,double freq,int ne, int nv);
double* FreqGenerartor(double fi ,double ff,int points,char* tipe,char *unit);

#endif
