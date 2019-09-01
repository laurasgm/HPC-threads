#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
using namespace std;

#define MAX_THREAD 4

//https://www.geeksforgeeks.org/multiplication-matrix-using-pthreads/
//que ingrese por srg el tamañano de la matriz y si va a ser por serie o por paralelo
//ejecucion
//g++ -o multi multi.cpp -pthread
//./multi TAM op

int m1[2][2];
int m2[2][2];
int r[2][2];
int m3[2][2];
int step_i = 0;


void* multi(void* arg)
{
    long TAM;
    TAM = (long)arg;
    int core = step_i++;

    // Each thread computes 1/4th of matrix multiplication
    for (int i = core * TAM / 4; i < (core + 1) * TAM / 4; i++)
        for (int j = 0; j < TAM; j++)
            for (int k = 0; k < TAM; k++)
                m3[i][j] += m1[i][k] * m2[k][j];
}

void multi_secuencial(int TAM){
    for(int i=0; i<TAM; i++){
      for(int j=0; j<TAM; j++){
          r[i][j]=0;
          for(int k=0; k<TAM; k++){
                r[i][j]= r[i][j]+m1[i][k]*m2[k][j];
          }
      }
    }
}

void imprimir_secuencial(int TAM){
    cout<<endl
        << "Multiplicacion secuencial" << endl;
    for (int i=0; i<TAM; i++){
        for (int j=0; j<TAM; j++){
            cout<<r[i][j]<<" ";
        }
        cout<<endl;
    }
}

void imprimir_paralelo(int TAM){
    cout << endl
         << "Multiplicacion por hilos" << endl;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++)
            cout << m3[i][j] << " ";
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

	//llenado

    for (int i=0; i<TAM; i++){

        for (int j=0; j<TAM; j++){
            m1[i][j]=rand() % 10 + 1;
            m2[i][j]=rand() % 10 + 1;
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

    switch(op){
      case 1: multi_secuencial(TAM);
              imprimir_secuencial(TAM);
              break;
      case 2: // declaramos 4 hilos
              pthread_t threads[MAX_THREAD];

              // creando cuatro hilos, cada uno evaluando su propia parte
              for (int i = 0; i < MAX_THREAD; i++) {

                  pthread_create(&threads[i], NULL, multi, (void *)TAM);
              }

              // joining and waiting for all threads to complete
              //Join thread:The function returns when the thread execution has completed.
              for (int i = 0; i < MAX_THREAD; i++)
                  pthread_join(threads[i], NULL);

              imprimir_paralelo(TAM);
              break;
    }


}
