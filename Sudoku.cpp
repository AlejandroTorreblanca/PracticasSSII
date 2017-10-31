/* ------------------------- PROBLEMA DEL SUDOKU ----------------------- */
#include <ga/GASimpleGA.h> //  Algoritmo Genetico simple
#include <ga/GA1DArrayGenome.h> // Genoma --> array de enteros (dim. 1) alelos
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;



struct plantilla{
       int tam;
       int *fijo;
};


float Objective(GAGenome &); // Funcion objetivo --> al final

GABoolean Termina(GAGeneticAlgorithm &); // Funcion de terminacion --> al final

void InicioSudoku(GAGenome& g){

     //*******************************************************//
     //*******************************************************//
     // Hacer el casting correspondiente para obtener genome  //
     //*******************************************************//
     //*******************************************************//

     GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;  //Casting

     struct plantilla * plantilla1;
     plantilla1 = (struct plantilla *) genome.userData();

     int aux[plantilla1->tam];

     for(int f=0;f<plantilla1->tam;f++){

          for(int j=0;j<plantilla1->tam;j++) aux[j]=0;

          for(int j=1;j<=plantilla1->tam;j++){
            int v=GARandomInt(0,plantilla1->tam-1);
            while (aux[v]!=0) v=(v+1)%plantilla1->tam;
            aux[v]=j;
          }

          int i=0;

          while(i<plantilla1->tam){

              while((plantilla1->fijo[(f*plantilla1->tam)+i]==0) && (i<plantilla1->tam)) i++;

              if (i<plantilla1->tam){

                     bool encontrado=false;
                     for(int j=0;(j<plantilla1->tam) && (!encontrado);j++)
                             if (aux[j]==plantilla1->fijo[(f*plantilla1->tam)+i]) {
                                encontrado=true;
                                aux[j]=aux[i];
                             }

                     aux[i]=plantilla1->fijo[(f*plantilla1->tam)+i];
              }
              i++;

          }

          for(int c=0;c<plantilla1->tam;c++)
                  genome.gene((f*plantilla1->tam)+c,aux[c]);
     }
}

int CruceSudoku(const GAGenome& p1,const GAGenome & p2,GAGenome* c1,GAGenome* c2){

     //******************************************************//
     //******************************************************//
     // Hacer el casting correspondiente para obtener m y p  //
     //******************************************************//
     //******************************************************//

    GA1DArrayGenome<int> & m = (GA1DArrayGenome<int> &)p1;
    GA1DArrayGenome<int> & p = (GA1DArrayGenome<int> &)p2;

    struct plantilla * plantilla1 = (struct plantilla *) m.userData();
    int n=0;

    int punto1=GARandomInt(0,m.length());
    while ((punto1%plantilla1->tam)!=0) punto1++;
    int punto2=m.length()-punto1;

    if (c1){
     	      //***************************************************//
              //***************************************************//
              // Hacer el casting correspondiente para obtener h1  //
              //***************************************************//
              //***************************************************//
             GA1DArrayGenome<int> & h1 = (GA1DArrayGenome<int> &)*c1;

             h1.copy(m,0,0,punto1);
             h1.copy(p,punto1,punto1,punto2);
             n++;
    }

    if (c2){
              //***************************************************//
              //***************************************************//
              // Hacer el casting correspondiente para obtener h2  //
              //***************************************************//
              //***************************************************//
             GA1DArrayGenome<int> & h2 = (GA1DArrayGenome<int> &)*c2;

             h2.copy(p,0,0,punto1);
             h2.copy(m,punto1,punto1,punto2);
             n++;
    }

    return n;

}

void leerSudoku(struct plantilla *S,char *nombreF){
   ifstream f(nombreF);

   f>>S->tam;

   S->fijo = new int[S->tam*S->tam];

   for(int i=0;i<S->tam*S->tam;i++)
           f>>S->fijo[i];

   f.close();
}

