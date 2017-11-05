/* ------------------------- PROBLEMA DEL SUDOKU ----------------------- */
#include <ga/GASimpleGA.h> //  Algoritmo Genetico simple
#include <ga/GA1DArrayGenome.h> // Genoma --> array de enteros (dim. 1) alelos
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;
int const GENERACIONES=12000;
int const TAMSUDOKU=81;

struct plantilla{
       int tam;
       int *fijo;
};


float Objective(GAGenome &); // Funcion objetivo --> al final

GABoolean Termina(GAGeneticAlgorithm &); // Funcion de terminacion --> al final

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

     //*******************************************************//
     //*******************************************************//
     // Hacer el casting correspondiente para obtener genome  //
     //*******************************************************//
     //*******************************************************//

     //GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;  //Casting
     GA1DArrayGenome<int> & genome = (GA1DArrayGenome<int> &)g;

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
    GA1DArrayGenome<int> & genome = (GA1DArrayGenome<int> &)g;

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

void imprimeGenoma(GA1DArrayAlleleGenome<int> g)
{
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; i++)
            cout<<" " <<endl;
    }

}

float Objective(GAGenome& g) {
     cout<<"objective " <<endl;
    GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;
    int SIZE = 9;
    int F[SIZE];
    int C[SIZE];
    int R[3][SIZE];
    float valor=0;
    int aux=0;
    int fila=1;
    for(int i=0; i<genome.length(); i++)
    {
        aux++;
        F[i%9]=genome.gene(i);      //Tomamos los valores de las filas
        C[i%9]=genome.gene((i%9)*9+(fila-1));        //Tomamos los valores de las columnas
        //Tomamos los valores de los cuadrados
        if(fila%3==1)     //Si estamos en las tres primeras filas
        {
            if(i%9<3)   //Tres primeras casillas
                R[0][i%9]=genome.gene(i);
            else if(i%9<6 && i%9>=3)    //Tres segundas casillas
                R[1][i%9-3]=genome.gene(i);
            else        //Tres ultimas casillas
                R[2][i%9-6]=genome.gene(i);
        }
        else if(fila%3==2)  //Si estamos en las filas 4, 5 o 6
        {
            if(i%9<3)
                R[0][i%9+3]=genome.gene(i);
            else if(i%9<6 && i%9>=3)
                R[1][i%9]=genome.gene(i);
            else
                R[2][i%9-3]=genome.gene(i);
        }
        else        //Si estamos en las tres ultimas filas
        {
            if(i%9<3)
                R[0][i%9+6]=genome.gene(i);
            else if(i%9<6 && i%9>=3)
                R[1][i%9+3]=genome.gene(i);
            else
                R[2][i%9]=genome.gene(i);
        }

        if(aux%9==0)
        {
            if(fila%3==0)
            {
                sort(R[0], R[0] + SIZE);
                sort(R[1], R[1] + SIZE);
                sort(R[2], R[2] + SIZE);
                for(int k=0;k<SIZE;k++)
                {
                    if(R[0][k]!=k+1)
                         valor++;
                    if(R[1][k]!=k+1)
                         valor++;
                    if(R[2][k]!=k+1)
                         valor++;
                }
            }

            sort(F, F + SIZE);
            sort(C, C + SIZE);
            for(int k=0;k<SIZE;k++)
            {
                if(C[k]!=k+1)
                     valor++;
                if(F[k]!=k+1)
                     valor++;
            }
            fila++;
        }

//        if(aux%9==0)
//            cout<<"" <<endl;
//        else cout << genome.gene(i) <<" ";;


    }
    cout<<"valor= " << valor<<endl;
    return valor;
}


GABoolean Termina(GAGeneticAlgorithm & ga){
    if ( (ga.statistics().minEver()==0) ||
        (ga.statistics().generation()==GENERACIONES) ) return gaTrue;
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

    cout << "Parametros:  \n- Poblacion size: " << poblacionSize << endl;
    cout << "- Selector utilizado: " << selector << endl;
    cout << "- Probabilidad cruce: " << pcruce << endl;
    cout << "- Probabilidad mutacion: " << pmut << endl ;
    cout << "- Nombre de la plantilla: " << file << endl << endl;

//Leemos el fichero de la plantilla

    plantilla plantillaInicial;
    leerSudoku(&plantillaInicial,file);

// Conjunto enumerado de alelos --> valores posibles de cada gen del genoma

    GAAlleleSet<int> alelos;
    for(int i=1;i<=TAMSUDOKU;i++) alelos.add(i);      //Los alelos toman los valores entre 1-9

// Creamos el genoma y definimos operadores de inicio, cruce y mutación

    GA1DArrayGenome<int> genome(TAMSUDOKU,Objective,&plantillaInicial);   //cambiamos variables

    genome.initializer(InicioSudoku);
    genome.crossover(CruceSudoku);
    genome.mutator(MutacionSudoku);

// Creamos el algoritmo genetico

    GASimpleGA ga(genome);

// Inicializamos - minimizar funcion objetivo, tamaño poblacion, nº generaciones,
// pr. cruce y pr. mutacion, selección y le indicamos que evolucione.

    ga.minimaxi(-1);
    ga.populationSize(poblacionSize);

    ga.pCrossover(pcruce);
    ga.pMutation(pmut);
    ga.terminator(Termina);

    if(selector=="ruleta")
    {
        GARouletteWheelSelector selec;
        ga.selector(selec);
    }
    else if(selector=="torneo")
    {
        GATournamentSelector selec;
        ga.selector(selec);
    }
    else
    {
        cout << "Tipo de selector incorrecto, los valores permitidos son {ruleta,torneo}" << endl;
        return -1;
    }

    ga.evolve(1);

// Imprimimos el mejor individuo que encuentra el GA y su valor fitness

    cout << "El GA encuentra la solucion: " << endl;
    imprimeSudoku(plantillaInicial.tam,ga);
    cout << "El valor fitness es: " << ga.statistics().minEver() << endl;

}







