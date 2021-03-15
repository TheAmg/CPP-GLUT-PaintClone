#include<stdio.h>
#include<GL/glut.h>

int curW=1000,curH=1000;
GLfloat br=1.0,bg=1.0,bb=1.0;

GLfloat cbr=0.0,cbg=0.0,cbb=0.0;

int curTool=1;
int totalPrimitives=0;
int curPrimPointer=0;
GLint poX=0,poY=0;

GLfloat markerC=0.5f;
GLint brushSize=10;
bool isMouseHeldLeft=false;
//bool pauseDraw=false;


typedef struct primitive
{
  GLint xPoS[10],yPoS[10];
  GLfloat pr,pg,pb;
  int noVerts=0;
  int curVertPointer=0;
  int limit=5;
  bool drawMarkers=true;
};
typedef struct pixelData
{
  //GLint x,y;
  bool painted=false;
  GLfloat thisR,thisG,thisB;
};

pixelData pixels[1000][1000];
primitive prims[100];

void drawAllPrimitives();
void drawPrimitive(primitive);
void drawPointer();
void keys(unsigned char,int,int);
void drawMarkerAt(GLint,GLint);
void drawBorder();
void drawWithBrush(GLint,GLint);
void drawCanvas();
void drawUnitCanvas(pixelData,GLint,GLint);


