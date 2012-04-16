#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <graphics.h>

#include "LinearAlgebra/algebra.h"
#include "main.h"
#include "component.h"
#include "graphics.h"

#define MAX_LINHA 80
#define MAX_NOME 20
#define MAX_ELEM 50
#define MAX_NOS 50
#define TOLG 1e-9
#define DEBUG


Control controledoprog ;

elemento netlist[MAX_ELEM]; /* Netlist */

int
  ne, /* Elementos */
  nv, /* Variaveis */
  nn, /* Nos */
  ns, /*  pontos de simulacao*/
  el,/*  elemento a ser plotado*/
  i,j,k,
  O ;

char
/* Foram colocados limites nos formatos de leitura para alguma protecao
   contra excesso de caracteres nestas variaveis */
  filename[MAX_NOME+1], filename2[MAX_NOME+1],
  tipo,
  na[MAX_NOME],nb[MAX_NOME],nc[MAX_NOME],nd[MAX_NOME],
  lista[MAX_NOS+1][MAX_NOME+2], /*Tem que caber jx antes do nome */
  txt[MAX_LINHA+1];
char  *p;
FILE *arquivo,*arquivoSaida;
double resultMat[MAX_SPOINTS+3][MAX_NOS+1][2];
double  g,freqi,freqf;
double *freq;
Complex  Yn[MAX_NOS+1][MAX_NOS+2];

int MOL[MAX_NOS+1][MAX_NOS+2], MOC[MAX_NOS+1][MAX_NOS+2], TOL[MAX_NOS+1], TOC[MAX_NOS+1];
int nLinhasOtimizadas, nColunasOtimizadas;

int main(int argc,char** argv){    
      
      printf("Programa demonstrativo de analise nodal modificada Compacta por Op. Amp.\n");
      printf("Por:     Bruno Furtado\n\t Felipe Senra Ribeiro\n\t Hugo Spinelli \n");
      /*printf("Versao %s\n",versao);*/
      /* Leitura do netlist */
      
      menuFreq();
      return 0;
    }
    
int menuEstado(){
    printf("Nome do arquivo com o netlist (ex: mna.net): ");
      scanf("%50s",filename);
      printf("Nome do arquivo de Saida: ");
      
      scanf("%50s",filename2);
      arquivo=fopen(filename,"r");
      arquivoSaida=fopen(filename2,"w");
      
      if (arquivo==0) {
        printf("Arquivo %s inexistente\n",filename);
    //    goto denovo;
      }      
      ne=0; nv=0; strcpy(lista[0],"0");
      open(filename);
      
      for (i=1; i<=ne; i++) {
        tipo=netlist[i].nome[0];
        if (tipo=='V' || tipo=='E' || tipo=='F' || tipo=='O') {
          nv++;
          if (nv>MAX_NOS) {
            printf("As correntes extra excederam o numero de variaveis permitido (%d)\n",MAX_NOS);
            getchar();
            exit(1);
          }
          strcpy(lista[nv],"j"); /* Tem espaco para mais dois caracteres */
          strcat(lista[nv],netlist[i].nome);
          netlist[i].x=nv;
        }
        else if ((tipo=='H')||(tipo=='K')) {
          nv=nv+2;
          if (nv>MAX_NOS) {
            printf("As correntes extra excederam o numero de variaveis permitido (%d)\n",MAX_NOS);
            getchar();
            exit(1);
          }
          strcpy(lista[nv-1],"jx"); strcat(lista[nv-1],netlist[i].nome);
          netlist[i].x=nv-1;
          strcpy(lista[nv],"jy"); strcat(lista[nv],netlist[i].nome);
          netlist[i].y=nv;
        }
        else if ((tipo=='C')||(tipo=='R')) {
          nv=nv+2;
          if (nv>MAX_NOS) {
            printf("As correntes extra excederam o numero de variaveis permitido (%d)\n",MAX_NOS);
            getchar();
            exit(1);
          }
          strcpy(lista[nv-1],"jx"); strcat(lista[nv-1],netlist[i].nome);
          netlist[i].x=nv-1;
          strcpy(lista[nv],"jy"); strcat(lista[nv],netlist[i].nome);
          netlist[i].y=nv;
        }
        
      }
}
    
