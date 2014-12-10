#include "mainwindow.hpp"

#include <QApplication>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
