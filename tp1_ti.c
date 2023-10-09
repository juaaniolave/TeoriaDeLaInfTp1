#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void printBinaryWithLength(int,int);
void lee_archivo(char*, int);

int main(int argc, char *argv[]) {
   
   char* extension_bin = ".bin";
   char *nombre_archivo=NULL;
   int n=3;


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
   //nombre_archivo = "tp1_sample4.bin"; orden=4;
   unsigned int cant1s   = 0;
   unsigned int cant0s   = 0;
   unsigned int cant00s  = 0;
   unsigned int cant01s  = 0;
   unsigned int cant10s  = 0;
   unsigned int cant11s  = 0;
   float tolerancia = 0.05;
   unsigned int cantTot = 0;
   float prob0s, prob1s, prob00s, prob01s, prob10s, prob11s, entropia = 0, X, Y;
   unsigned char byte, bit, bitAnterior = -1;
   long tamanoArchivo;
   long posArchivo = 0;
   int *miArray;
   int tamano = pow(2,orden);
   int posArray = 0;
   
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
      for (int i = 7 ; i >= 0 ; i--) {        
         bit = (byte >> i) & 1; 
         arrayDeBits[posArchivo++] = bit;
         //printf("%d",bit);
         
         if (bit == 0) {
            cant0s++;
            if (bitAnterior == 0)
               cant00s++;
            else
               if (bitAnterior != -1)
                  cant10s++;
               //salio un 0 y antes un 1
         }
         else {
            cant1s++;
            if (bitAnterior == 0)
               cant01s++;
               //salio un 1 y antes un 0
            else
               if (bitAnterior != -1)
                  cant11s++;
         }          
         cantTot++; 
         bitAnterior = bit;  
      }      
   }

   if (cantTot > 0) {
      prob0s  = cant0s /(float)cantTot;
      prob1s  = cant1s /(float)cantTot;
      prob00s = cant00s/(float)(cant00s + cant01s);
      prob01s = cant01s/(float)(cant00s + cant01s);
      prob10s = cant10s/(float)(cant10s + cant11s);
      prob11s = cant11s/(float)(cant10s + cant11s);
   }
 /*  for(int j=0; j<posArchivo; j++){
      printf("%d", arrayDeBits[j]);
   }*/

   entropia = prob0s * log2(1/prob0s) + prob1s * log2(1/prob1s);

   //printf("\n--orden 1--\n");
   printf("La cantidad de 0 es: %d y su prob es: %.2f \n", cant0s,prob0s);
   printf("La cantidad de 1 es: %d y su prob es: %.2f \n\n\n", cant1s, prob1s);
   printf("Probabilidades condicionales:\nLa cantidad de 00 es: %d y su prob es: %.2f \n", cant00s, prob00s);
   printf("La cantidad de 01 es: %d y su prob es: %.2f \n", cant01s, prob01s);
   printf("La cantidad de 10 es: %d y su prob es: %.2f \n", cant10s, prob10s);
   printf("La cantidad de 11 es: %d y su prob es: %.2f \n\n", cant11s, prob11s);
   int exponente = orden - 1;
   int total = 0;
   if (fabs(prob00s-prob10s) > tolerancia || fabs(prob01s-prob11s) > tolerancia) {
      printf("La fuente es memoria no nula y su entropia es: %.2f bits\n",entropia);
      // calculo del inciso D (vector estacionario)
      // 1ero M-I
      prob00s--;
      prob11s--;
      /*prob00s.X + prob01s.Y = 0
        prob10s.X + prob11s.Y = 0
                X + Y = 1
      */
      X = -1/((prob00s/prob10s) - 1);
      Y = 1 - X;
      printf("Vector estacionario: V = [%.2f;%.2f] \n",X,Y);
   }  
   else {
      printf("La fuente es memoria nula");
      printf(" y su entropia es: %.2f bits\n",entropia);
      
      printf("Probabilidades de orden %d:\n",orden);
      int j = 0;
      while (j < tamanoArchivo) {
         for (int k = j; j < k + orden ; j++){
            if (arrayDeBits[j]) 
               posArray += pow(2,exponente);
            exponente--;
         }
         exponente = orden-1;
         total++;
         miArray[posArray]++;
         posArray = 0;
      }
      for(int i = 0 ; i < tamano ; i++) {
         printBinaryWithLength(i,orden);
         printf("%.4f\n",miArray[i]/(float)total);
      }
      printf("\nCalculado con multiplicaciones:\n");
      float probN = -1;
      for (int k = 0; k < pow(2,orden) ; k++) {
         int i = k;
         if (probN >= 0) 
            printf(": %.4f\n",probN);
         probN = 1;
         for (j = 0; j < orden ; j++){
            if (i & 1)
               probN *= prob1s;
            else 
               probN *= prob0s;
            printf("%d",(i & 1));
            i >>= 1;
         }
      }
      printf(": %.4f\n",probN);
      float entropiaSumada = 0;
      for (int p = 0; p < tamano ; p++) {
         if (miArray[p] != 0)
            entropiaSumada += (miArray[p]/(float)total) * log2(total/(float)miArray[p]);
      }

      printf("\n");
      printf("Entropia de orden %d (orden*entropia): %.2f\n",orden, orden*entropia);
      printf("Entropia de orden %d (sumatoria de probs): %.2f", orden, entropiaSumada);
   }
   fclose(arch);
}


void printBinaryWithLength(int num, int length) {
    if (length <= 0) {
       printf("Longitud inválida.\n");
       return;
    }

    // Calcular el número mínimo de dígitos necesarios para representar num
    int minDigits = 1;
    int absNum    = abs(num);

    while ((1 << minDigits) <= absNum) 
        minDigits++;

    // Imprimir ceros a la izquierda si es necesario
    for (int i = 0; i < length - minDigits; i++) 
       printf("0");

    // Luego, imprimir los dígitos binarios
    for (int i = minDigits - 1 ; i >= 0 ; i--) 
        printf("%d", (absNum >> i) & 1);
    
    printf(": ");
}