bool checkColumna(int col[], int * check, int tam){
     bool repe=false;

     for(int i=0;i<tam;i++) check[i]=0;

     for(int i=0;i<tam;i++)
             check[col[i]-1]++;
     for(int i=0;i<tam;i++) if (check[i]>1) repe=true;

     return repe;
}


int MutacionSudoku(GAGenome& g,float pmut){

     //*******************************************************//
     //*******************************************************//
     // Hacer el casting correspondiente para obtener genome  //
     //*******************************************************//
     //*******************************************************//
    GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;

    struct plantilla * plantilla1;
    plantilla1 = (struct plantilla *) genome.userData();
    int nmut=0;
    int aux;
    int fil;
    bool fila;

    int caux[plantilla1->tam];
    int *checkC=new int[plantilla1->tam];

    if (pmut<=0.0) return 0;

    for(int f=0; f<plantilla1->tam; f++)
       for(int c=0; c<plantilla1->tam; c++)
          if (plantilla1->fijo[(f*plantilla1->tam)+c]==0){
           if (GAFlipCoin(pmut) ){
                if (GAFlipCoin(0.5)) fila = true;
                else fila = false;

                if (!fila){

                      for(int j=0;j<plantilla1->tam;j++) caux[j]=genome.gene((j*plantilla1->tam)+c);
                      if (checkColumna(caux,checkC,plantilla1->tam)){
                         int v1 = GARandomInt(0,plantilla1->tam-1);
                         while (checkC[v1]<=1) v1=(v1+1)%plantilla1->tam;
                         v1++;
                         int v2 = GARandomInt(0,plantilla1->tam-1);
                         while (checkC[v2]!=0) v2=(v2+1)%plantilla1->tam;
                         v2++;

                         bool encontrado = false;
                         for(int j=0;j<plantilla1->tam && !encontrado;j++)
                                 if ((plantilla1->fijo[j*(plantilla1->tam)+c]==0)&&(genome.gene(j*(plantilla1->tam)+c)==v1)){
                                    encontrado = true;
                                    genome.gene((j*plantilla1->tam)+c,v2);
                                    fil = j;
                                 }

                         int col=(c+1)%plantilla1->tam;
                         while(genome.gene((fil*plantilla1->tam)+col)!=v2) col=(col+1)%plantilla1->tam;
                         if (plantilla1->fijo[(fil*plantilla1->tam)+col]==0) {
                                nmut++;
                                genome.gene((fil*plantilla1->tam)+col,v1);
                         }
                         else {
                              genome.gene((fil*plantilla1->tam)+c,v1);
                         }

                      }

                }
                else{
                   int v1 = (c + 1) %plantilla1->tam;
                   while ((plantilla1->fijo[(f*plantilla1->tam)+v1]!=0)) v1=(v1+1)%plantilla1->tam;
                   aux = genome.gene((f*plantilla1->tam)+c);
                   genome.gene((f*plantilla1->tam)+c,genome.gene((f*plantilla1->tam)+v1));
                   genome.gene((f*plantilla1->tam)+v1,aux);
                   nmut++;
                }
           }
          }

    return nmut;
}

