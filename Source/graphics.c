#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "main.h"



void DesenhaGrid(Control controledoprog ){
    int x1,x2,k,count;
    double size ;
    setbkcolor(1);
    x2 = 0;
    setcolor(9);
    line(0,600,800,600);
    line(0,150,800,150);
    line(0,300,800,300);
    line(0,450,800,450);
    if(strcmp(controledoprog.mode,"log") == 0){
        size=log10(controledoprog.freqf/controledoprog.freqi);
        k = (int)(controledoprog.freqi*pow(10,(int)(floor(size))))%10;
        x2 = -(int)(800*log10(k)/size);   
        printf("%f %d",controledoprog.freqi*pow(10,(int)(floor(size))),(int)(floor(size)));
        for(int i = 0 ;i <= (int)(size)+1 ;i++){
            for (count = k; count<=10; count++){
                x1 = (int)(800*log10(count)/size)+x2;   
                 if (!(gety() % 10)) {  
                      setcolor(9);
                      line (gety(), 0, gety(), 800);
                    }
                    line(x1,0,x1, 600);
                }
            x2 = x1;
            k = 1 ;
            }  
    }else{
        for(int i = 0 ;i <= controledoprog.pontos ;i++){
        x1 = (int)(800*i/controledoprog.pontos);
        if (!(gety() % 10)) {
          setcolor(9);
          line (gety(), 0, gety(), 800);
        }
        line(x1,0,x1, 600); }
    }
}

void printGraphics(Control controledoprog ,double *freq,int el,double resultMat[MAX_SPOINTS+3][MAX_NOS+1][2]){
      int x1,x2,y1,y2, count;
      double result[MAX_SPOINTS+3][MAX_NOS+1][2] ;
      initwindow(800,600); 
      moveto(400,0); 
      DesenhaGrid(controledoprog);
      setcolor(0);        
      for (count = 0; count<controledoprog.pontos; count++){
       if(strcmp(controledoprog.mode,"log") == 0){
            
            x1 = (int)(800*log10(freq[count]/freq[0])/log10(freq[controledoprog.pontos]/freq[0]));
            y1 = (int)(3.0*(resultMat[count][el][0]+150.0));
            x2 = (int)(800*log10(freq[count+1]/freq[0])/log10(freq[controledoprog.pontos]/freq[0]));
            y2 =(int)(3.0*(resultMat[count+1][el][0]+150.0));
               }
       else{
             x1 = (int)(800*(freq[count-1]-controledoprog.freqi)/(controledoprog.freqf-controledoprog.freqi));
             y1 = (int)(1.5*(resultMat[count][el][0]+150.0));
             x2 =(int)(800*(freq[count]-controledoprog.freqi)/(controledoprog.freqf-controledoprog.freqi));
             y2 =(int)(1.5*(resultMat[count+1][el][0]+150.0));
        }
        if (!(gety() % 10)) { /*Grid -> não está preenchendo tudo...*/
          setcolor(6);
          line (gety(), 0, gety(), 800);
          setcolor(-1);
        }
        line(x1, 600-y1,x2, 600-y2); /*Fica muuito comprimido, mas essa é a idéia... Talvez estendendo a linha por um fator*/
      }
      setcolor(2);
      for (int count = 0; count<controledoprog.pontos; count++){
       if(strcmp(controledoprog.mode,"log") == 0){
        
            x1 = (int)(800*log10(freq[count]/freq[0])/log10(freq[controledoprog.pontos]/freq[0]));
            y1 = (int)(1.5*(resultMat[count][el][1]+200.0));
            x2 = (int)(800*log10(freq[count+1]/freq[0])/log10(freq[controledoprog.pontos]/freq[0]));
            y2 =(int)(1.5*(resultMat[count+1][el][1]+200.0));
               }
       else{
    
             x1 = (int)(800*(freq[count-1]-controledoprog.freqi)/(controledoprog.freqf-controledoprog.freqi));
             y1 = (int)(1.5*(resultMat[count][el][1]+200.0));
             x2 =(int)(800*(freq[count]-controledoprog.freqi)/(controledoprog.freqf-controledoprog.freqi));
             y2 =(int)(1.5*(resultMat[count+1][el][1]+200.));
        }
        if (!(gety() % 10)) { /*Grid -> não está preenchendo tudo...*/
          setcolor(2);
          line (gety(), 0, gety(), 800);
          
        }
        line(x1, 600-y1,x2, 600-y2); /*Fica muuito comprimido, mas essa é a idéia... Talvez estendendo a linha por um fator*/
      }     
            
      while(!kbhit());     //wait for user to press a key
      closegraph();        //close graphics window
     };
