#-------------------------------------------------
#
# Project created by QtCreator 2014-11-26T13:10:50
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhysicsSystem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    addtetdialog.cpp \
    editobjdialog.cpp \
    particlesystem.cpp \
    objvertexsystem.cpp \
    particle.cpp \
    editskeletondialog.cpp \
    simoptionsdialog.cpp \
    editsystemdialog.cpp

HEADERS  += mainwindow.hpp \
    glwidget.hpp \
    vec.hpp \
    addtetdialog.hpp \
    editobjdialog.hpp \
    particlesystem.hpp \
    objvertexsystem.hpp \
    particle.hpp \
    editskeletondialog.hpp \
    simoptionsdialog.hpp \
    editsystemdialog.hpp

FORMS    += mainwindow.ui \
    addtetdialog.ui \
    editobjdialog.ui \
    editskeletondialog.ui \
    simoptionsdialog.ui \
    editsystemdialog.ui

CONFIG += c++11


macx {
    INCLUDEPATH += /opt/local/include
    LIBS += -framework GLUT
    LIBS += -L/opt/local/lib -lassimp
}


unix:!macx {
    LIBS += -lglut -lGL -lGLU -lassimp
}


OTHER_FILES += \
    .gitignore \
    License.txt \
    README.md
