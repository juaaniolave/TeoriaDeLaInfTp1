#include<stdio.h>
#include<conio.h>
#include<stdlib.h>

int main()
{
    FILE *arch;
    arch=fopen("archivo1.bin","wb");
    if (arch==NULL)
        exit(1);

    char letra='A';
    fwrite(&letra, sizeof(char), 1, arch);
    int valor1=125;
    fwrite(&valor1, sizeof(int), 1, arch);
    float valor2=45.25;
    fwrite(&valor2, sizeof(float), 1, arch);

    fclose(arch);

    printf("Se creo un archivo binario que almacena un char, un int y un float.");
    return 0;
}