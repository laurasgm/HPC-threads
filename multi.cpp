#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include "timer.hh"
using namespace std;

#define MAX_THREAD 4

//https://www.geeksforgeeks.org/multiplication-matrix-using-pthreads/
//https://www.tutorialspoint.com/cplusplus/cpp_multithreading.htm#
//que ingrese por srg el tamañano de la matriz y si va a ser por serie o por paralelo
//ejecucion
//g++ -o multi multi.cpp -pthread
//./multi TAM op

//manejamos matrices dinamicas
int **m1;
int **m2;
int **r;
int **m3;
int **m4;
int step_i = 0;


//multiplicacion en paralelo
void* multi(void* arg)
{
    long TAM;
    TAM = (long)arg;
    int core = step_i++;

    // Each thread computes 1/4th of matrix multiplication
    for (int i = core * TAM / 4; i < (core + 1) * TAM / 4; i++)
        for (int j = 0; j < TAM; j++)
            for (int k = 0; k < TAM; k++)
                *(*(m3+i)+j) += *(*(m1+i)+k) * *(*(m4+j)+k);//transpuesta activa

}

//multiplicacion en secuencial
void multi_secuencial(int TAM){
    for(int i=0; i<TAM; i++){
      for(int j=0; j<TAM; j++){
          *(*(r+i)+j) = 0;
          for(int k=0; k<TAM; k++){
                *(*(r+i)+j) += *(*(m1+i)+k)  * *(*(m2+k)+j) ;
          }
      }
    }
}

void imprimir_secuencial(int TAM){
    cout<<endl
        << "Multiplicacion secuencial" << endl;
    for (int i=0; i<TAM; i++){
        for (int j=0; j<TAM; j++){
            cout<<*(*(r+i)+j) <<" ";
        }
        cout<<endl;
    }
}

void imprimir_paralelo(int TAM){
    cout << endl
         << "Multiplicacion por hilos" << endl;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++)
            cout << *(*(m3+i)+j)  << " ";
        cout << endl;
    }
}

int main (int argc, char **argv)
{

  // argc es 3 = argv[0]nombre del programa,argv[1]TAM,argv[2]op
    int TAM;
    TAM = strtol(argv[1], NULL, 10);
    int op;
    op = strtol(argv[2], NULL, 10);


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

  //reservar memoria para la matriz dinamica
  m3 = new int*[TAM];//reservando memoria para las filas
  for (int i=0;i<TAM;i++){
    m3[i]= new int[TAM];//reservando memoria para las columnas
  }

  //reservar memoria para la matriz dinamica
  m4 = new int*[TAM];//reservando memoria para las filas
  for (int i=0;i<TAM;i++){
    m4[i]= new int[TAM];//reservando memoria para las columnas
  }

	//llenado

  for (int i=0; i<TAM; i++){

      for (int j=0; j<TAM; j++){
          *(*(m1+i)+j) =rand() % 10 + 1;
          *(*(m2+i)+j) =rand() % 10 + 1;
      }
  }

  //transpuesta
  for (int i=0; i<TAM; i++){

      for (int j=0; j<TAM; j++){
          *(*(m4+j)+i) = *(*(m2+i)+j);
      }
  }

//imprime las matrices a multiplicar
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
            cout<<m4[i][j]<<"\t";
        }
        cout<<endl;
    }
*/
    switch(op){
      case 1: //t0=clock();
              {
              ScopedTimer p;
              multi_secuencial(TAM);
              //imprimir_secuencial(TAM);
              /*t1=clock();
              double tf;
              tf= ((double) (t1- t0)) / CLOCKS_PER_SEC;
              cout<<tf<<endl;*/

              cout<<TAM<<","<<p.elapsed()/1e+6<<endl;
              }
              break;
      case 2: // declaramos 4 hilos
              //t2=clock();
              {ScopedTimer t;
              pthread_t threads[MAX_THREAD];

              // creando cuatro hilos, cada uno evaluando su propia parte
              for (int i = 0; i < MAX_THREAD; i++) {

                  pthread_create(&threads[i], NULL, multi, (void *)TAM);
              }

              // joining and waiting for all threads to complete
              //Join thread:The function returns when the thread execution has completed.
              for (int i = 0; i < MAX_THREAD; i++)
                  pthread_join(threads[i], NULL);

              //imprimir_paralelo(TAM);
              /*t3=clock();
              double tf1;
              tf1= ((double) (t3- t2)) / CLOCKS_PER_SEC;
              cout<<tf1<<endl;*/
              t.elapsed();
              cout<<TAM<<","<<t.elapsed()/1e+6<<endl;
              }
              break;
    }


}
