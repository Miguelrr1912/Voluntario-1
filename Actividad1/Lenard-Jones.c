#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>


#define L 10
#define N 16
#define h 0.002
#define PI 3.14159265358979323846

typedef struct {
    double x, y;
    double vx, vy;
    double ax, ay; 
    double theta;
} body;

void InializarCuerpos(body bodies[])
{    int i,j; 
    srand(time(NULL)); // Inicializa la semilla para la generación de números aleatorios
     for(i=0; i<N; i++)
     { 

       bodies[i].x=(double)(rand())/RAND_MAX*L;
       bodies[i].y=(double)(rand())/RAND_MAX*L;
       
       // Me aseguro que esten espaciados de forma que su radio sea mayor que 1
       for(j=0; j<i; j++)
       {   double dx=bodies[i].x-bodies[j].x;
           double dy=bodies[i].y-bodies[j].y;

           //Pongo las condiciones de contorno
           if(dx > L/2) dx -= L;
           if(dx < -L/2) dx += L;
           if(dy > L/2) dy -= L;
           if(dy < -L/2) dy += L;

           double r=sqrt(dx*dx+dy*dy);
           if(r<1.1)
           {   bodies[i].x=(double)(rand())/RAND_MAX*L;
               bodies[i].y=(double)(rand())/RAND_MAX*L;
               j=-1; // Reinicia el bucle para verificar la nueva posición
           }
       }
       
       bodies[i].theta=(double)(rand())/RAND_MAX*2*PI;
       bodies[i].vx=4*cos(bodies[i].theta);
       bodies[i].vy=4*sin(bodies[i].theta);
       //bodies[i].vy=0; // Inicializo la velocidad en y a 0 para simplificar el problema
       bodies[i].ax=0;
       bodies[i].ay=0;
    }
}

void Energia  (body bodies[], FILE *file)
{  
    int i,j; 

    double E=0;
    double Ecinetica=0;
    double Epotencial=0;
    
    //Para evitar conflictos con OpenMP
    #pragma omp parallel for reduction(+:Ecinetica)
    for(i=0; i<N; i++)
    {   
        Ecinetica+=0.5*(bodies[i].vx*bodies[i].vx+bodies[i].vy*bodies[i].vy);
        for(j=i+1; j<N; j++)
        {
            double dx=bodies[i].x-bodies[j].x;
            double dy=bodies[i].y-bodies[j].y;

            //Pongo las condiciones de contorno
            if(dx > L/2) dx -= L;
            if(dx < -L/2) dx += L;
            if(dy > L/2) dy -= L;
            if(dy < -L/2) dy += L;

            double r=sqrt(dx*dx+dy*dy);

               Epotencial+=4*(pow(r,-12)-pow(r,-6));
            
        }
    }
    E=Ecinetica+Epotencial;
    // Guardar la energía en el archivo
    fprintf(file, "%lf, %lf, %lf\n", Epotencial, Ecinetica, E);
}


void aceleracion (body bodies[])
{
    int i,j; //Variables de control
  double dx,dy,r;
  
  //calculo la aceleracion a partir de las fuerzas
  //Paralelizo
  #pragma omp parallel for private(j,dx,dy,r) shared(bodies)
  for (i=0; i<N; i++)
    { 
        bodies[i].ax=0;
        bodies[i].ay=0;

        for(j=0; j<N; j++)
        { if (i!=j) 
            {
            dx=bodies[i].x-bodies[j].x;
            dy=bodies[i].y-bodies[j].y;

            //Pongo las condiciones de contorno
            if(dx > L/2) dx -= L;
            if(dx < -L/2) dx += L;
            if(dy > L/2) dy -= L;
            if(dy < -L/2) dy += L;

            r=sqrt(dx*dx+dy*dy);
            
            if(r<2.5)
            {bodies[i].ax+=24*(+2*pow(r,-14)-pow(r,-8))*dx;
            bodies[i].ay+=24*(+2*pow(r,-14)-pow(r,-8))*dy; 
            }   
            }
        }
     }
}    
void Verlet (body bodies[], FILE *file)
{   
    int i;
    double acelant[N][2];
    double omega[N][2];
    

    //Actualizo la posición y calculo omega
    for(i=0; i<N; i++)
    {
        bodies[i].x+=bodies[i].vx*h+0.5*bodies[i].ax*h*h;
        bodies[i].y+=bodies[i].vy*h+0.5*bodies[i].ay*h*h;
        omega[i][0]=bodies[i].vx + h/2*bodies[i].ax;
        omega[i][1]=bodies[i].vy + h/2*bodies[i].ay; 
    }

    // Cambio las posiciones para que cumpla las condiciones de contorno
    for(i=0; i<N; i++)
    {   if(bodies[i].x>L)
        {   bodies[i].x=bodies[i].x-L;
        }
        if(bodies[i].x<0)
        {   bodies[i].x=bodies[i].x+L;
        }
        if(bodies[i].y>L)
        {   bodies[i].y=bodies[i].y-L;
        }
        if(bodies[i].y<0)
        {   bodies[i].y=bodies[i].y+L;
        }     
    }
    
    //Actualizo las aceleraciones
    aceleracion(bodies);

    //Ahora calculo las velocidades
    for(i=0; i<N; i++)
    {
        bodies[i].vx=omega[i][0]+h/2*bodies[i].ax;
        bodies[i].vy=omega[i][1]+h/2*bodies[i].ay;
    }

    //Para librarme de hacer mas funciones me los voy a guardar ya todas las variables 
    //en un solo archivo

    for(i=0; i<N; i++)
    {
        fprintf(file, "%lf, %lf\n", bodies[i].x, bodies[i].y);
    }
    fprintf(file, "\n"); //Salto de línea para separar los pasos
}

