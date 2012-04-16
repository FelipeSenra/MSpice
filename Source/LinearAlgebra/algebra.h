#ifndef ALGEBRA
#define ALGEBRA "algebra.h"

#define TOLG 1e-9
#define MAX_NOS 50
#define PI 3.14159265

typedef struct complex{
double Re,Im;
}Complex ;

/*Hugo*/
int elimGauss(Complex M[MAX_NOS+1][MAX_NOS+2], int n);
/*Retorna 0 se sucesso, 1 se a matriz eh mal condicionada e 2 se der pau.
Atencao: M nao eh retornada resolvida. A solucao eh dada pelo vetor 'solucao'.
Obs.: 'n' eh o 'nv'.*/
/*Hugo*/

void resolversistema(Complex Yn[MAX_NOS+1][MAX_NOS+2], int nv,int nn);
Complex add(Complex Yn,Complex Xn);
Complex sub(Complex Yn,Complex Xn);
Complex divide(Complex Yn,Complex Xn);
Complex multi(Complex Yn,Complex Xn);
double absC(Complex Yn);
double angleC(Complex Yn);

void printComplex(Complex Yn);
void printComplexMatrix(Complex Yn[MAX_NOS+1][MAX_NOS+2],int nv);
void ZerarMatrix(Complex Yn[MAX_NOS+1][MAX_NOS+2]);

#endif
