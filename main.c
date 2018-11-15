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
int min(int a, int b);

// Função principal de processamento: ela deve chamar outras funções
// quando for necessário (ex: algoritmos de tone mapping, etc)
void process()
{
    printf("Exposure: %.3f\n", exposure);
    float red, green, blue;
    //
    //
    // SUBSTITUA este código pelos algoritmos a serem implementados
    //
    for(int index = 0; index < (sizeX * sizeY); index++) {
        red = (image[index].r * exposure);
        green = (image[index].g * exposure);
        blue = (image[index].b * exposure);

        float filterRed;
        float filterGreen;
        float filterBlue;

        if(modo == SCALE) {
            filterRed = red / (red + tone_value);
            filterGreen = green / (green + tone_value);
            filterBlue = blue / (blue + tone_value);
        } else {
            filterRed = fastpow(red, (1 / gamma_value));
            filterGreen = fastpow(green, (1 / gamma_value));
            filterBlue = fastpow(blue, (1 / gamma_value));
        }

        int R8 = (int)(min(1.0,filterRed)* 255);
        int G8 = (int) (min(1.0,filterGreen)* 255);
        int B8 = (int)(min(1.0,filterBlue)* 255);

        image8[index].r = R8;
        image8[index].g = G8;
        image8[index].b = B8;
    }
    printf("RED: %f - GREEN: %f - BLUE: %f\n", red, green, blue);
    //
    // NÃO ALTERAR A PARTIR DAQUI!!!!
    //
    buildTex();
}

int getImageSize() {
    return sizeX * sizeY;
}

void applyExposure(RGBf pixel) {
    pixel.r = pixel.r * exposure;
    pixel.g = pixel.g * exposure;
    pixel.b = pixel.b * exposure;
}

void toneMapping(RGBf pixel){
    pixel.r = pixel.r / (pixel.r + tone_value);
    pixel.g = pixel.g / (pixel.g + tone_value);
    pixel.b = pixel.b / (pixel.b + tone_value);
}


void gammaCorrection(RGBf pixel){
    pixel.r = fastpow(pixel.r, (1 / gamma_value));
    pixel.g = fastpow(pixel.g, (1 / gamma_value));
    pixel.b = fastpow(pixel.b, (1 / gamma_value));
}

int min( int a, int b )
{ return (a) < (b) ? (a) : (b); }

int convertTo8Bits(int color){
    if (color > 1) color = 1;
    return (int) (min(1, color) * 255);
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