double Promedio(body bodies[], FILE *file)
{   
    int i;
    double promediotot=0, suma=0;
    for (i=0; i<N; i++)
    {   // Calculo el promedio de la energia cinetica
        suma+=0.5*(bodies[i].vx*bodies[i].vx+bodies[i].vy*bodies[i].vy);
    }
    //fprintf(file, "%lf\n", promediotot);
    return suma/N;
}

double velocidad(body bodies[])
{   
    int i;
    double vel=0;
    for (i=0; i<N; i++)
    {   // Calculo el promedio de la energia cinetica
        vel=sqrt(bodies[i].vx*bodies[i].vx+bodies[i].vy*bodies[i].vy);
        return vel;
    }
  
}

double Momento(body bodies[], double antx[], double anty[]) {
    int j;
    double momento = 0;

    for (j = 0; j < N; j++) {
        // Cruce en X
        if (antx[j] > L - 1 && bodies[j].x < 1) {
            momento += 2 * fabs(bodies[j].vx); // de L a 0
        } else if (antx[j] < 1 && bodies[j].x > L - 1) {
            momento += 2 * fabs(bodies[j].vx); // de 0 a L
        }

        // Cruce en Y
        if (anty[j] > L - 1 && bodies[j].y < 1) {
            momento += 2 * fabs(bodies[j].vy);
        } else if (anty[j] < 1 && bodies[j].y > L - 1) {
            momento += 2 * fabs(bodies[j].vy);
        }
    }

    return momento;
}



int main (void)
{
    double i,aux, antx[N], anty[N], pprom;
    int j, k,contador=0;
    double promedio=0,promediotot=0;
    //Para calculo de errores
    double T[100000], P[100000], varianzat=0, varianzap=0;

    FILE *file = fopen("SALIDA.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    FILE *file2 = fopen("ENERGIA.txt", "w");
    if (file2 == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    FILE *file3 = fopen("Temperatura.txt", "w");
    if (file3 == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    FILE *file4 = fopen("Presión.txt", "w");
    if (file4 == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    body bodies[N];
    InializarCuerpos(bodies);
    aceleracion(bodies);

    //Guardo las velocidades individuales en t=0 de cada particula para ver que se ajuste a boltzmann
    fprintf(file3, "%lf\n", velocidad(bodies));

    //Para el calculo de la presión, guardo las posiciones de las particulas en el tiempo t=0
    for(j=0; j<N; j++)
        {
            antx[j]=bodies[j].x;
            anty[j]=bodies[j].y;
        }

    //Bucle principal 
    for (i=0; i<100; i=i+h)
    {
        Verlet(bodies, file);
        Energia(bodies, file2);

        // Calculo la temperatura, la velocidad y la presión en un intervalor determinado
        if(i>20 && i<50)
        {  
           promedio+=Promedio(bodies, file2);
           //Para el calculo de errores
           T[contador]=Promedio(bodies, file2);
          
           fprintf(file3, "%lf\n", velocidad(bodies));

           //Calculo la presión con la definición de momento para cada particula
           aux=Momento(bodies, antx, anty);
           pprom+=aux;



           //Calcula la presión para este paso 
            fprintf(file4, "%lf\n", aux/(L*L*h)); 
            //Para el calculo de errores
            P[contador] = aux/(L*L*h);
            contador++;
                
        }

        for(j=0; j<N; j++)
        {
            // Actualizo las posiciones de las particulas para el siguiente paso
            antx[j]=bodies[j].x;
            anty[j]=bodies[j].y;
        }
     


    }

    // Calculo el promedio de la energia cinetica
    promedio=promedio/(contador);
    printf("El promedio de la energia cinetica es: %lf\n", promedio);
    //Imprimo el promedio de la temperatura y presión
    fprintf(file3, "%lf\n", promedio);
    fprintf(file4, "%lf\n", pprom/(contador*L*L*h));

    //Calculo los errorres
    for (j=0; j<contador; j++)
    {
        varianzat += (T[j] - promedio) * (T[j] - promedio);
        varianzap += (P[j] - pprom/(contador*L*L*h)) * (P[j] - pprom/(contador*L*L*h));
    }
   
    // Divido por el número de elementos para obtener la varianza
    varianzat /= (contador-1);
    varianzap /= (contador-1);

    // Añado el error al archivo de temperatura y presión
    fprintf(file3, "Error: %lf\n", sqrt(varianzat)/ sqrt(contador));
    fprintf(file4, "Error: %lf\n", sqrt(varianzap)/ sqrt(contador));
    
    fclose(file);
    fclose(file2);
    fclose(file3);
    fclose(file4);
}