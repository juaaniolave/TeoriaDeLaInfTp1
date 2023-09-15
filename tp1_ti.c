#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void printBinaryWithLength(int,int);
void lee_archivo(char*, int);

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
   lee_archivo(nombre_archivo, n);

   return 0;
}

void lee_archivo (char *nombre_archivo, int orden){
   unsigned int cant1s  = 0;
   unsigned int cant0s  = 0;
   unsigned int cant00s  = 0;
   unsigned int cant01s  = 0;
   unsigned int cant10s  = 0;
   unsigned int cant11s  = 0;
   float tolerancia = 0.05;
   unsigned int cantTot = 0;
   float prob0s, prob1s, prob00s, prob01s, prob10s, prob11s, entropia = 0;
   unsigned char byte, bit, bitAnterior=0;
   long tamanoArchivo;
   long posArchivo=0;
   int *miArray;
   int tamano = pow(2,orden);
   int posArray=0;
  // nombre_archivo = "tp1_sample0.bin";
   FILE* arch = fopen(nombre_archivo,"rb");
   // Asignar memoria dinámica para el array
   miArray = (int *)malloc(tamano* sizeof(int));
   memset(miArray, 0, tamano*sizeof(int)); //inicializa en 0 todo miarray

   if (miArray == NULL) {
      printf("No se pudo asignar memoria para el array.\n");
      exit(-1);
   }


   if (arch == NULL){
      printf("No se pudo abrir el archivo: %s\n",nombre_archivo);
      exit(-4);
   }
   fseek(arch, 0, SEEK_END); // Mueve el puntero al final del archivo
   tamanoArchivo = ftell(arch)*8; // Obtiene la posición actual, que es el tamaño del archivo
   fseek(arch, 0, SEEK_SET); // Vuelve a mover el puntero al inicio del archivo

   // Crea un array para almacenar el contenido del archivo
   unsigned char *arrayDeBits = (unsigned char *)malloc(tamanoArchivo);   
   
   while (fread(&byte, sizeof(byte), 1, arch)) {
      for (int i = 7 ; i >= 0 ; i-- ) {        
         bit = (byte >> i) & 1; 
         arrayDeBits[posArchivo++]=bit;
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
  /* for(int j=0; j<posArchivo; j++){
      printf("%d", arrayDeBits[j]);
   }
   */
   
   entropia = prob0s*log2(1/prob0s)+ prob1s*log2(1/prob1s);

   printf("\n--orden 1--\n");
   printf("La cantidad de 0 es: %d y su prob es: %.2f \n", cant0s,prob0s );
   printf("La cantidad de 1 es: %d y su prob es: %.2f \n", cant1s, prob1s);
   printf("--orden 2 --\nLa cantidad de 00 es: %d y su prob es: %.2f \n", cant00s, prob00s);
   printf("La cantidad de 01 es: %d y su prob es: %.2f \n", cant01s, prob01s);
   printf("La cantidad de 10 es: %d y su prob es: %.2f \n", cant10s, prob10s);
   printf("La cantidad de 11 es: %d y su prob es: %.2f \n\n", cant11s, prob11s);
   int exponente = orden-1;
   
   if ( fabs(prob00s-prob10s) > tolerancia || fabs(prob01s-prob11s) > tolerancia){
      printf("La fuente es memoria no nula\n");
      for(int j=0; j<tamanoArchivo; j++){
         for (int k=j; k<j+orden;k++){
            if (arrayDeBits[k]){
               posArray+=pow(2,exponente);
            }
            exponente--;
         }
         exponente = orden-1;
         miArray[posArray]++;
         posArray=0;
   }
   for(int i =0 ; i<tamano; i++){
      printBinaryWithLength(i,orden);
      printf("%d\n",miArray[i]);
   }


   }  
   else{
      printf("La fuente es memoria nula");
   }
   printf(" y su entropia es: %.2f bits\n ",entropia);

   fclose(arch);

}

void printBinaryWithLength(int num, int length) {
    if (length <= 0) {
        printf("Longitud inválida.\n");
        return;
    }

    // Calcular el número mínimo de dígitos necesarios para representar num
    int minDigits = 1;
    int absNum = abs(num);
    while ((1 << minDigits) <= absNum) {
        minDigits++;
    }

    // Imprimir ceros a la izquierda si es necesario
    for (int i = 0; i < length - minDigits; i++) {
        printf("0");
    }

    // Luego, imprimir los dígitos binarios
    for (int i = minDigits - 1; i >= 0; i--) {
        printf("%d", (absNum >> i) & 1);
    }
   printf(": ");
}