int main(int argc, char **argv) //var: sudokuSize, poblacionsize, operador, pCruces, pmutacion, nombrePlantilla
{


    int nsudoku=atoi(argv[1]);  //Tamaño del sudoku
    cout << "Problema del sudoku de tamaño: " << nsudoku << " \n\n";


// Declaramos variables para los parametros del GA y las inicializamos

    int poblacionsize = atoi(argv[2]);
    int ngen = atoi(argv[3]);
    float pcruce = atof(argv[4]);
    float pmut = atof(argv[5]);
    char * file = argv[6];

    cout << "Parametros:  \n  - Poblacion size: " << poblacionsize << endl;
    cout << "               - Numero de generaciones: " << ngen << endl;
    cout << "               - Probabilidad cruce: " << pcruce << endl;
    cout << "               - Probabilidad mutacion: " << pmut << endl ;
    cout << "               - Nombre de la plantilla: " << file << endl << endl;

//Leemos el fichero de la plantilla

    plantilla plantillaInicial;
    leerSudoku(&plantillaInicial,file);

// Conjunto enumerado de alelos --> valores posibles de cada gen del genoma

    GAAlleleSet<int> alelos;
    for(int i=1;i<=nsudoku;i++) alelos.add(i);      //Los alelos toman los valores entre 1-9

// Creamos el genoma y definimos operadores de inicio, cruce y mutación

    GA1DArrayAlleleGenome<int> genome(nsudoku,alelos,Objective,&plantillaInicial);   //cambiamos variables
    genome.crossover(GA1DArrayAlleleGenome<int>::OnePointCrossover);
    genome.mutator(GA1DArrayAlleleGenome<int>::FlipMutator);

// Creamos el algoritmo genetico

    GASimpleGA ga(genome);

// Inicializamos - minimizar funcion objetivo, tamaño poblacion, nº generaciones,
// pr. cruce y pr. mutacion, selección y le indicamos que evolucione.

    ga.minimaxi(-1);
    ga.populationSize(poblacionsize);
    ga.nGenerations(ngen);
    ga.pCrossover(pcruce);
    ga.pMutation(pmut);
    GARouletteWheelSelector selector;
    ga.selector(selector);
    ga.terminator(Termina);
    ga.evolve(1);

// Imprimimos el mejor individuo que encuentra el GA y su valor fitness

    cout << "El GA encuentra la solucion ( " << ga.statistics().bestIndividual() << ")" << endl;
    cout << "con valor fitness " << ga.statistics().minEver() << endl;
}

// Funcion objetivo.

float Objective(GAGenome& g) {
    GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;
    int SIZE = 9;
    int F[SIZE];
    int C[SIZE];
    int R[3][SIZE];
    float valor=0;
    int aux=0;
    for(int i=0; i<genome.length(); i++)
    {
        //Comprobamos filas y columnas
       for(int j=0;j<SIZE;j++)
       {
           F[j]=genome.gene(i);
           C[j]=genome.gene(i+j*9);
           if(j<3)
           {
               R[0][j]=genome.gene(i);
           }
           else if(j>=3 && j<6)
                R[1][j]=genome.gene(i);
           else R[2][j]=genome.gene(i);

       }
        aux++;
        if(aux==3)
        {
            sort(R[0], R[0] + SIZE);
            sort(R[1], R[1] + SIZE);
            sort(R[2], R[2] + SIZE);
            for(int k=0;k++;k<SIZE)
            {
                if(R[0][k]!=k+1)
                     valor++;
                if(R[1][k]!=k+1)
                     valor++;
                if(R[2][k]!=k+1)
                     valor++;
            }
            aux=0;
        }
        sort(C, C + SIZE);
        sort(F, F + SIZE);

        for(int k=0;k++;k<SIZE)
        {
            if(C[k]!=k+1)
                 valor++;
            if(F[k]!=k+1)
                 valor++;
        }
    }








    float jaques=0;
    int c,f;
    // jaques de misma fila
    for(int i=0; i<genome.length(); i++)
       for(int j=i+1;j<genome.length();j++)
            if (genome.gene(i)==genome.gene(j)) jaques++;

    // jaques en diagonal
    for(int en_est=0;en_est<genome.length();en_est++){

	  // diagonal derecha abajo
	  c=en_est+1;
	  f=genome.gene(en_est)+1;
	  while ((c<genome.length()) && (f<genome.length())){
	  	  if (genome.gene(c)==f) jaques++;
	  	  c++;
	  	  f++;
	  }

	  // diagonal derecha arriba
	  c=en_est+1;
	  f=genome.gene(en_est)-1;
	  while ((c<genome.length()) && (f>=0)){
	  	 if (genome.gene(c)==f) jaques++;
	  	  c++;
	  	  f--;
	  }
    }

    return jaques;
}

// Funcion de terminacion

GABoolean Termina(GAGeneticAlgorithm & ga){
    if ( (ga.statistics().minEver()==0) ||
        (ga.statistics().generation()==120000) ) return gaTrue;
    else return gaFalse;
}