int menuFreq(){
    int i,k,j;
      char dir[5];
      int flagErro,nvconst;
      el = 1;
      O = 1;
      
     printf("Mostrar arquivos na pasta (y/n): ");
      scanf("%4s",dir);
      if(dir[0] == 'y')
                system("DIR");
      denovo:
      
      printf("Nome do arquivo com o netlist (ex: mna.net): ");
      scanf("%50s",filename);
      printf("Nome do arquivo de Saida: ");
      
      scanf("%50s",filename2);
      arquivo=fopen(filename,"r");
      arquivoSaida=fopen(filename2,"w");
      
      if (arquivo==0) {
        printf("Arquivo %s inexistente\n",filename);
        goto denovo;
      }      
      ne=0; nv=0; strcpy(lista[0],"0");
      open(filename);
    
      /* Acrescenta variaveis de corrente acima dos nos, anotando no netlist */
      nn=nv;
      for (i=1; i<=ne; i++) {
        tipo=netlist[i].nome[0];
        if (tipo=='V' || tipo=='E' || tipo=='F' || tipo=='O') {
          nv++;
          if (nv>MAX_NOS) {
            printf("As correntes extra excederam o numero de variaveis permitido (%d)\n",MAX_NOS);
            getchar();
            exit(1);
          }
          strcpy(lista[nv],"j"); /* Tem espaco para mais dois caracteres */
          strcat(lista[nv],netlist[i].nome);
          netlist[i].x=nv;
        }
        else if ((tipo=='H')||(tipo=='K')) {
          nv=nv+2;
          if (nv>MAX_NOS) {
            printf("As correntes extra excederam o numero de variaveis permitido (%d)\n",MAX_NOS);
            getchar();
            exit(1);
          }
          strcpy(lista[nv-1],"jx"); strcat(lista[nv-1],netlist[i].nome);
          netlist[i].x=nv-1;
          strcpy(lista[nv],"jy"); strcat(lista[nv],netlist[i].nome);
          netlist[i].y=nv;
        }
      }
      getchar();
     
      printf("Variaveis internas: \n");
      for (i=0; i<=nv; i++)
        printf("%d -> %s\n",i,lista[i]);
      getchar();
      printf("Netlist interno final\n");
      printNetlist(netlist);    
      getchar();
      /*Otimizacao*/
      montarMOL (MOL, netlist, ne, nv);
      montarMOC (MOC, netlist, ne, nv);
      nLinhasOtimizadas = montarTOL (MOL, TOL, nv);
      nColunasOtimizadas = montarTOC (MOC, TOC, nv);
      if (nLinhasOtimizadas != nColunasOtimizadas)
      {
        printf ("Erro na otimizacao!\n");
        system("Pause");
        return 1;
      }
      nv = nColunasOtimizadas;
      /* Monta o sistema nodal modificado */
      printf("O circuito tem %d nos, %d variaveis e %d elementos\n",nn,nv,ne);
      getchar();
       printf("Variavel a ser plotada: (1-%d)",nv);
      scanf("%d",&el);
      nvconst = nv ;
      write(arquivoSaida,Yn,nv-1,1,0);
      /* Monta estampas */
      printf("Frequencias : ");
      freq = FreqGenerartor(controledoprog.freqi,controledoprog.freqf,controledoprog.pontos,controledoprog.mode,controledoprog.unit);
      
      for (int count = 0; count<=controledoprog.pontos; count++){
            nv = nvconst;
            //printf("%f\n ",freq[count]/6.28);
            ZerarMatrix(Yn);
            adicionarEstampa(Yn,TOL,TOC,netlist,freq[count],ne,nv);
            //if(O == 1)
            //  OtimizarNetlist();
            //printf("O circuito tem  %d variaveis apos Otimização\n",nv);
            flagErro = elimGauss(Yn,nv);  /*caso o sistema nao tenha sido resolvido*/ 
            if (flagErro == 0) /* Sucesso */
            {                  
              //printf("Solucao na frequencia %f:\n",(freq[count]/6.28));
                for (i=1; i<nv+1; i++){
                    resultMat[count][i][0] = 20*log10((absC(Yn[i][nv+1])));
                    resultMat[count][i][1] = angleC(Yn[i][nv+1]);
              }
                write(arquivoSaida,Yn,nv,0,freq[count]);
            }
            else
            {
              printf("Erro: matriz mal condicionada!\n");
              /*Tratamento de erro*/
            }
      }
      
      printGraphics((Control)controledoprog,&(*freq),(int)el,resultMat);
     
}    


