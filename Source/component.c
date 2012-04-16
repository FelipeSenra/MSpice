#include <math.h>
#include <string.h>
#include "LinearAlgebra/algebra.h"
#include "main.h"
#include "component.h"

double result[MAX_SPOINTS] ;

/*Montar Matriz de Otimizacao de Linha (MOL)*/
/*Essa matriz funciona assim: se MOL[i][j] == 1, entao as linhas i e j da
  matriz nao otimizada devem ser somadas, caso contrario sera MOL[i][j] == 0.
  Isso sera usado pelas funcoes montarTOL e montarTOC, que, por sua vez,
  serao usadas para montar a matriz ja otimizada em adicionarEstampa.*/
void montarMOL (int MOL[MAX_NOS+1][MAX_NOS+2], elemento *netlist,int ne, int nv)
{
  int i, j;
  char tipo;
  
  /*Zerar MOL*/
  for (i=0; i<=nv; i++)
    for (j=0; j<=nv; j++)
      MOL[i][j] = 0;
  
  /*Montar MOL*/
  for (i=0; i<ne; i++)
  {
    tipo=netlist[i].nome[0];
    
    MOL[netlist[i].a][netlist[i].a] = 1;
    MOL[netlist[i].b][netlist[i].b] = 1;
    
    if (tipo=='G' || tipo=='K' || tipo=='E' || tipo=='F' || tipo=='H' || tipo=='O')
    {
      MOL[netlist[i].c][netlist[i].c] = 1;
      MOL[netlist[i].d][netlist[i].d] = 1;
    }
    if (tipo=='K' || tipo=='V' || tipo=='E' || tipo=='F' || tipo=='H' || tipo=='O')
      MOL[netlist[i].x][netlist[i].x] = 1;
    if (tipo=='K' || tipo=='H')
      MOL[netlist[i].y][netlist[i].y] = 1;
      
    if (tipo=='V' || tipo=='E' || tipo=='H' || tipo=='O')
    {
      MOL[netlist[i].a][netlist[i].b] = 1;
      MOL[netlist[i].b][netlist[i].a] = 1;
    }
    if (tipo=='F' || tipo=='H' || tipo=='O')
    {
      MOL[netlist[i].x][0] = 1;
      MOL[0][netlist[i].x] = 1;
    }
  }
  
  /*printf ("MOL:\n");
  for (i=0; i<=nv; i++)
  {
    for (j=0; j<=nv; j++)
      printf ("%d ", MOL[i][j]);
    printf ("\n");
  }
  printf ("\n");*/
}

/*Montar Matriz de Otimizacao de Coluna (MOC)*/
void montarMOC (int MOC[MAX_NOS+1][MAX_NOS+2], elemento *netlist,int ne, int nv)
{
  int i, j;
  char tipo;
  
  /*Zerar MOC*/
  for (i=0; i<=nv; i++)
    for (j=0; j<=nv; j++)
      MOC[i][j] = 0;
  
  /*Montar MOC*/
  for (i=0; i<ne; i++)
  {
    tipo=netlist[i].nome[0];
    
    MOC[netlist[i].a][netlist[i].a] = 1;
    MOC[netlist[i].b][netlist[i].b] = 1;
    
    if (tipo=='G' || tipo=='K' || tipo=='E' || tipo=='F' || tipo=='H' || tipo=='O')
    {
      MOC[netlist[i].c][netlist[i].c] = 1;
      MOC[netlist[i].d][netlist[i].d] = 1;
    }
    if (tipo=='K' || tipo=='V' || tipo=='E' || tipo=='F' || tipo=='H' || tipo=='O')
      MOC[netlist[i].x][netlist[i].x] = 1;
    if (tipo=='K' || tipo=='H')
      MOC[netlist[i].y][netlist[i].y] = 1;
      
    if (tipo=='F' || tipo=='H' || tipo=='O')
    {
      MOC[netlist[i].c][netlist[i].d] = 1;
      MOC[netlist[i].d][netlist[i].c] = 1;
    }
    if (tipo=='V' || tipo=='E' || tipo=='O')
    {
      MOC[netlist[i].x][0] = 1;
      MOC[0][netlist[i].x] = 1;
    }
    if (tipo=='H')
    {
      MOC[netlist[i].y][0] = 1;
      MOC[0][netlist[i].y] = 1;
    }
  }
  
  /*printf ("MOC:\n");
  for (i=0; i<=nv; i++)
  {
    for (j=0; j<=nv; j++)
      printf ("%d ", MOC[i][j]);
    printf ("\n");
  }
  printf ("\n");*/
}