void reshape(int w, int h)
{
    curH=h;
    curW=w;
    glViewport(0,0,1000,1000);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLdouble)1000,0.0,(GLdouble)1000);
    glMatrixMode(GL_MODELVIEW);
}
void display()
{
    glClearColor(br,bg,bb,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawCanvas();
    drawBorder();
    drawPointer();

    drawAllPrimitives();

    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();

}
void drawBorder()
{
    glBegin(GL_LINE_LOOP);
    if(br==0.0&&bg==0.0&&bg==0.0)
    {
    glColor3f(1.0,1.0,1.0);
    }
    else{
        glColor3f(0.0,0.0,0.0);
    }
    glVertex2i(1,1);
    glVertex2i(1000,1);
    glVertex2i(1000,1000);
    glVertex2i(1,1000);
    glEnd();

}
void drawUnitCanvas(pixelData p,GLint x,GLint y)
{
    glBegin(GL_POLYGON);
    if(p.painted==true)
    {
        glColor3f(p.thisR,p.thisG,p.thisB);
    }else
    {
        glColor3f(br,bg,bb);
    }
    glVertex2i(x,y);
    glVertex2i(x+1,y);
    glVertex2i(x+1,y+1);
    glVertex2i(x,y+1);
    glEnd();
}
void drawCanvas()
{
    for(int i=0;i<1000;i++)
    {
        for(int j=0;j<1000;j++)
        {
            drawUnitCanvas(pixels[i][j],i,j);
        }
    }
}

void drawWithBrush(GLint x,GLint y)
{
     for(int i=0;i<brushSize;i++)
     {
         for(int j=0;j<brushSize;j++)
         {
             pixels[x+i][y+j].thisR=cbr;
             pixels[x+i][y+j].thisG=cbg;
             pixels[x+i][y+j].thisB=cbb;
             pixels[x+i][y+j].painted=true;
         }
     }
}

void drawMarkerAt(GLint x,GLint y)
{
    glBegin(GL_POLYGON);
    glColor3f(markerC,markerC,markerC);
    glVertex2i(x,y);
    glVertex2i(x+10,y);
    glVertex2i(x+10,y+10);
    glVertex2i(x,y+10);
    glEnd();
}

void movePointerTo(GLint x, GLint y)
{
    poX=x;
    poY=y;
}
void drawPointer()
{
    glBegin(GL_POLYGON);
    if(curTool==2)
    {
        if(prims[curPrimPointer].curVertPointer==4)
        {
           glColor3f(0.2,0.2,0.2);
        }
        else if((prims[curPrimPointer].curVertPointer==0))
        {
           glColor3f(0.5,0.2,0.2);
        }
        else
        {
           glColor3f(0.0,0.8,0.8);
        }

    }
    else
    {
        if(br==0.0&&bg==0.0&&bg==0.0)
        {
            glColor3f(0.8,0.8,0.8);
        }
        else
        {
            glColor3f(0.0,0.0,0.0);
        }

    }
    glVertex2i(poX,poY);
    glVertex2i(poX+brushSize,poY);
    glVertex2i(poX+brushSize,poY+brushSize);
    glVertex2i(poX,poY+brushSize);
    glEnd();
}
void drawPrimitive(primitive p)
{
        glBegin(GL_POLYGON);

        glColor3f(p.pr,p.pg,p.pb);
        for(int i=0;i<p.noVerts;i++)
        {
         glVertex2i(p.xPoS[i],p.yPoS[i]);
        }
        glEnd();
        if(p.drawMarkers==true)
        {
           for(int i=0;i<p.noVerts;i++)
           {
               drawMarkerAt(p.xPoS[i],p.yPoS[i]);
           }
        }
}
void drawAllPrimitives()
{
    for(int i=0;i<totalPrimitives;i++)
    {
        drawPrimitive(prims[i]);
    }
}
void AddPointToPrim(int x, int y)
{
    //printf("Current vertex pointer is %d\n",prims[curPrimPointer].curVertPointer);
        if(prims[curPrimPointer].curVertPointer>=prims[curPrimPointer].limit-1)
    {
        //prims[curPrimPointer].drawMarkers=false;
        curPrimPointer++;
        totalPrimitives++;
        prims[curPrimPointer].drawMarkers=true;
       // pauseDraw=true;
    }
    else
    {
        prims[curPrimPointer].drawMarkers=true;
        prims[curPrimPointer].pr=cbr;
        prims[curPrimPointer].pg=cbg;
        prims[curPrimPointer].pb=cbb;
        prims[curPrimPointer].xPoS[prims[curPrimPointer].curVertPointer]=x;
        prims[curPrimPointer].yPoS[prims[curPrimPointer].curVertPointer]=y;
        prims[curPrimPointer].curVertPointer++;
        prims[curPrimPointer].noVerts++;
    }

    }
void mouseInputF(int btn, int state, int x, int y)
{
     if(btn==GLUT_LEFT_BUTTON && state== GLUT_DOWN && curTool==3)
     {
        // isMouseHeldLeft=true;
         movePointerTo(x,curH-y);
         drawWithBrush(x,curH-y);
         glutPostRedisplay();
     }

    if(btn==GLUT_LEFT_BUTTON && state== GLUT_DOWN && curTool!=2 && curTool!=3)
     {
         printf("Moving to point %d, %d \n",x,y);
         movePointerTo((GLint)x,(GLint)(curH-y));
         //isMouseHeldLeft=true;
         glutPostRedisplay();
     }



     if(btn==GLUT_LEFT_BUTTON && state== GLUT_DOWN &&curTool==2)
     {
         //printf("Moving to point %d, %d \n",x,y);
         AddPointToPrim(x,curH-y);
         movePointerTo((GLint)x,(GLint)(curH-y));
         glutPostRedisplay();
     }
}

void menu(int i)
{
    switch(i)
    {
       case 1: br=0.0;
               bg=0.0;
               bb=0.0;
               break;
       case 2: br=1.0;
               bg=1.0;
               bb=1.0;
               break;
       case 3: exit(0);
               break;
       case 4: cbr=0.0;
               cbg=0.0;
               cbb=0.0;
               break;
       case 5: cbr=1.0;
               cbg=1.0;
               cbb=1.0;
               break;
       case 6: cbr=1.0;
               cbg=0.0;
               cbb=0.0;
               break;
       case 7: cbr=0.0;
               cbg=1.0;
               cbb=0.0;
               break;
       case 8: cbr=0.0;
               cbg=0.0;
               cbb=1.0;
               break;
       case 9: cbr=1.0;
               cbg=1.0;
               cbb=0.0;
               break;
       case 10: curTool=1;
               break;
       case 11: curTool=2;
               break;
       case 12: curTool=3;
               break;
        case 13: brushSize=10;
               break;
        case 14: brushSize=20;
               break;
        case 15: brushSize=50;
               break;
        case 16: brushSize=100;
               break;
    }
    glutPostRedisplay();
}
int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);

    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("PAINT CLONE");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseInputF);
    glutKeyboardFunc(keys);

    int bckSubMenu_ID=glutCreateMenu(menu);
    //printf("Sub Menu id %d\n",bckSubMenu_ID);
    glutAddMenuEntry("Black   ",1);
    glutAddMenuEntry("White   ",2);
    int brushSubMenu_ID=glutCreateMenu(menu);
    glutAddMenuEntry("Black   ",4);
    glutAddMenuEntry("White   ",5);
    glutAddMenuEntry("Red   ",6);
    glutAddMenuEntry("Green   ",7);
    glutAddMenuEntry("Blue   ",8);
    glutAddMenuEntry("Yellow   ",9);
    int currentTool_ID=glutCreateMenu(menu);
    glutAddMenuEntry("Fill Shape   ",10);
    glutAddMenuEntry("Draw Polygon   ",11);
    glutAddMenuEntry("Color brush   ",12);
    int brushSize_ID=glutCreateMenu(menu);
    glutAddMenuEntry("Brush size - 10",13);
    glutAddMenuEntry("Brush size - 20",14);
    glutAddMenuEntry("Brush size - 50",15);
    glutAddMenuEntry("Brush size - 100",16);
    glutCreateMenu(menu);
    glutAddSubMenu("Background color ",bckSubMenu_ID);
    glutAddSubMenu("Brush color ",brushSubMenu_ID);
    glutAddSubMenu("Select Tool ",currentTool_ID);
    glutAddSubMenu("Select Brush Size",brushSize_ID);
    glutAddMenuEntry("Exit   ",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);



    glutMainLoop();
    return 0;

}
void keys(unsigned char key,int,int)
{
    if(key=='c') {printf("Current pointer position is %d %d \n",poX,poY);}

    if(key=='p') {printf("Printing all polygons\n");
                  for(int i=0;i<totalPrimitives;i++)
                  {
                      printf("Poly %d coords are: \n",i+1);

                      for(int j=0;j<prims[i].limit;j++)
                      {
                          printf("%d %d\n",prims[i].xPoS[j],prims[i].yPoS[j]);
                      }
                      printf("Poly %d colors in (R,G,B) are: (%f,%f,%f) \n",i+1,prims[i].pr,prims[i].pg,prims[i].pb);
                  }
                 }

    if(key=='w') {poY++;}
    if(key=='s') {poY--;}
    if(key=='a') {poX--;}
    if(key=='d') {poX++;}

}
