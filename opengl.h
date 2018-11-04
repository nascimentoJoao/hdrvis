#ifndef MYOPENGL_H
#define MYOPENGL_H

#ifdef WIN32
#include <windows.h>    // includes only in MSWindows not in UNIX
#include "gl/glut.h"
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>

// Modo de exibição: escala simples ou correção gama
enum { SCALE, GAMMA };

// Representa um pixel na imagem HDR
typedef struct {
    float r, g, b;
} RGBf;

// Representa um pixel na imagem de 24 bits
typedef struct {
    unsigned char r, g, b;
} RGB8;

// Funções da interface gráfica e OpenGL
void init(int argc, char** argv);
void display();
void keyboard( unsigned char key, int x, int y );
void mouse(int button, int state, int x, int y);
void arrow_keys(int key, int x, int y);
void reshape(int w, int h);
void buildTex();

#endif // MYOPENGL_H
