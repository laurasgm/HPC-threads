#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include "timer.hh"
using namespace std;

#define MAX_THREAD 4
#define MIN(A,B) (((A)<(B))?(A):(B))


/*para volver a enteros elimine la variable a, y genere el random de la siguiente manera
    rand() % 10 + 1
    tambien cambie todo los float a int*/

//manejamos matrices dinamicas
float **m1;
float **m2;
float **r;
//int *tem;
int step_i = 0;


//multiplicacion en paralelo
void* multi(void* arg)
{
    long TAM;
    TAM = (long)arg;
    int core = step_i++;
    int TAM_TILED = TAM*sizeof(double);


   // Each thread computes 1/4th of matrix multiplication
        //for (int i = core * TAM / 4; i < (core + 1) * TAM / 4; i++){
            for (int ii=0; ii<TAM; ii+=TAM_TILED){
                for(int jj=0; jj<TAM; jj+=TAM_TILED){
                    for(int kk=0; kk<TAM; kk+=TAM_TILED){
                        
                        for (int i = core * TAM / 4; i < (core + 1) * TAM / 4; i++){
                            for(int j=jj; j<MIN(jj+TAM_TILED,TAM); j++){
                                float temp=0;
                                for(int k=kk; k< MIN(kk+TAM_TILED,TAM); k++){
                                    temp += *(*(m1+i)+k) * *(*(m2+k)+j);                   
                                }
                                *(*(r+i)+j) += temp;
                            }
                        }
                    }
                }
          //  }
        }

}



void imprimir_paralelo(int TAM){
    cout << endl
         << "Multiplicacion por hilos" << endl;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++)
            cout << *(*(r+i)+j)  << " ";
        cout << endl;
    }
}

int main (int argc, char **argv)
{
    // argc es 3 = argv[0]nombre del programa,argv[1]TAM
    int TAM;
    TAM = strtol(argv[1], NULL, 10);


    //reservar memoria para la matriz dinamica
    m1 = new float*[TAM];//reservando memoria para las filas
    for (int i=0;i<TAM;i++){
        m1[i]= new float[TAM];//reservando memoria para las columnas
    }

    //reservar memoria para la matriz dinamica
    m2 = new float*[TAM];//reservando memoria para las filas
    for (int i=0;i<TAM;i++){
        m2[i]= new float[TAM];//reservando memoria para las columnas
    }

    //reservar memoria para la matriz dinamica
    r = new float*[TAM];//reservando memoria para las filas
    for (int i=0;i<TAM;i++){
        r[i]= new float[TAM];//reservando memoria para las columnas
    }

    
    //llenado
    float a = 10.0;
    for (int i=0; i<TAM; i++){
        for (int j=0; j<TAM; j++){
            *(*(m1+i)+j) =((float)rand()/(float)(RAND_MAX)) * a;
            *(*(m2+i)+j) =((float)rand()/(float)(RAND_MAX)) * a;
        }
    }

    //imprime las matrices a multiplicar
    /*
    //imprimir
    for (int i=0; i<TAM; i++){

        for (int j=0; j<TAM; j++){
            cout<<m1[i][j]<<"  ";
        }
        cout<<endl;
    }

    //imprimir 2
    cout<<endl;
    for (int i=0; i<TAM; i++){

        for (int j=0; j<TAM; j++){
            cout<<m2[i][j]<<"  ";
        }
        cout<<endl;
    }*/
    fflush(stdin); 

    ScopedTimer t;
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