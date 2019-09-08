#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include "timer.hh"
#include <sys/wait.h>



using namespace std;

int **m1;
int **m2;
int **r;
int pid;


int main(int argc, char **argv)
{
  int TAM;
  TAM = strtol(argv[1], NULL, 10);


  //reservar memoria para la matriz dinamica
  m1 = new int*[TAM];//reservando memoria para las filas
  for (int i=0;i<TAM;i++){
    m1[i]= new int[TAM];//reservando memoria para las columnas
  }

  //reservar memoria para la matriz dinamica
  m2 = new int*[TAM];//reservando memoria para las filas
  for (int i=0;i<TAM;i++){
    m2[i]= new int[TAM];//reservando memoria para las columnas
  }

  //reservar memoria para la matriz dinamica
  r = new int*[TAM];//reservando memoria para las filas
  for (int i=0;i<TAM;i++){
    r[i]= new int[TAM];//reservando memoria para las columnas
  }

  //llenado

  for (int i=0; i<TAM; i++){

      for (int j=0; j<TAM; j++){
          *(*(m1+i)+j) =rand() % 10 + 1;
          *(*(m2+i)+j) =rand() % 10 + 1;
      }
  }

  //imprimir
  for (int i=0; i<TAM; i++){

      for (int j=0; j<TAM; j++){
          cout<<m1[i][j]<<"\t";
      }
      cout<<endl;
  }

  //imprimir 2
  cout<<endl;
  for (int i=0; i<TAM; i++){

      for (int j=0; j<TAM; j++){
          cout<<m2[i][j]<<"\t";
      }
      cout<<endl;
  }

  ScopedTimer p;
  //recuerda que el fork funciona como 2 ^n entonces 2 ^5 nos saca 32 datos o entradas a proceso hijo 
  for (int i=0; i<5; i++){

    pid = fork();
    //cout<<"este es pid: "<<pid<<endl;
    if (pid<0){
      cout<<"error, el pid es menor a cero";
    }
    //child
    if (pid==0){
      //cout<<"entre"<<endl;
      for(int i=0; i<TAM; i++){
        for(int j=0; j<TAM; j++){
            r[i][j]=0;
            for(int k=0; k<TAM; k++){
              //cout<<"soy m1: "<<*(*(m1+i)+j)<<endl;
              //cout<<"soy m2: "<<*(*(m2+i)+j)<<endl;
              *(*(r+i)+j) += *(*(m1+i)+k) * *(*(m2+k)+j) ;
              //cout<<"soy r: "<<*(*(r+i)+j)<<endl;

            }
        }
      }
    }
    //father
    if (pid>0){
      wait(0);
    }

  }
  cout<<TAM<<","<<p.elapsed()/1e+6<<endl;
  /*cout<<endl
      << "Multiplicacion secuencial" << endl;
  for (int i=0; i<TAM; i++){
      for (int j=0; j<TAM; j++){
          cout<<*(*(r+i)+j) <<" ";
      }
      cout<<endl;
  }*/

  return 0;
}