void open(char* filename){

  printf("Lendo netlist:\n");
  fgets(txt,MAX_LINHA,arquivo);
  printf("Titulo: %s",txt);
  while (fgets(txt,MAX_LINHA,arquivo)) {
    ne++; /* Nao usa o netlist[0] */
    if (ne>MAX_ELEM) {
      printf("O programa so aceita ate %d elementos\n",MAX_ELEM);
      exit(1);
    }
    txt[0]=toupper(txt[0]);
    tipo=txt[0];
    sscanf(txt,"%10s",netlist[ne].nome);
    p=txt+strlen(netlist[ne].nome); /* Inicio dos parametros */
    /* O que e lido depende do tipo */
    if (tipo=='I' || tipo=='V') {
      sscanf(p,"%10s%10s%lg%lg",na,nb,&netlist[ne].valor1,&netlist[ne].valor2);
      printf("%s %s %s %g\n",netlist[ne].nome,na,nb,netlist[ne].valor1,netlist[ne].valor2);
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
    }
    else if (tipo=='R' || tipo == 'L' || tipo == 'C') {
      sscanf(p,"%10s%10s%lg",na,nb,&netlist[ne].valor1);
      printf("%s %s %s %g\n",netlist[ne].nome,na,nb,netlist[ne].valor1);
      netlist[ne].a=numero(na);
      printf("%d",netlist[ne].a);
      netlist[ne].b=numero(nb);
    }
    else if (tipo=='G' || tipo=='E' || tipo=='F' || tipo=='H') {
      sscanf(p,"%10s%10s%10s%10s%lg",na,nb,nc,nd,&netlist[ne].valor1);
      printf("%s %s %s %s %s %lg\n",netlist[ne].nome,na,nb,nc,nd,netlist[ne].valor1);
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
      netlist[ne].c=numero(nc);
      netlist[ne].d=numero(nd);
    }
     else if (tipo=='K') {
      sscanf(p,"%10s%10s%10s%10s%lg%lg%lg",na,nb,nc,nd,&netlist[ne].valor1,&netlist[ne].valor2,&netlist[ne].valor3);
      printf("%s %s %s %s %s %g %g %g\n",netlist[ne].nome,na,nb,nc,nd,netlist[ne].valor1,netlist[ne].valor2,netlist[ne].valor3);
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
      netlist[ne].c=numero(nc);
      netlist[ne].d=numero(nd);
    }
    else if (tipo=='O') {
      sscanf(p,"%10s%10s%10s%10s",na,nb,nc,nd);
      printf("%s %s %s %s %s\n",netlist[ne].nome,na,nb,nc,nd);
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
      netlist[ne].c=numero(nc);
      netlist[ne].d=numero(nd);
    }
    else if (tipo=='*') { /* Comentario comeca com "*" */
      printf("Comentario: %s",txt);
      ne--;
    }
    else if (tipo=='.') { /* Aanalise "*" */        
        sscanf(p,"%10s%d%lg%lg%s\n",nb,&ns,&freqi,&freqf,nc);
        strcpy(controledoprog.anal,netlist[ne].nome);
        strcpy(controledoprog.mode,nb);
        controledoprog.pontos = ns;
        controledoprog.freqi = freqi;
        controledoprog.freqf = freqf;
        strcpy(controledoprog.unit,nc);
         }
    else {
      printf("Elemento desconhecido: %s\n",txt);
      getchar();
      exit(1);
    }
  }
  fclose(arquivo);
}

