/* ------------------------- PROBLEMA DEL SUDOKU ----------------------- */
#include <ga/GASimpleGA.h> //  Algoritmo Genetico simple
#include <ga/GA1DArrayGenome.h> // Genoma --> array de enteros (dim. 1) alelos
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;
int const GENERACIONES=12000;   //
int const TAMSUDOKU=81;

struct plantilla{
       int tam;
       int *fijo;
};


float Objective(GAGenome &); // Funcion objetivo --> al final

GABoolean termina(GAGeneticAlgorithm &); // Funcion de terminacion --> al final

void imprimeSudoku(int tamSudoku, GAGeneticAlgorithm& ga)
{
     GA1DArrayGenome<int> & sudoku = (GA1DArrayGenome<int> &) ga.statistics().bestIndividual();
     for(int i=0;i<sudoku.length();i++)
     {
         if(i%tamSudoku==0) cout<<endl;
         cout << sudoku.gene(i)<< " ";


     }
     cout<<endl;
}

void InicioSudoku(GAGenome& g){

     GA1DArrayGenome<int> & genome = (GA1DArrayGenome<int> &)g; //Hacemos el correspondiente casting

     struct plantilla * plantilla1;
     plantilla1 = (struct plantilla *) genome.userData();   //Se obtiene la plantilla inicial

     int aux[plantilla1->tam];

     for(int f=0;f<plantilla1->tam;f++){                //Para cada fila

          for(int j=0;j<plantilla1->tam;j++) aux[j]=0;  //Inicializamos la fila con valores 0

          for(int j=1;j<=plantilla1->tam;j++){          //Para cada columna
            int v=GARandomInt(0,plantilla1->tam-1);     //Tomamos un valor aleatorio
            while (aux[v]!=0) v=(v+1)%plantilla1->tam;  //Buscamos la posición v que tendrá un 0
            aux[v]=j;                                   //Le asignamos un valor entre 0 y 9
          }
          int i=0;

          while(i<plantilla1->tam){

              while((plantilla1->fijo[(f*plantilla1->tam)+i]==0) && (i<plantilla1->tam)) i++; //Buscamos una posición de la plantilla que no contenga un 0

              if (i<plantilla1->tam){
                //Buscamos los valores fijos de la plantilla y los introducimos en el array, los valores sustituidos se ponen en otra posición que
                //contuviera el valor de la plantilla

                     bool encontrado=false;
                     for(int j=0;(j<plantilla1->tam) && (!encontrado);j++)
                             if (aux[j]==plantilla1->fijo[(f*plantilla1->tam)+i]) {
                                encontrado=true;
                                aux[j]=aux[i];
                             }

                     aux[i]=plantilla1->fijo[(f*plantilla1->tam)+i];    //Le asignamos el valor de la plantilla
              }
              i++;

          }

          for(int c=0;c<plantilla1->tam;c++)
                  genome.gene((f*plantilla1->tam)+c,aux[c]);    //Le asignamos la cadena al individuo
     }
}


