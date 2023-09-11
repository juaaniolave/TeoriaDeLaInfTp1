#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int cant1s  = 0;
unsigned int cant0s  = 0;
unsigned int cantTot = 0; 
float prob0s;
float prob1s;

int main () {

}

void lee_archivo (char *nombre_archivo){
   unsigned char byte, bit;
   FILE* arch = fopen(nombre_archivo,"rb");

   if (arch == NULL){
      printf("No se pudo abrir el archivo: %s\n",nombre_archivo);
      exit(-4);
   }
   while (fread(&byte, sizeof(byte), 1, arch)) {
      for (int i = 0 ; i < 8 ; i++ ) {
         bit = (byte >> i) & 1; 
         if (bit == 0)
            cant0s++;
         else
            cant1s++;
         cantTot++;   
      }      
   }
   if (cantTot > 0) {
      prob0s = cant0s/(float)cantTot;
      prob1s = cant1s/(float)cantTot;
   }
}
}