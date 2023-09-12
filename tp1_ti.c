#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void lee_archivo(char*);

int main(int argc, char *argv[]) {
   
   char* extension_bin = ".bin";
   char *nombre_archivo=NULL;
   int n=0;


   for (int i = 1; i < argc; i++) {
      char *param = argv[i];
      if (strstr(param, extension_bin) != NULL)  //.bin
         nombre_archivo = param;

      else{ 
         n = atoi(param);
      }
   }

   lee_archivo(nombre_archivo);



}
void lee_archivo (char *nombre_archivo){


   unsigned int cant1s  = 0;
   unsigned int cant0s  = 0;
   unsigned int cant00s  = 0;
   unsigned int cant01s  = 0;
   unsigned int cant10s  = 0;
   unsigned int cant11s  = 0;
   float tolerancia = 0.05;


   unsigned int cantTot = 0;
   float prob0s, prob1s, prob00s, prob01s, prob10s, prob11s;
   unsigned char byte, bit, bitAnterior=0;
   FILE* arch = fopen(nombre_archivo,"rb");

   if (arch == NULL){
      printf("No se pudo abrir el archivo: %s\n",nombre_archivo);
      exit(-4);
   }
   
   while (fread(&byte, sizeof(byte), 1, arch)) {
      for (int i = 7 ; i >= 0 ; i-- ) {
         bit = (byte >> i) & 1; 
         //printf("%d",bit);
         
         if (bit == 0){
            cant0s++;
            if (bitAnterior == 0)
               cant00s++;
            else
               cant10s++;
         }
         else{
            cant1s++;
            if (bitAnterior == 0)
               cant01s++;
            else
               cant11s++;
         }           
         cantTot++; 
         bitAnterior = bit;  
      }      
   }
   if (cantTot > 0) {
      prob0s = cant0s/(float)cantTot;
      prob1s = cant1s/(float)cantTot;
      prob00s = cant00s/(float)(cant00s + cant01s);
      prob01s = cant01s/(float)(cant00s + cant01s);
      prob10s = cant10s/(float)(cant10s + cant11s);
      prob11s = cant11s/(float)(cant10s + cant11s);
   }

   printf("\nLa cantidad de 0 es: %d y su prob es: %.2f \n", cant0s,prob0s );
   printf("La cantidad de 1 es: %d y su prob es: %.2f \n", cant1s, prob1s);
   printf("orden 2 --\nLa cantidad de 00 es: %d y su prob es: %.2f \n", cant00s, prob00s);
   printf("La cantidad de 01 es: %d y su prob es: %.2f \n", cant01s, prob01s);
   printf("La cantidad de 10 es: %d y su prob es: %.2f \n", cant10s, prob10s);
   printf("La cantidad de 11 es: %d y su prob es: %.2f \n", cant11s, prob11s);

   if ( fabs(prob00s-prob10s) > tolerancia || fabs(prob01s-prob11s) > tolerancia)
      printf("La fuente es memoria no nula");
   else
      printf("La fuente es memoria nula");


}