void OtimizarNetlist()
{
  int n,m,l,k;
  
  //printf("Matriz antes: nv = %d\n", nv);
  //printComplexMatrix (Yn, nv);
  
  for(int i =1 ;i <= ne;i++)
  {
    n = m = l = k = 0;
    tipo=netlist[i].nome[0];
            
    if(tipo=='V')
    {
      if(netlist[i].a > netlist[i].b)
      {
        n = netlist[i].a;
        m = netlist[i].b;
        l = netlist[i].x;
        k = 0;
      }
      else
      {
        m = netlist[i].a;
        n = netlist[i].b;
        l = netlist[i].x;
        k = 0;
      }             
    }
    else if(tipo=='O')
    {
      if(netlist[i].a > netlist[i].b)
      {
        n = netlist[i].a;
        m = netlist[i].b;
        l = netlist[i].x;
        k = 0;
      }
      else
      {
        m = netlist[i].a;
        n = netlist[i].b;
        l = netlist[i].x;
        k = 0;
      }
      
    /* Soma linhas n e m e zera a linha n */
    //printf("linha :%d %d \n",n,m);
    if(n != 0)
    {
      for(int i2 = 1;i2 <= nv+1;i2++)
        Yn[m][i2] = add(Yn[n][i2],Yn[m][i2]);
      /* Translada as linhas */
      for(int j2 = n;j2 <= nv-1;j2++)
        for(int i2 = 1;i2 <= nv+1;i2++)
          Yn[j2][i2] = Yn[j2+1][i2];
    }

    //printf("coluna :%d %d \n",l,k);
    if(l != 0)
    {
      /* Soma colunas l e k e zera a coluna l */
      for(int i2 = 1;i2 <= nv;i2++)
        Yn[i2][k] = add(Yn[i2][l],Yn[i2][k]);
      /* Translada as colunas */
      for(int j2 = l;j2 <= nv;j2++)
        for(int i2 = 1;i2 <= nv;i2++)
          Yn[i2][j2] = Yn[i2][j2+1];
            
      nv = nv-1;
    }
      
      if(netlist[i].c > netlist[i].d)
      {
        n = netlist[i].x;
        m = 0;
        l = netlist[i].c;
        k = netlist[i].d; 
      }
      else
      {
        n = netlist[i].x;
        m = 0;
        k = netlist[i].c;
        l = netlist[i].d;
      }      
    }
    else if(tipo=='F')
    {
      if(netlist[i].c > netlist[i].d)
      {
        n = netlist[i].x;
        m = 0;
        l = netlist[i].c;
        k = netlist[i].d;
      }
      else
      {
        n = netlist[i].x;
        m = 0;
        k = netlist[i].c;
        l = netlist[i].d;
      }             
    }
    else if(tipo=='E')
    {
      if(netlist[i].a > netlist[i].b)
      {
        n = netlist[i].a;
        m = netlist[i].b;
        l = netlist[i].x;
        k = 0;
      }
      else
      {
        m = netlist[i].a;
        n = netlist[i].b;
        l = netlist[i].x;
        k = 0;
      }
    }
    else if(tipo=='H')
    {
      if(netlist[i].a > netlist[i].b)
      {
        n = netlist[i].a;
        m = netlist[i].b;
        l = netlist[i].y;
        k = 0;
      }
      else
      {
        m = netlist[i].a;
        n = netlist[i].b;
        l = netlist[i].y;
        k = 0;
      }
      
    /* Soma linhas n e m e zera a linha n */
    //printf("linha :%d %d \n",n,m);
    if(n != 0)
    {
      for(int i2 = 1;i2 <= nv+1;i2++)
        Yn[m][i2] = add(Yn[n][i2],Yn[m][i2]);
      /* Translada as linhas */
      for(int j2 = n;j2 <= nv-1;j2++)
        for(int i2 = 1;i2 <= nv+1;i2++)
          Yn[j2][i2] = Yn[j2+1][i2];
    }

    //printf("coluna :%d %d \n",l,k);
    if(l != 0)
    {
      /* Soma colunas l e k e zera a coluna l */
      for(int i2 = 1;i2 <= nv;i2++)
        Yn[i2][k] = add(Yn[i2][l],Yn[i2][k]);
      /* Translada as colunas */
      for(int j2 = l;j2 <= nv;j2++)
        for(int i2 = 1;i2 <= nv;i2++)
          Yn[i2][j2] = Yn[i2][j2+1];
            
      nv = nv-1;
    }
      
      if(netlist[i].c > netlist[i].d)
      {
        n = netlist[i].x;
        m = 0;
        l = netlist[i].c;
        k = netlist[i].d; 
      }
      else
      {
        n = netlist[i].x;
        m = 0;
        k = netlist[i].c;
        l = netlist[i].d;
      } 
    }

    /* Soma linhas n e m e zera a linha n */
    //printf("linha :%d %d \n",n,m);
    if(n != 0)
    {
      for(int i2 = 1;i2 <= nv+1;i2++)
        Yn[m][i2] = add(Yn[n][i2],Yn[m][i2]);
      /* Translada as linhas */
      for(int j2 = n;j2 <= nv-1;j2++)
        for(int i2 = 1;i2 <= nv+1;i2++)
          Yn[j2][i2] = Yn[j2+1][i2];
    }

    //printf("coluna :%d %d \n",l,k);
    if(l != 0)
    {
      /* Soma colunas l e k e zera a coluna l */
      for(int i2 = 1;i2 <= nv;i2++)
        Yn[i2][k] = add(Yn[i2][l],Yn[i2][k]);
      /* Translada as colunas */
      for(int j2 = l;j2 <= nv;j2++)
        for(int i2 = 1;i2 <= nv;i2++)
          Yn[i2][j2] = Yn[i2][j2+1];
            
      nv = nv-1;
    }
  }

  //printf("Matriz depois: nv = %d\n", nv);
  //printComplexMatrix (Yn, nv);             		
}


