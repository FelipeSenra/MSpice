#include "algebra.h"
#include <math.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0
typedef unsigned int boolean;

int elimGauss(Complex M[MAX_NOS+1][MAX_NOS+2], int n)
{
  unsigned int i, i2, j, j2, ipivo;
  Complex pivo, b, const1;
  Complex solucao[MAX_NOS+1];
  boolean flag[MAX_NOS+1];
  const1.Re = 1; const1.Im = 0;    
  
  /*flag[i] verdadeiro indica que a linha i ja foi usada como pivo*/
  for (i=1; i<n+1; i++)
    flag[i] = FALSE;
  
  /*O j varre coluna por coluna para pegar os pivos*/
  /*Espera-se que todos os elementos pivos estejam na diagonal*/
  for (j=1; j<n+1; j++)
  {    
    /*Na coluna j, pega a linha disponivel com o maior pivo*/
    pivo.Re = 0; pivo.Im = 0;
    for (i=1; i<n+1; i++)
    {
      if (!flag[i] && (absC(M[i][j])>absC(pivo)))
      {
        pivo = M[i][j];
        ipivo = i;
      }
    }

    /*Se nao entrar nesse if, a matriz pode estar mal condicionada*/
    if (absC(pivo) > TOLG)
    {
      /*Normalizando a linha pivo*/
      for (j2=j; j2<n+2; j2++)
        M[ipivo][j2] = divide(M[ipivo][j2] , pivo);
    
      /*Eliminacao propriamente dita*/
      flag[ipivo] = TRUE;
      for (i2=1; i2<n+1; i2++)
        if (i2 != ipivo)
        {
          b = M[i2][j];
          for (j2=j; j2<n+2; j2++)
            M[i2][j2] = sub(M[i2][j2] , multi(b, M[ipivo][j2]));
        }
    }
    else
      return 1; /*Matriz mal condicionada*/
  }
  
  /*Calcula a solucao, na ordem correta*/
  for (j=1; j<n+1; j++)
    for (i=1; i<n+1; i++)
    {
      if (absC(sub(M[i][j] , const1)) < TOLG) /*mesmo que M[i][j] == 1*/
      {
        solucao[j] = M[i][n+1];
        break;
      }
      if (i == n)
        return 2; /*Sistema nao foi resolvido (algum erro estranho ocorreu)*/
    }
    
  for (i=1; i<n+1; i++)
    M[i][n+1] = solucao[i];
  
  return 0;
}
/*Hugo*/

void resolversistema(Complex Yn[MAX_NOS+1][MAX_NOS+2], int nv,int nn)
{
    int i,j,l, k;
    int max[nv+1];
	Complex t;
	for (i = 0; i <= nv; i++) 
		max[i] = i;		
    for (i = 1; i < nv+1; i++) {
        /* algoritmo de pivoteamento */     
       if((Yn[i][i].Re == 0.0)&&(Yn[i][i].Im == 0.0))
        {
            printf(" pivotei ");
            for (j = i + 1; j <= nv+1; j++)
			if ((Yn[j][i].Re != 0.0)||(Yn[j][i].Im != 0.0))
				max[i] = j;
		    if(max[i] == i)
           	printf("Mal condicionada");		
    		if((max[i] != i)&&(max[i] != nv+1))	
    		for (j = 1; j <= nv + 2; j++) {
    			t = Yn[max[i]][j];
    		    Yn[max[i]][j] = Yn[i][j];
    			Yn[i][j] = t;
    		}
    	
        }
        /* divide a linha i por aii*/
       t = Yn[i][i];
        for (k = 1 ; k <= nv+1 ; k++){
            Yn[i][k] = divide(Yn[i][k],t);
        }
        for (k = i+1 ; k <= nv+1 ; k++){
            t = Yn[k][i];
                for (l = i ; l <= nv+1 ; l++){
                    Yn[k][l] = sub(Yn[k][l],multi(t,Yn[i][l]));
                }
            }
        }           
       	for (i = nv; i >= 1;i--) {
    		for (j = i - 1; j >= 1; j--) {
    			Yn[j][nv+1] = sub(Yn[j][nv+1],multi(Yn[j][i],Yn[i][nv+1]));
    			Yn[j][i].Re = 0;
    			Yn[j][i].Im = 0;
    		}
    	}
  }


Complex add(Complex Yn,Complex Xn){
    Complex result ;
    result.Re = Yn.Re+Xn.Re ;
    result.Im = Yn.Im+Xn.Im ;
    return result ;
}

Complex sub(Complex Yn,Complex Xn){
 Complex result ;
result.Re = Yn.Re-Xn.Re ;
result.Im = Yn.Im-Xn.Im ;
return result ;
}

/*
  y/x
*/

Complex divide(Complex Yn,Complex Xn){
 Complex result ;
result.Re = (Yn.Re*Xn.Re+Yn.Im*Xn.Im)/(Xn.Re*Xn.Re+Xn.Im*Xn.Im);
result.Im = (Yn.Im*Xn.Re-Yn.Re*Xn.Im)/(Xn.Re*Xn.Re+Xn.Im*Xn.Im);
 
return result ;
}

Complex multi(Complex Yn,Complex Xn){
 Complex result ;
result.Re = Yn.Re*Xn.Re-Yn.Im*Xn.Im ;
result.Im = Yn.Re*Xn.Im+Yn.Im*Xn.Re ;
return result ;
}

double absC(Complex Yn){
  return sqrt(Yn.Re*Yn.Re+Yn.Im*Yn.Im);
}

double angleC(Complex Yn){
       return atan2 (Yn.Im,Yn.Re) * 180 / PI;
       }
       
void printComplex(Complex Yn){
     printf(" %.2g+j%.2g ",Yn.Re,Yn.Im);
     }
     
void printComplexMatrix(Complex Yn[MAX_NOS+1][MAX_NOS+2],int nv){
     int ip ;
     int jp ;
     for (ip = 1 ; ip <= nv; ip++){
           for(jp = 1 ; jp <= nv+1; jp++){
                 printf("[%d,%d]", ip, jp);
                 printComplex(Yn[ip][jp]);
                 printf("\t");
           }
           printf("\n");
     }
     printf("\n");
}


void ZerarMatrix(Complex Yn[MAX_NOS+1][MAX_NOS+2]){
      for (int k=0; k<MAX_NOS+1; k++) {
            for (int j=0; j<MAX_NOS+2; j++){
              Yn[k][j].Re = 0.0;
              Yn[k][j].Im = 0.0;
              }
          } 
     }
