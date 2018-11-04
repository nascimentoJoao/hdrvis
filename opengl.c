#include "opengl.h"

// Dimensões da imagem de entrada
extern int sizeX, sizeY;

// Imagem de entrada
extern RGBf* image;

// Imagem de saída
extern RGB8* image8;

// Texid
GLuint tex;

// Escala de exibição da imagem
float scale;

extern int modo;
extern float exposure;

extern void process();

#define LARGURA_JAN 1024
#define ALTURA_JAN 768

// ************************************************************
//
// A PARTIR DESTE PONTO, O PROGRAMA NÃO DEVE SER ALTERADO
//
// A NÃO SER QUE VOCÊ SAIBA ***EXATAMENTE*** O QUE ESTÁ FAZENDO
//
// ************************************************************
void init(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(10, 10);

    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize(LARGURA_JAN, ALTURA_JAN);
    glutCreateWindow("Visualizador HDR");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrow_keys);
    glutMouseFunc(mouse);

    scale = 1.0f;    // escala inicial de exibição

    modo = SCALE;    // modo inicial de conversão
}

void reshape(int w, int h)
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    gluOrtho2D(0, w, 0, h);

    // Set the clipping volume
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Reconstrói a textura da imagem de saída quando algum parâmetro
// for alterado
void buildTex()
{
    if(tex == -1)
        glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image8);
}

void display()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Fundo de tela preto
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    int glut_width  = glutGet(GLUT_WINDOW_WIDTH);
    int glut_height = glutGet(GLUT_WINDOW_HEIGHT);

    float offsetx, offsety;

    // Calcula offsets para centralizar imagem
    offsetx = glut_width/2 - sizeX*scale/2;
    offsety = glut_height/2 - sizeY*scale/2;

    glColor3ub(255, 255, 255);  // cinza claro

    glPushMatrix();
    // Posiciona e aplica escala na imagem
    glTranslatef(offsetx, offsety, 0);
    glScalef(scale,scale,1.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);

    glTexCoord2f(0,1);
    glVertex2f(0,0);

    glTexCoord2f(1,1);
    glVertex2f(sizeX,0);

    glTexCoord2f(1,0);
    glVertex2f(sizeX, sizeY);

    glTexCoord2f(0,0);
    glVertex2f(0,sizeY);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // Exibe a tela
    glutSwapBuffers();
}

void keyboard( unsigned char key, int x, int y )
{
  switch( key ) {
    case 27:
      free(image8);
      free(image);
      exit(1);
      break;
    case 't':
      printf("Tone mapping\n");
      modo = SCALE;
      process();
      glutPostRedisplay();
      break;
    case 'g':
      printf("Gamma correction\n");
      modo = GAMMA;
      process();
      glutPostRedisplay();
      break;
  }
}

void mouse(int button, int state, int x, int y)
{
    //printf("%d %d %d\n",button,x,y);
    if(button == 3)
        scale -= 0.02;
    else if(button == 4)
        scale += 0.02;
    glutPostRedisplay();
}

#define OFFSET 0.05
void arrow_keys(int a_keys, int x, int y)
{
    switch(a_keys)
    {
    case GLUT_KEY_RIGHT:
        exposure += OFFSET;
        process();
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:
        exposure -= OFFSET;
        if(exposure < 0.05)
            exposure = 0.05;
        process();
        glutPostRedisplay();
        break;
    default:
        break;
    }
}