void write(FILE *file,Complex Yn[MAX_NOS+1][MAX_NOS+2],int nv, int fist,double freq){
     double esc ;
     if(controledoprog.unit[0] == 'r')
        esc = 1.0 ;
     else
        esc = 6.28 ;
     if(fist == 1){
      fprintf(arquivoSaida,"freq\t\t");
      for (i=0; i<nv; i++)
         fprintf(arquivoSaida,"Modulo%d\t Fase%d \t",i+1,i+1);
         fprintf(arquivoSaida,"\n");
      }
      else{
      fprintf(arquivoSaida,"%.3g %s\t ",freq/esc,controledoprog.unit);
      for (i=1; i<nv+1; i++)
      
         fprintf(arquivoSaida,"%3.3lf %3.3lf\t ",(absC(Yn[i][nv+1])),angleC(Yn[i][nv+1]));
         fprintf(arquivoSaida,"\t\n");
      }
      
}

void printNetlist(elemento *netlist){
     char tipo ; 
     for (i=1; i<=ne; i++) {
        tipo=netlist[i].nome[0];
        if (tipo=='R' || tipo=='L' ||tipo=='C' ){
          printf("%s %d %d %e \n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].valor1);
        }
        if (tipo=='I' || tipo=='V') {
          printf("%s %d %d %g %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].valor1,netlist[i].valor2);
        }
        else if (tipo=='G' || tipo=='E' || tipo=='F' || tipo=='H'|| tipo=='K') {
          printf("%s %d %d %d %d %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,netlist[i].valor1);
        }
        else if (tipo=='K') {
          printf("%s %d %d %d %d %g %g %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,netlist[i].valor1,netlist[i].valor2,netlist[i].valor3);
        }
        else if (tipo=='O') {
          printf("%s %d %d %d %d\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d);
        }
        if (tipo=='V' || tipo=='E' || tipo=='F' || tipo=='O')
          printf("Corrente jx: %d\n",netlist[i].x);
        else if (tipo=='H'|| tipo=='K')
          printf("Correntes jx e jy: %d, %d\n",netlist[i].x,netlist[i].y);
      }
     
     }

int numero(char *nome)
{
  int i,achou;

  i=0; achou=0;
  while (!achou && i<=nv)
    if (!(achou=!strcmp(nome,lista[i]))) i++;
  if (!achou) {
    if (nv==MAX_NOS) {
      printf("O programa so aceita ate %d nos\n",nv);
      getchar();
      exit(1);
    }
    nv++;
    strcpy(lista[nv],nome);
    return nv; /* novo no */
  }
  else {
    return i; /* no ja conhecido */
  }
}
