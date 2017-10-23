/* ------------------------- PROBLEMA DE LAS N REINAS ----------------------- */
#include <ga/GASimpleGA.h> //  Algoritmo Genetico simple
#include <ga/GA1DArrayGenome.h> // Genoma --> array de enteros (dim. 1) alelos
#include <iostream>
#include <fstream>
using namespace std;

float Objective(GAGenome &); // Funcion objetivo --> al final

GABoolean Termina(GAGeneticAlgorithm &); // Funcion de terminacion --> al final

int main(int argc, char **argv)
{
    int nreinas=atoi(argv[1]);
    cout << "Problema de las " << nreinas << " reinas \n\n";


// Declaramos variables para los parametros del GA y las inicializamos

    int popsize = atoi(argv[2]);
    int ngen = atoi(argv[3]);
    float pcross = atof(argv[4]);
    float pmut = atof(argv[5]);

    cout << "Par\240metros:    - Tama\xA4o poblaci\242n: " << popsize << endl;
    cout << "               - N\243mero de generaciones: " << ngen << endl;
    cout << "               - Probabilidad cruce: " << pcross << endl;
    cout << "               - Probabilidad mutaci\242n: " << pmut << endl << endl;


// Conjunto enumerado de alelos --> valores posibles de cada gen del genoma

    GAAlleleSet<int> alelos;
    for(int i=0;i<nreinas;i++) alelos.add(i);

// Creamos el genoma y definimos operadores de inicio, cruce y mutación

    GA1DArrayAlleleGenome<int> genome(nreinas,alelos,Objective,NULL);
    genome.crossover(GA1DArrayAlleleGenome<int>::OnePointCrossover);
    genome.mutator(GA1DArrayAlleleGenome<int>::FlipMutator);

// Creamos el algoritmo genetico

    GASimpleGA ga(genome);

// Inicializamos - minimizar funcion objetivo, tamaño poblacion, nº generaciones,
// pr. cruce y pr. mutacion, selección y le indicamos que evolucione.

    ga.minimaxi(-1);
    ga.populationSize(popsize);
    ga.nGenerations(ngen);
    ga.pCrossover(pcross);
    ga.pMutation(pmut);
    GARouletteWheelSelector selector;
    ga.selector(selector);
    ga.terminator(Termina);
    ga.evolve(1);

// Imprimimos el mejor individuo que encuentra el GA y su valor fitness

    cout << "El GA encuentra la soluci\242n ( " << ga.statistics().bestIndividual() << ")" << endl;
    cout << "con valor fitness " << ga.statistics().minEver() << endl;
}

// Funcion objetivo.

float Objective(GAGenome& g) {
    GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;
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
        (ga.statistics().generation()==ga.nGenerations()) ) return gaTrue;
    else return gaFalse;
}
