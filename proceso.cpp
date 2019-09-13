#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include "timer.hh"
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/shm.h>
//https://www.geeksforgeeks.org/fork-system-call/
//https://timmurphy.org/2014/04/26/using-fork-in-cc-a-minimum-working-example/
//el vfork si comparte memoria, el fork no
using namespace std;

//double **newGrid = (double **) mmap(NULL, ARRAY_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);

int **m1;
int **m2;
int **r;
int pid;


int main(int argc, char **argv)
{
  int TAM;
  TAM = strtol(argv[1], NULL, 10);

  key_t Clave = ftok ("/bin/ls", 33);

  int id = shmget(Clave, sizeof(int)*TAM, 0777 | IPC_CREAT);
  int **r = (int **)shmat(id, 0, 0);


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
/*
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
*/
  int pepito=0;
  int cont=0;
  ScopedTimer p;
  //recuerda que el fork funciona como 2 ^n entonces 2 ^5 nos saca 32 datos o entradas a proceso hijo
  for (int i=0; i<TAM; i++){
      pid = vfork();
      //cout<<"este es pid: "<<pid<<endl;
      if (pid<0){
        cout<<"error, el pid es menor a cero";
      }
      //child
      if (pid==0){
        //cout<<"entre"<<endl;
        for(int i=0; i<TAM; i++){
          for(int j=0; j<TAM; j++){
            //*(*(r+pepito)+cont) += *(*(m1+pepito)+i) * *(*(m2+i)+pepito);
            *(*(r+pepito)+i) += *(*(m1+pepito)+j) * *(*(m2+j)+i);
            //cout<<"soy m1: "<<*(*(m1+pepito)+j)<<endl;
            //cout<<"soy m2: "<<*(*(m2+j)+i)<<endl;
            //cout<<"soy r: "<<*(*(r+pepito)+i)<<endl;

            //cout<<"soy m1: "<<*(*(m1+pepito)+i)<<endl;
            //cout<<"soy m2: "<<*(*(m2+i)+pepito)<<endl;
            //cout<<"soy r: "<<*(*(r+pepito)+cont)<<endl;
            //cout<<"soy i: "<<i<<endl;
          }
        }
        exit(0);
      }

      //father
      if (pid>0){
        //cout<<"soy cont"<<cont<<endl;
        //cout<<"soy pepito"<<pepito<<endl;
        //cont+=1;
        pepito+=1;
        wait(0);
      }
    }


  //cout<<"hola soy pepito "<<pepito<<endl;
  cout<<TAM<<","<<p.elapsed()/1e+6<<endl;
  //shmdt(r);
  //shmctl(id,IPC_RMID,NULL);
  //}
/*
  cout<<endl
      << "Multiplicacion fork" << endl;
  for (int i=0; i<TAM; i++){
      for (int j=0; j<TAM; j++){
          cout<<*(*(r+i)+j) <<" ";
      }
      cout<<endl;
  }
*/
  return 0;
}