/*Montar Tabela de Otimizacao de Linha (TOL)*/
/*Deve-se passar a matriz MOL ja preenchida e a tabela TOL para ser preenchida.
  nLinhasOtimizadas indica a ultima linha da matriz otimizada (a ser feita
  posteriormente ja em adicionarEstampa).
  TOL[n] indica em que linha da matriz otimizada ficara uma estampa que
  ficaria na linha n da matriz normal. Exemplo: ao inves de colocarmos algo em:
  Yn[netlist[i].a][netlist[i].b], colocamos em:
  Yn[TOL[netlist[i].a]][TOC[netlist[i].b]], onde TOC e o mesmo que TOL, mas
  para as colunas ao inves das linhas.*/
int montarTOL (int MOL[MAX_NOS+1][MAX_NOS+2], int TOL[MAX_NOS+1], int nv)
{
  int i, j;
  int nLO=0;
  TOL[0]=0;

  for (i=1; i<=nv; i++)
    for (j=0; j<=i; j++)
      if (MOL[i][j] == 1)
      {
        if (j==i)
          TOL[i] = ++nLO;
        else
          TOL[i] = TOL[j];
        break;
      }
      
  /*printf ("TOL:\n");
  for (i=0; i<=nv; i++)
    printf ("%d\n", TOL[i]);
  printf ("\n");*/
  
  return nLO;
}

/*Montar Tabela de Otimizacao de Coluna (TOC)*/
int montarTOC (int MOC[MAX_NOS+1][MAX_NOS+2], int TOC[MAX_NOS+1], int nv)
{
  int i, j;
  int nCO=0;
  TOC[0]=0;

  for (i=1; i<=nv; i++)
    for (j=0; j<=i; j++)
      if (MOC[i][j] == 1)
      {
        if (j==i)
          TOC[i] = ++nCO;
        else
          TOC[i] = TOC[j];
        break;
      }
            
  /*printf ("TOC:\n");
  for (i=0; i<=nv; i++)
    printf ("%d\n", TOC[i]);
  printf ("\n");*/
  
  return nCO;
}

