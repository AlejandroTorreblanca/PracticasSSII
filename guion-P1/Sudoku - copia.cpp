#include <ga/GASimpleGA.h> // Algoritmo Genetico simple
#include <ga/GA1DArrayGenome.h> //genoma-> array de enteros (dim. 1) alelos
#include <iostream>
#include <fstream>
using namespace std;
int const MAX_GENERACIONES=12000;

struct plantilla{
       int tam; // tamaño del sudoku
       int *fijo; // valores fijos iniciales    
};

// Lectura del sudoku inicial en S

void leerSudoku(struct plantilla *S,char *nombreF){
   ifstream f(nombreF);
   
   f>>S->tam;
   
   S->fijo = new int[S->tam*S->tam]; 
    
   for(int i=0;i<S->tam*S->tam;i++) 
           f>>S->fijo[i];   
   
   f.close();
}

// Inicia el genoma sin repetidos en filas y respetando fijos

void InicioSudoku(GAGenome& g){
     
     //Casting:
     GA1DArrayGenome<int> & genome = (GA1DArrayGenome<int> &)g;
     
     // Obtenemos la estructura auxiliar 
     struct plantilla * plantilla1;
     plantilla1 = (struct plantilla *) genome.userData();
     
     int aux[plantilla1->tam];
     
     for(int f=0;f<plantilla1->tam;f++){ // por cada fila
          
          // se inicializa a 0 la estructura auxiliar
          
          for(int j=0;j<plantilla1->tam;j++) aux[j]=0;
          
          // se inicializa la fila de 1 a tam sin repetidos
          
          for(int j=1;j<=plantilla1->tam;j++){
            int v=GARandomInt(0,plantilla1->tam-1);
            while (aux[v]!=0) v=(v+1)%plantilla1->tam;
            aux[v]=j;
          }
          
          // se colocan los fijos en su lugar
          
          int i=0;
          
          while(i<plantilla1->tam){
          
              while((plantilla1->fijo[(f*plantilla1->tam)+i]==0) && (i<plantilla1->tam)) i++; 
              
              if (i<plantilla1->tam){ // se encuentra un fijo en la plantilla
              
                     // se busca el fijo en aux
                     
                     bool encontrado=false;
                     for(int j=0;(j<plantilla1->tam) && (!encontrado);j++) 
                             if (aux[j]==plantilla1->fijo[(f*plantilla1->tam)+i]) {
                                encontrado=true;
                                aux[j]=aux[i];
                             }
                     // se pone el fijo en su sitio
                     
                     aux[i]=plantilla1->fijo[(f*plantilla1->tam)+i];             
              }
              i++;
          
          }
          
          // se copia la fila en el genoma
          
          for(int c=0;c<plantilla1->tam;c++)
                  genome.gene((f*plantilla1->tam)+c,aux[c]);   
     }        
}

// Cruce por un punto que coincide con final de una fila

int CruceSudoku(const GAGenome& p1,const GAGenome & p2,GAGenome* c1,GAGenome* c2){
    
     //Casting:
     GA1DArrayGenome<int> & m = (GA1DArrayGenome<int> &)p1;
     GA1DArrayGenome<int> & p = (GA1DArrayGenome<int> &)p2;
    
    struct plantilla * plantilla1 = (struct plantilla *) m.userData();
    int n=0;
    
    // buscamos un punto de cruce por filas
    
    int punto1=GARandomInt(0,m.length()); 
    while ((punto1%plantilla1->tam)!=0) punto1 = (punto1 + 1)%plantilla1->tam;
    int punto2=m.length()-punto1;
    
    // el hijo 1 hereda la primera parte del padre 1 y la segunda parte del
    // padre 2. 
    
    if (c1){
     	     //Casting: 
             GA1DArrayGenome<int> & h1 = (GA1DArrayGenome<int> &)*c1; 
             
             h1.copy(m,0,0,punto1);
             h1.copy(p,punto1,punto1,punto2);
             n++;
    }
    
    // el hijo 2 hereda la primera parte del padre 2 y la segunda parte del
    // padre 1. 
    
    if (c2){
             
             //Casting:
             GA1DArrayGenome<int> & h2 = (GA1DArrayGenome<int> &)*c2;  
             
             h2.copy(p,0,0,punto1);
             h2.copy(m,punto1,punto1,punto2);
             n++;
    }
    
    return n;
    
}

