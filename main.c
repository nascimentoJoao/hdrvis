#include <math.h>
#include <string.h>		// para usar strings

// Rotinas para acesso da OpenGL
#include "opengl.h"

// Rotinas para leitura de arquivos .hdr
#include "rgbe.h"

// Variáveis globais a serem utilizadas:

// Dimensões da imagem de entrada
int sizeX, sizeY;

// Imagem de entrada
RGBf* image;

// Imagem de saída
RGB8* image8;

// Fator de exposição inicial
float exposure = 1.0, gamma_value = 1.8, tone_value = 0.5;

// Modo de exibição atual
int modo;

// Função pow mais eficiente (cerca de 7x mais rápida)
float fastpow(float a, float b);
float fastpow(float a, float b) {
     union { float f; int i; }
      u = { a };
      u.i = (int)(b * (u.i - 1065307417) + 1065307417);
      return u.f;
}

void toneMapping();
void gammaCorrection();
void correctImage();
int min(int a, int b);

// Função principal de processamento: ela deve chamar outras funções
// quando for necessário (ex: algoritmos de tone mapping, etc)
void process()
{
    printf("Exposure: %.3f\n", exposure);
    //
    //
    // SUBSTITUA este código pelos algoritmos a serem implementados
    //

    int position;
    for(position=0; position<sizeX*sizeY; position++) {

        image8[position].r = (unsigned char) (image[position].r * exposure);
        image8[position].g = (unsigned char) (image[position].g * exposure);
        image8[position].b = (unsigned char) (image[position].b * exposure);
    }
    //
    // NÃO ALTERAR A PARTIR DAQUI!!!!
    //
    buildTex();
}

void toneMapping(){
    int position;
    for(position = 0; position < sizeX * sizeY; position++){
        image8[position].r = (unsigned char) ((image[position].r) / ((image[position].r) + exposure));
        image8[position].g = (unsigned char) ((image[position].g) / ((image[position].g) + exposure));
        image8[position].b = (unsigned char) ((image[position].b) / ((image[position].b) + exposure));
    }
}

int min( int a, int b )
{ return a < b ? a : b;
}

void convertImage(){
     int position;
    for(position = 0; position < sizeX * sizeY; position++){
        image8[position].r = (unsigned char) ((min(1.0, image[position].r)) * 255);
        image8[position].g = (unsigned char) ((min(1.0, image[position].g)) * 255);
        image8[position].b = (unsigned char) ((min(1.0, image[position].b)) * 255);
    }
}

void gammaCorrection(){
    printf("entrou em gamma correction\n");
    int position;
    for(position = 0; position < sizeX * sizeY; position++){
        image8[position].r = (unsigned char) (fastpow(image8[position].r, gamma_value));
        image8[position].g = (unsigned char) (fastpow(image8[position].g, gamma_value));
        image8[position].b = (unsigned char) (fastpow(image8[position].b, gamma_value));
    }
}

int main(int argc, char** argv)
{
    if(argc==1) {
        printf("hdrvis [image file.hdr]\n");
        exit(1);
    }

    // Inicialização da janela gráfica
    init(argc,argv);

    //
    // INCLUA aqui o código para LER a imagem de entrada
    //
    // Siga as orientações no enunciado para:
    //
    // 1. Descobrir o tamanho da imagem (ler header)
    // 2. Ler os pixels

    FILE* arquivo = fopen(argv[1], "rb");
    RGBE_ReadHeader(arquivo, &sizeX, &sizeY, NULL);

    // Aloca imagem float
    image = (RGBf *)malloc(sizeof(RGBf) * sizeX * sizeY);

    // Aloca memória para imagem de 24 bits
    image8 = (RGB8*) malloc(sizeof(RGB8) * sizeX * sizeY);


    int result = RGBE_ReadPixels_RLE(arquivo, (float*)image, sizeX, sizeY);

    if(result == RGBE_RETURN_FAILURE){
        printf("Não foi possível abrir a imagem!");
    }

    exposure = 1.0f; // exposição inicial

    //int pos;
      //  for(pos = 0; pos < sizeX * sizeY; pos++){
      //  printf("r = %d\n", (unsigned char) (image[pos].r * exposure));
      //  printf("g = %d\n", (unsigned char) (image[pos].g * exposure));
      //  printf("b = %d\n", (unsigned char) (image[pos].b * exposure));
    // }

    // Aplica processamento inicial
    process();

    // Não retorna... a partir daqui, interação via teclado e mouse apenas, na janela gráfica
    glutMainLoop();
    return 0;
}