/*Adiciona as estampas na matriz ja otimizada*/
void adicionarEstampa(Complex Yn[MAX_NOS+1][MAX_NOS+2], int TOL[MAX_NOS+1], int TOC[MAX_NOS+1], elemento *netlist,double freq,int ne, int nv){
    double g,L1,L2,M ;
    
    for(int i = 0 ;i <ne;i++ ){
        char tipo=netlist[i].nome[0];
        if (tipo=='R') {
          g=1/netlist[i].valor1;
          /*printf("%f %f\n",g,freq);*/
          Yn[TOL[netlist[i].a]][TOC[netlist[i].a]].Re+=g;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].b]].Re+=g;
          Yn[TOL[netlist[i].a]][TOC[netlist[i].b]].Re-=g;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].a]].Re-=g;
        }
        else if (tipo=='G') {
          g=netlist[i].valor1;
          Yn[TOL[netlist[i].a]][TOC[netlist[i].c]].Re+=g;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].d]].Re+=g;
          Yn[TOL[netlist[i].a]][TOC[netlist[i].d]].Re-=g;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].c]].Re-=g;
        }
        else if (tipo=='C') {
          g=netlist[i].valor1*freq;
          Yn[TOL[netlist[i].a]][TOC[netlist[i].a]].Im+=g;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].a]].Im-=g;
          Yn[TOL[netlist[i].a]][TOC[netlist[i].b]].Im-=g;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].b]].Im+=g;
        }
        else if (tipo=='L') {
          g=(-1.0)/(netlist[i].valor1*freq);
          Yn[TOL[netlist[i].a]][TOC[netlist[i].a]].Im+=g;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].a]].Im-=g;
          Yn[TOL[netlist[i].a]][TOC[netlist[i].b]].Im-=g;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].b]].Im+=g;
          }
       else if (tipo=='K') {
          L1=(netlist[i].valor1*freq);
          L2=(netlist[i].valor2*freq);
          M = netlist[i].valor3*sqrt(L1*L2); 
          Yn[TOL[netlist[i].a]][TOC[netlist[i].x]].Re+=1;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].x]].Re-=1;
          Yn[TOL[netlist[i].c]][TOC[netlist[i].y]].Re+=1;
          Yn[TOL[netlist[i].d]][TOC[netlist[i].y]].Re-=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].a]].Re-=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].b]].Re+=1;
          Yn[TOL[netlist[i].y]][TOC[netlist[i].c]].Re-=1;
          Yn[TOL[netlist[i].y]][TOC[netlist[i].d]].Re+=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].x]].Im+=L1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].y]].Im+=M;
          Yn[TOL[netlist[i].y]][TOC[netlist[i].x]].Im+=M;
          Yn[TOL[netlist[i].y]][TOC[netlist[i].y]].Im+=L2;
        }
        else if (tipo=='I') {
          g=netlist[i].valor1;
          Yn[TOL[netlist[i].a]][nv+1].Re-=g;
          Yn[TOL[netlist[i].b]][nv+1].Re+=g;
          g=netlist[i].valor2;
          Yn[TOL[netlist[i].a]][nv+1].Im-=g;
          Yn[TOL[netlist[i].b]][nv+1].Im+=g;
        
        }
        else if (tipo=='V') {
          Yn[TOL[netlist[i].a]][TOC[netlist[i].x]].Re+=1;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].x]].Re-=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].a]].Re-=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].b]].Re+=1;
          Yn[TOL[netlist[i].x]][nv+1].Re-=netlist[i].valor1;
          Yn[TOL[netlist[i].x]][nv+1].Im-=netlist[i].valor2;
        }
        else if (tipo=='E') {
          g=netlist[i].valor1;
          Yn[TOL[netlist[i].a]][TOC[netlist[i].x]].Re+=1;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].x]].Re-=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].a]].Re-=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].b]].Re+=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].c]].Re+=g;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].d]].Re-=g;
        }
        else if (tipo=='F') {
          g=netlist[i].valor1;
          Yn[TOL[netlist[i].a]][TOC[netlist[i].x]].Re+=g;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].x]].Re-=g;
          Yn[TOL[netlist[i].c]][TOC[netlist[i].x]].Re+=1;
          Yn[TOL[netlist[i].d]][TOC[netlist[i].x]].Re-=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].c]].Re-=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].d]].Re+=1;
        }
        else if (tipo=='H') {
          g=netlist[i].valor1;
          Yn[TOL[netlist[i].a]][TOC[netlist[i].y]].Re+=1;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].y]].Re-=1;
          Yn[TOL[netlist[i].c]][TOC[netlist[i].x]].Re+=1;
          Yn[TOL[netlist[i].d]][TOC[netlist[i].x]].Re-=1;
          Yn[TOL[netlist[i].y]][TOC[netlist[i].a]].Re-=1;
          Yn[TOL[netlist[i].y]][TOC[netlist[i].b]].Re+=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].c]].Re-=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].d]].Re+=1;
          Yn[TOL[netlist[i].y]][TOC[netlist[i].x]].Re+=g;
        }
        else if (tipo=='O') {
          Yn[TOL[netlist[i].a]][TOC[netlist[i].x]].Re+=1;
          Yn[TOL[netlist[i].b]][TOC[netlist[i].x]].Re-=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].c]].Re-=1;
          Yn[TOL[netlist[i].x]][TOC[netlist[i].d]].Re+=1;
        }
    }
}

double* FreqGenerartor(double fi ,double ff,int points,char *tipe,char *unit){
       double k,esc,l;
       if(unit[0] == 'r')
           esc =  1.0;     
       else
           esc = 6.28; 
       if(strcmp(tipe,"log") == 0){
               l =  1.0/points;
               k = pow(ff/fi,l);
               if(k < 1+TOLG)
                k = 1+TOLG;
               result[0] = esc*fi ;
               for(int i = 1 ;i < points+1;i++){
               result[i] = k*result[i-1];
               }
        return result; 
            }
                k = esc*(ff-fi)/points;
                result[0] = esc*fi ;
                for(int i = 1 ;i < points+1;i++)
                result[i] = result[i-1]+ k ;
   
           return result; 
       }
