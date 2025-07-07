#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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


void InializarCuerposCuadrado(body bodies[])
{    int i,j,aux=0; 
     for (i=0; i<N; i++)
     {bodies[i].x=L/8.0+L/4.0*(i%4);
      bodies[i].y=L/8.0+L/4.0*(aux%4);
      bodies[i].vx=0;
      bodies[i].vy=0;
      bodies[i].ax=0;
      bodies[i].ay=0;
      if(i%4==3)
      {aux++;}
     }
} 

void InializarCuerposAleatorio(body bodies[])
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
       bodies[i].vx=cos(bodies[i].theta);
       bodies[i].vy=sin(bodies[i].theta);
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

    //for(i=0; i<N; i++)
    //{
    //    fprintf(file, "%lf, %lf\n", bodies[i].x, bodies[i].y);
    //}
    //fprintf(file, "\n"); //Salto de línea para separar los pasos
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
        vel+=sqrt(bodies[i].vx*bodies[i].vx+bodies[i].vy*bodies[i].vy);   
    }
    return vel/N;
    
}

double fluctuación_particulas(body bodies[])
{   double dx, dy;
    // Calculo la fluctuación de la posición de las partículas 1 y 2
    dx= bodies[0].x - bodies[1].x;
    dy= bodies[0].y - bodies[1].y;

     // Aplicar condiciones de contorno
     if (dx > L/2) dx -= L;
     if (dx < -L/2) dx += L;
     if (dy > L/2) dy -= L;
     if (dy < -L/2) dy += L;
 
     //Calulo las fluctuaciones de la posición de la particula 1 y 2 con la temperatura para el tiempo t
     return dx*dx + dy*dy;
}


int main (void)
{
    double i, contador=0, fluc=0,temp=0;
    double promedio=0,promediotot=0;
    int j,aux;

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

    FILE *file4 = fopen("Fluctuación.txt", "w");
    if (file4 == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    body bodies[N];
    InializarCuerposCuadrado(bodies);
    aceleracion(bodies);

    for (i=0; i<1000; i=i+h)
    {   
        //Hago que cada 60 secs me de la flutuación media y la temperatura media 
        if ( aux%(30000) == 0 && aux != 0)
        {fprintf(file4, "%lf\n", fluc/30000);
        fprintf(file3, "%lf\n", temp/30000);

        //Ahora reinicio las fluctuaciones y la temperatura
        fluc=0;
        temp=0;
        }

        //Aumento la velocidad de los cuerpos en un factor de 1.1 añado un aux pq si no hace cosas raras
        if ( aux%(30000) == 0 && aux != 0)
        { for (j=0; j<N; j++)
          {bodies[j].vx=1.1*bodies[j].vx;
           bodies[j].vy=1.1*bodies[j].vy;
          }


        }

        Verlet(bodies, file);
        Energia(bodies, file2);
     

        fluc += fluctuación_particulas(bodies);
        

        //Imprimo la temperatura para cada iteración y la sumo
        temp+=Promedio(bodies, file3);
        
        
        aux++;
    }
    
    //Divido las fluctuaciones por el tiempo total
    fluc =fluc/(100/h);
    printf("La fluctuacion de la posicion es: %lf\n", fluc);

    promedio=promedio/(contador);
    printf("El promedio de la energia cinetica es: %lf\n", promedio);

    
    
    fclose(file);
    fclose(file2);
    fclose(file3);
}