// Comprueba si una columna del sudoku tiene valores repetidos
// check contiene la cuenta de valores repetidos

bool checkColumna(int col[], int * check, int tam){
     bool repe=false;
     
     for(int i=0;i<tam;i++) check[i]=0;
     
     for(int i=0;i<tam;i++) 
             check[col[i]-1]++;
     for(int i=0;i<tam;i++) if (check[i]>1) repe=true;
     
     return repe;       
}

// Si un gen debe mutar elegimos si lo hace en fila o columna. En fila es intercambio
// en columna es mas heurístico: cambio un valor repetido por uno que no exista en la columna 

int MutacionSudoku(GAGenome& g,float pmut){
    
    //Casting:
    GA1DArrayGenome<int> & genome = (GA1DArrayGenome<int> &)g;
    
    // Obtenemos la estructura auxiliar 
    struct plantilla * plantilla1;
    plantilla1 = (struct plantilla *) genome.userData();
    int nmut=0;
    int aux,aux1;
    int fil;
    bool fila;
    
    int caux[plantilla1->tam];
    int *checkC=new int[plantilla1->tam];
    
    if (pmut<=0.0) return 0;
    
    for(int f=0; f<plantilla1->tam; f++)
       for(int c=0; c<plantilla1->tam; c++)
          if (plantilla1->fijo[(f*plantilla1->tam)+c]==0){ // si no es fijo
           if (GAFlipCoin(pmut) ){ // si toca mutar
                if (GAFlipCoin(0.5)) fila = true; // cambiamos fila
                else fila = false; // cambiamos columna   
                
                if (!fila){ // mutamos columna y arreglamos fila
                
                      for(int j=0;j<plantilla1->tam;j++) caux[j]=genome.gene((j*plantilla1->tam)+c); // obtenemos la columna
                      if (checkColumna(caux,checkC,plantilla1->tam)){ // si hay repetidos en la columna
                         int v1 = GARandomInt(0,plantilla1->tam-1); // v1 es valor repetido
                         while (checkC[v1]<=1) v1=(v1+1)%plantilla1->tam;  
                         v1++;
                         int v2 = GARandomInt(0,plantilla1->tam-1); // v2 es un valor que no existe
                         while (checkC[v2]!=0) v2=(v2+1)%plantilla1->tam; 
                         v2++;
                         //buscamos donde está v1 y se cambia por v2
                         
                         bool encontrado = false;
                         for(int j=0;j<plantilla1->tam && !encontrado;j++) 
                                 if ((plantilla1->fijo[j*(plantilla1->tam)+c]==0)&&(genome.gene(j*(plantilla1->tam)+c)==v1)){
                                    encontrado = true;
                                    genome.gene((j*plantilla1->tam)+c,v2);
                                    fil = j;  // v1 esta en fil                                                      
                                 }                                                     
                         //arreglamos la fila fil donde está v1: buscamos v2 y ponemos v1
                         
                         int col=(c+1)%plantilla1->tam;
                         while(genome.gene((fil*plantilla1->tam)+col)!=v2) col=(col+1)%plantilla1->tam;
                         if (plantilla1->fijo[(fil*plantilla1->tam)+col]==0) { // si v2 no es fijo en fil se lleva a cabo la mutación 
                                nmut++; 
                                genome.gene((fil*plantilla1->tam)+col,v1);                         
                         }
                         else { // si es fijo se deshace la mutacion
                              genome.gene((fil*plantilla1->tam)+c,v1);
                         }
                          
                      } // fin de si hay repetidos
                
                }
                else{ // mutamos fila: cambiamos f,c por f,v1
                   int v1 = (c + 1) %plantilla1->tam;
                   while ((plantilla1->fijo[(f*plantilla1->tam)+v1]!=0)) v1=(v1+1)%plantilla1->tam; //busco un no fijo
                   aux = genome.gene((f*plantilla1->tam)+c);
                   genome.gene((f*plantilla1->tam)+c,genome.gene((f*plantilla1->tam)+v1));
                   genome.gene((f*plantilla1->tam)+v1,aux);
                   nmut++;
                }               
           } // si toca mutar
          } // si no fijo
    
    return nmut;   
}