int CruceSudoku(const GAGenome& p1,const GAGenome & p2,GAGenome* c1,GAGenome* c2){

    GA1DArrayGenome<int> & m = (GA1DArrayGenome<int> &)p1;      //Casting del los padres
    GA1DArrayGenome<int> & p = (GA1DArrayGenome<int> &)p2;

    struct plantilla * plantilla1 = (struct plantilla *) m.userData();  //Obtenemos la plantilla inicial
    int n=0;
    int punto1=GARandomInt(0,m.length());           //Tomamos un valor aleatorio entre 0 y 80 que nos servira como posicion para dividir la cadena
    while ((punto1%plantilla1->tam)!=0) punto1++;   //Avanzamos hasta encontrar en la plantilla una posicion con un 0
    int punto2=m.length()-punto1;                   //Calculamos el resto de posiciones de la cadena

    if (c1){
             GA1DArrayGenome<int> & h1 = (GA1DArrayGenome<int> &)*c1;   //Casting del primer hijo
             h1.copy(m,0,0,punto1);                                     //Copiamos la cadena del padre de la posicion 0 a punto1
             h1.copy(p,punto1,punto1,punto2);                           //Copiamos la cadena de la madre de la posicion punto1 a punto2
             n++;
    }

    if (c2){
             GA1DArrayGenome<int> & h2 = (GA1DArrayGenome<int> &)*c2;   //Casting del segundo hijo
             h2.copy(p,0,0,punto1);                                     //Copiamos la cadena del madre de la posicion 0 a punto1
             h2.copy(m,punto1,punto1,punto2);                           //Copiamos la cadena de la padre de la posicion punto1 a punto
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

/** Retorna true si el array col[] contiene algun valor repetido  **/
bool checkColumna(int col[], int * check, int tam){
     bool repe=false;

     for(int i=0;i<tam;i++) check[i]=0;

     for(int i=0;i<tam;i++)
             check[col[i]-1]++;
     for(int i=0;i<tam;i++) if (check[i]>1) repe=true;

     return repe;
}


int MutacionSudoku(GAGenome& g,float pmut){

    GA1DArrayGenome<int> & genome = (GA1DArrayGenome<int> &)g;      //Casting del individuo

    struct plantilla * plantilla1;
    plantilla1 = (struct plantilla *) genome.userData();            //Recuperamos la plantilla inicial
    int nmut=0;
    int aux;
    int fil;
    bool fila;

    int caux[plantilla1->tam];
    int *checkC=new int[plantilla1->tam];

    if (pmut<=0.0) return 0;        //Si la probabilidad de mutacion es 0 o menor no hacemos nada

    for(int f=0; f<plantilla1->tam; f++)        //Para cada fila
       for(int c=0; c<plantilla1->tam; c++)     //Para cada columna
          if (plantilla1->fijo[(f*plantilla1->tam)+c]==0){  //Si el  la casilla de la plantilla contiene un 0
           if (GAFlipCoin(pmut) ){              //Simulamos un lanzamiento de moneda con probabilidad pmut de que salga true
                if (GAFlipCoin(0.5)) fila = true;   //Tenemos la mitad de posibilidades de hacer una mutación por fila o por columna
                else fila = false;

                if (!fila){

                      for(int j=0;j<plantilla1->tam;j++) caux[j]=genome.gene((j*plantilla1->tam)+c);    //Copiamos la columna c en caux[]
                      if (checkColumna(caux,checkC,plantilla1->tam)){       //Si existe algun numero repetido en la columna c
                         int v1 = GARandomInt(0,plantilla1->tam-1);         //Tomamos un valor aleatorio entre 0 y 8

                         while (checkC[v1]<=1) v1=(v1+1)%plantilla1->tam;   //Buscamos el numero repetido
                         v1++;
                         int v2 = GARandomInt(0,plantilla1->tam-1);
                         while (checkC[v2]!=0) v2=(v2+1)%plantilla1->tam;   //Buscamos el numero que falta en la columna
                         v2++;

                         bool encontrado = false;
                         for(int j=0;j<plantilla1->tam && !encontrado;j++)  //Mutamos el elemento repetido
                                 if ((plantilla1->fijo[j*(plantilla1->tam)+c]==0)&&(genome.gene(j*(plantilla1->tam)+c)==v1)){
                                    encontrado = true;
                                    genome.gene((j*plantilla1->tam)+c,v2);
                                    fil = j;
                                 }

                         int col=(c+1)%plantilla1->tam;
                         while(genome.gene((fil*plantilla1->tam)+col)!=v2) col=(col+1)%plantilla1->tam; //Buscamos en la columna siguiente el elemento que no se encontraba en la columna
                         if (plantilla1->fijo[(fil*plantilla1->tam)+col]==0) {  //Lo cambiamos por v1 si en la plantilla era un 0
                                nmut++;
                                genome.gene((fil*plantilla1->tam)+col,v1);
                         }
                         else {
                              genome.gene((fil*plantilla1->tam)+c,v1);
                         }

                      }

                }
                else{
                   int v1 = (c + 1) %plantilla1->tam;                                               //Tomamos un valor de 0 a 9
                   while ((plantilla1->fijo[(f*plantilla1->tam)+v1]!=0)) v1=(v1+1)%plantilla1->tam; //Buscamos una posición de la plantilla con valor 0
                   aux = genome.gene((f*plantilla1->tam)+c);                                        //calcula el valor del gen en la poscion c
                   genome.gene((f*plantilla1->tam)+c,genome.gene((f*plantilla1->tam)+v1));          //Establece el valor del gen c
                   genome.gene((f*plantilla1->tam)+v1,aux);                                         //Establece el valor del gen v1
                   nmut++;                                                                          //Se estan intercambiando los valores de los genes v1 y c
                }
           }
          }

    return nmut;
}


float Objective(GAGenome& g) {

    GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;
    int SIZE = 9;
    int F[SIZE];
    int C[SIZE];
    int R[3][SIZE];
    memset(C, 0, sizeof(C));    //Inicializamos arrays a 0
    memset(F, 0, sizeof(F));
    memset(R, 0, sizeof(R));
    float valor=0;
    int aux=0;
    int fila=1;
    for(int i=0; i<genome.length(); i++)
    {
        aux++;
        F[genome.gene(i)-1]++;      //Tomamos los valores de las filas
        if(F[genome.gene(i)-1]>=2)  //Si se repite un numero aumentamos el fitness
            valor++;

        int f=genome.gene((i%9)*9+(fila-1));
        C[f-1]++;        //Tomamos los valores de las columnas
        if(C[f-1]>=2)    //Si se repite un numero aumentamos el fitness
            valor++;
        int n=0;

        //Tomamos los valores de los cuadrados
        if(i%9<3){   //Tres primeras casillas
            R[0][genome.gene(i)-1]++;
            n=0;
        }
        else if(i%9<6 && i%9>=3){    //Tres segundas casillas
            R[1][genome.gene(i)-1]++;
            n=1;
        }
        else {       //Tres ultimas casillas
            R[2][genome.gene(i)-1]++;
            n=2;
        }
        if(R[n][genome.gene(i)-1]>=2)   //Si se repite un numero aumentamos el fitness
            valor++;

        if(aux%9==0)    //Si hemos terminado de recorrer la columna y la fila reseteamos arrays de columna y fila
        {
            memset(C, 0, sizeof(C));
            memset(F, 0, sizeof(F));
            if(fila%3==0)   //Si hemos recorrido tres filas reseteamos arrays de bloques
            {
                memset(R, 0, sizeof(R));
            }
            fila++;
        }
    }
    return valor;
}


GABoolean termina(GAGeneticAlgorithm & ga){
    if ( (ga.statistics().minEver()==0) ||
        (ga.statistics().generation()==GENERACIONES) ) return gaTrue;   //Comprobamos que no hemos llegado a ninguna solucion y que las generaciones son menores de 12000
    else return gaFalse;
}


int main(int argc, char **argv) //var: sudokuSize, poblacionsize, operador, pCruces, pmutacion, nombrePlantilla
{
    if (argc != 6) {
             cout << "- error: "  << endl;
       cout << "Argumentos incorrectos" << endl;
       cout << "Forma de uso: ./Sudoku.exe  Poblacion Selector{ruleta,torneo} ProbCruce ProbMutacion fichero.txt" << endl;
       return -1;
    }



// Declaramos variables para los parametros del GA y las inicializamos
    int poblacionSize=atoi(argv[1]);
    string selector = argv[2];
    float pcruce = atof(argv[3]);
    float pmut = atof(argv[4]);
    char * file = argv[5];

    cout << selector <<" "<< poblacionSize<<" "<< pcruce <<" "<< pmut << endl ;

//Leemos el fichero de la plantilla

    plantilla plantillaInicial;
    leerSudoku(&plantillaInicial,file);

// Conjunto enumerado de alelos --> valores posibles de cada gen del genoma

    GAAlleleSet<int> alelos;
    for(int i=1;i<=TAMSUDOKU;i++) alelos.add(i);      //Los alelos toman los valores entre 1-9

// Creamos el genoma y definimos operadores de inicio, cruce y mutación

    GA1DArrayGenome<int> genome(TAMSUDOKU,Objective,&plantillaInicial);   //cambiamos variables

    genome.initializer(InicioSudoku);   //Establecemos el operador de inicializacion
    genome.crossover(CruceSudoku);      //Establecemos el operador de cruce
    genome.mutator(MutacionSudoku);     //Establecemos el operador de mutacion

// Creamos el algoritmo genetico

    GASimpleGA ga(genome);

// Inicializamos - minimizar funcion objetivo, tamaño poblacion, nº generaciones,
// pr. cruce y pr. mutacion, selección y le indicamos que evolucione.

    ga.minimaxi(-1);                    //Establecemos que el algoritmo genetico es de minimizar
    ga.populationSize(poblacionSize);   //Establecemos el tamaño de la poblacion

    ga.pCrossover(pcruce);              //Establecemos la probabilidad de cruce
    ga.pMutation(pmut);                 //Establecemos la probabilidad de mutacion
    ga.terminator(termina);             //Establecemos la funcion termina() como criterio de finalizacion

    if(selector=="ruleta")
    {
        GARouletteWheelSelector selec;
        ga.selector(selec);             //Establecemos el selector ruleta
    }
    else if(selector=="torneo")
    {
        GATournamentSelector selec;
        ga.selector(selec);             //Establecemos el selector torneo
    }
    else
    {
        cout << "Tipo de selector incorrecto, los valores permitidos son {ruleta,torneo}" << endl;
        return -1;
    }

    ga.evolve(1);                       //Indicamos que el algoritmo genetico debe empezar a evolucionar

// Imprimimos el mejor individuo que encuentra el GA y su valor fitness

    cout << "El GA encuentra la solucion: " << endl;
    imprimeSudoku(plantillaInicial.tam,ga);
    cout << " " << ga.statistics().minEver()<<" " <<ga.statistics().generation()<< endl;


}







