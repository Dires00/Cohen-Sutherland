#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define W 1600
#define H 900

typedef struct {
    double x0, y0, x1, y1;
} linha;

typedef linha* Linha;

void desenhaBorda(double xmin, double xmax, double ymin, double ymax){
    glColor3f(0., 0., 1.);
    
    glBegin(GL_LINE_LOOP);
        glVertex2f(xmin, ymin);
        glVertex2f(xmin, ymax);
        glVertex2f(xmax, ymax);
        glVertex2f(xmax, ymin);
    glEnd();

}

void desenhaLinha(double x0, double y0, double x1, double y1, bool accept)
{    
    if(accept){
        glColor3f(0., 1., 0.);
    } else {
        glColor3f(1., 0., 0.);
    }
    
    glBegin(GL_LINES);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
    glEnd();
}

unsigned char code(double x, double y, double xmin, double xmax, double ymin, double ymax){
    unsigned char code = 0;

    if(y > ymax){
        code += 8;
    } else if(y < ymin){
        code += 4;
    }

    if(x > xmax){
        code += 2;
    }
    else if(x < xmin){
        code += 1;
    }

    return code;
}

void cohenSutherlandLineClip(double x0, double y0, double x1, double y1, double xmin, double xmax, double ymin, double ymax){
    unsigned char outcode0, outcode1, outcodeOut;
    bool accept = false, done = false;
    double x, y;

    outcode0 = code(x0, y0, xmin, xmax, ymin, ymax);
    outcode1 = code(x1, y1, xmin, xmax, ymin, ymax);

    do{
        if(!outcode0 && !outcode1){ //trivial draw and exit
            accept = true;
            done = true;
        } else if((outcode0 & outcode1) != 0){ // trivial reject and exit
            done = true;
        } else {
            outcodeOut = (outcode0) ? outcode0 : outcode1; // pick an out vertice
            if (outcodeOut & 8){ // discart top
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            } else if(outcodeOut & 4){ // discart bottom
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            } else if(outcodeOut & 2){
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            } else if(outcodeOut & 1){
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            if(outcodeOut == outcode0) {
                desenhaLinha(x0, y0, x, y, false);
                x0 = x;
                y0 = y;
                outcode0 = code(x0, y0, xmin, xmax, ymin, ymax);
            } else {
                desenhaLinha(x, y, x1, y1, false);
                x1 = x;
                y1 = y;
                outcode1 = code(x1, y1, xmin, xmax, ymin, ymax);
            }
        }
    }while (!done);

    desenhaLinha(x0, y0, x1, y1, accept);

}

void geraLinhas(){
    double ymax = 700, ymin = 200, xmax = 1200, xmin = 400;
    
    desenhaBorda(xmin, xmax, ymin, ymax);

    int numLinhas = 8;
    Linha linhas = (Linha) malloc(sizeof(linha) * numLinhas);
    linhas[0].x0 = 300;
    linhas[0].y0 = 450;
    linhas[0].x1 = 600;
    linhas[0].y1 = 650;

    linhas[1].x0 = 600;
    linhas[1].y0 = 100;
    linhas[1].x1 = 1000;
    linhas[1].y1 = 450;

    linhas[2].x0 = 1100;
    linhas[2].y0 = 800;
    linhas[2].x1 = 1150;
    linhas[2].y1 = 500;

    linhas[3].x0 = 900;
    linhas[3].y0 = 300;
    linhas[3].x1 = 1500;
    linhas[3].y1 = 400;

    linhas[4].x0 = 300;
    linhas[4].y0 = 300;
    linhas[4].x1 = 600;
    linhas[4].y1 = 150;

    linhas[5].x0 = 1000;
    linhas[5].y0 = 800;
    linhas[5].x1 = 1500;
    linhas[5].y1 = 500;

    linhas[6].x0 = 600;
    linhas[6].y0 = 450;
    linhas[6].x1 = 800;
    linhas[6].y1 = 600;

    linhas[7].x0 = 100;
    linhas[7].y0 = 450;
    linhas[7].x1 = 300;
    linhas[7].y1 = 600;


    for(int i = 0; i < numLinhas; i++){
        cohenSutherlandLineClip(linhas[i].x0, linhas[i].y0, linhas[i].x1, linhas[i].y1, xmin, xmax, ymin, ymax);
    }
 
}

void Desenha(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    geraLinhas();

    

    glutSwapBuffers();
}

void Inicializa (void)
{   
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(.0f, .0f, .0f, 1.0f);
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
     if(w == 0) w = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f, W, 0.0f, H);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(W, H);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Recorte!");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    Inicializa();
    glutMainLoop();

    return 0;
}