// Funcion objetivo
float Objective(GAGenome& g) {
      
      //Recuperamos la variable genérica en una más concreta
      GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;
      //Guardamos las variables que más usamos para ahorrar tiempo de ejecución
      struct plantilla * plantilla1;
      plantilla1 = (struct plantilla *) genome.userData();
      int longitud=plantilla1->tam;
      int superficie=genome.length();     
      float repeticiones=0;

      int repetidos[longitud];
      // repeticiones de la misma columna
      
      // repeticiones de la misma celda
      
      return repeticiones;
}

// Funcion de terminacion
GABoolean Termina(GAGeneticAlgorithm & ga){
          //Devuelve verdadero si y solo si se encuentra una solucion o se supera el numero de generaciones establecido
          if ( (ga.statistics().minEver()==0) || (ga.statistics().generation()==MAX_GENERACIONES) )
             return gaTrue;
          return gaFalse;
}

// Imprime la mejor solucion del Sudoku
void imprimeSudoku(plantilla sudo, GAGeneticAlgorithm & ga) {
    //Obtenemos el mejor sudoku 
    GA1DArrayGenome<int> & sudoku = (GA1DArrayGenome<int> &) ga.statistics().bestIndividual();
    //Recorremos su estructura imprimiendo sus genes
    for (int i=0; i < sudoku.length(); i++) {
        // Si se cumple la condicion, estamos en una nueva fila del sudoku. Imprimimos un salto de linea.
        if ( (i%sudo.tam) == 0 ) cout << endl;
        cout << sudoku.gene(i) << " ";
    }
        cout << endl;
}

int main(int argc, char **argv) {
    
    struct plantilla Sudoku;
    
    if (argc != 6) {
       cerr << "Argumentos incorrectos" << endl;
       cerr << "Forma de uso: ./Sudoku.exe fichero.txt Poblacion Selector ProbCruce ProbMutacion" << endl;
       return -1;
    }
    // Leemos los parámetros
    char* fichero = argv[1];
    int popsize = atoi(argv[2]);
    string elector = argv[3];
    float pcross = atof(argv[4]);
    float pmut = atof(argv[5]);

    // Leemos el fichero de los parametros de entrada
    leerSudoku(&Sudoku,fichero);
    
    // Creamos el algoritmo genetico e indicamos los operadores a utilizar
    GA1DArrayGenome<int> genome(Sudoku.tam*Sudoku.tam,Objective,&Sudoku);
    
    genome.initializer(InicioSudoku);
    genome.crossover(CruceSudoku);
    genome.mutator(MutacionSudoku);
    
    GASimpleGA ga(genome);
    
    if (elector == "Ruleta"){
       GARouletteWheelSelector selector;
       ga.selector(selector);
    }else if (elector == "Torneo"){
       GATournamentSelector selector;
       ga.selector(selector);
    }else {
       cerr << "Selector incorrecto" << endl;
       cerr << "Valores posibles: Ruleta y Torneo" << endl;
       return -2;
    }
    
    ga.terminator(Termina);
    ga.populationSize(popsize);
    ga.pCrossover(pcross);
    ga.pMutation(pmut);
    //Buscamos minimizar
    ga.minimaxi(-1);
    ga.evolve(1);
    
    cout << popsize << " " << elector << " "<< pcross << " " << pmut << " " << ga.statistics().minEver() << endl;
    
    imprimeSudoku(Sudoku, ga);
    
    return 0;
}
