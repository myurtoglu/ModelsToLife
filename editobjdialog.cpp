#include "editobjdialog.hpp"
#include "ui_editobjdialog.h"

#include <QDebug>
#include <QtMath>
#include <iostream>

EditObjDialog::EditObjDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::EditObjDialog), mPrevXShift(0), mPrevYShift(0),
    mPrevZShift(0), mPrevScale(1.f), mPrevXRotate(0), mPrevYRotate(0),
    mPrevZRotate(0)
{
    ui->setupUi(this);
    setUpEditObjDialog();
    createConnections();
//    this->setFixedSize(this->width(),this->height());
}


EditObjDialog::~EditObjDialog()
{
    delete ui;
}


void EditObjDialog::setUpEditObjDialog()
{
    ui->verticalSlider_translateX->setRange(-100, 100);
    ui->verticalSlider_translateY->setRange(-100, 100);
    ui->verticalSlider_translateZ->setRange(-100, 100);
    ui->verticalSlider_translateX->setSingleStep(1);
    ui->verticalSlider_translateY->setSingleStep(1);
    ui->verticalSlider_translateZ->setSingleStep(1);
    ui->verticalSlider_translateX->setPageStep(10);
    ui->verticalSlider_translateY->setPageStep(10);
    ui->verticalSlider_translateZ->setPageStep(10);
    ui->verticalSlider_scale->setRange(0, 1000);
    ui->verticalSlider_scale->setSingleStep(1);
    ui->verticalSlider_scale->setPageStep(10);
    ui->verticalSlider_scale->setValue(100);

    ui->verticalSlider_rotateX->setRange(0, 120);
    ui->verticalSlider_rotateY->setRange(0, 120);
    ui->verticalSlider_rotateZ->setRange(0, 120);
    ui->verticalSlider_rotateX->setSingleStep(1);
    ui->verticalSlider_rotateY->setSingleStep(1);
    ui->verticalSlider_rotateZ->setSingleStep(1);
    ui->verticalSlider_rotateX->setPageStep(10);
    ui->verticalSlider_rotateY->setPageStep(10);
    ui->verticalSlider_rotateZ->setPageStep(10);
}


void EditObjDialog::createConnections()
{
    connect(ui->verticalSlider_translateX, &QSlider::sliderMoved, this,
            &EditObjDialog::translateXVal);
    connect(ui->verticalSlider_translateY, &QSlider::sliderMoved, this,
            &EditObjDialog::translateYVal);
    connect(ui->verticalSlider_translateZ, &QSlider::sliderMoved, this,
            &EditObjDialog::translateZVal);
    connect(ui->verticalSlider_scale, &QSlider::sliderMoved, this,
            &EditObjDialog::scale);
    connect(ui->verticalSlider_translateX, &QSlider::sliderReleased, this,
            &EditObjDialog::sliderReleased);
    connect(ui->verticalSlider_translateX, &QSlider::sliderPressed, this,
            &EditObjDialog::sliderPressed);
    connect(ui->verticalSlider_translateY, &QSlider::sliderReleased, this,
            &EditObjDialog::sliderReleased);
    connect(ui->verticalSlider_translateY, &QSlider::sliderPressed, this,
            &EditObjDialog::sliderPressed);
    connect(ui->verticalSlider_translateZ, &QSlider::sliderReleased, this,
            &EditObjDialog::sliderReleased);
    connect(ui->verticalSlider_translateZ, &QSlider::sliderPressed, this,
            &EditObjDialog::sliderPressed);
    connect(ui->verticalSlider_scale, &QSlider::sliderReleased, this,
            &EditObjDialog::sliderReleased);
    connect(ui->verticalSlider_scale, &QSlider::sliderPressed, this,
            &EditObjDialog::sliderPressed);

    connect(ui->verticalSlider_rotateX, &QSlider::sliderMoved, this,
            &EditObjDialog::rotateX);
    connect(ui->verticalSlider_rotateY, &QSlider::sliderMoved, this,
            &EditObjDialog::rotateY);
    connect(ui->verticalSlider_rotateZ, &QSlider::sliderMoved, this,
            &EditObjDialog::rotateZ);
    connect(ui->verticalSlider_rotateX, &QSlider::sliderReleased, this,
            &EditObjDialog::sliderReleased);
    connect(ui->verticalSlider_rotateX, &QSlider::sliderPressed, this,
            &EditObjDialog::sliderPressed);
    connect(ui->verticalSlider_rotateY, &QSlider::sliderReleased, this,
            &EditObjDialog::sliderReleased);
    connect(ui->verticalSlider_rotateY, &QSlider::sliderPressed, this,
            &EditObjDialog::sliderPressed);
    connect(ui->verticalSlider_rotateZ, &QSlider::sliderReleased, this,
            &EditObjDialog::sliderReleased);
    connect(ui->verticalSlider_rotateZ, &QSlider::sliderPressed, this,
            &EditObjDialog::sliderPressed);
}


void EditObjDialog::sliderReleased()
{
    mPrevXShift  = ui->verticalSlider_translateX->value();
    mPrevYShift  = ui->verticalSlider_translateY->value();
    mPrevZShift  = ui->verticalSlider_translateZ->value();
    mPrevScale   = 0.01f * ui->verticalSlider_scale->value();
    mPrevXRotate = ui->verticalSlider_rotateX->value();
    mPrevYRotate = ui->verticalSlider_rotateY->value();
    mPrevZRotate = ui->verticalSlider_rotateZ->value();
}


void EditObjDialog::sliderPressed()
{
    mObjVerticesInitPos = *mpObjVertices;
}


void EditObjDialog::translateXVal(int state)
{ 
    auto xShift = 0.05f * (state - mPrevXShift);
    for (unsigned i = 0; i < mpObjVertices->size(); ++i) {
        if (i % 3 == 0) {
            (*mpObjVertices)[i] = mObjVerticesInitPos[i] + xShift;
        }
    }
    emit objectEdited();
}


void EditObjDialog::translateYVal(int state)
{
    auto yShift = 0.05f * (state - mPrevYShift);
    for (unsigned i = 0; i < mpObjVertices->size(); ++i) {
        if ((i+2) % 3 == 0) {
            (*mpObjVertices)[i] = mObjVerticesInitPos[i] + yShift;
        }
    }
    emit objectEdited();
}


void EditObjDialog::translateZVal(int state)
{
    auto zShift = 0.05f * (state - mPrevZShift);
    for (unsigned i = 0; i < mpObjVertices->size(); ++i) {
        if ((i+1) % 3 == 0) {
            (*mpObjVertices)[i] = mObjVerticesInitPos[i] + zShift;
        }
    }
    emit objectEdited();
}


void EditObjDialog::scale(int state)
{
    auto scaling = 0.01f / mPrevScale * state;
    for (unsigned i = 0; i < mpObjVertices->size(); ++i) {
        (*mpObjVertices)[i] = mObjVerticesInitPos[i]*scaling;
    }
    emit objectEdited();
}


void EditObjDialog::rotateX(int state)
{
    auto xRotate = 3.f * (state - mPrevXRotate);
    auto theta = qDegreesToRadians(xRotate);
    auto centerVec = findCenter();

    for (unsigned i = 0; i < mpObjVertices->size()/3; ++i) {
        auto posVec = Vec3f(mObjVerticesInitPos[3*i], mObjVerticesInitPos[3*i+1],
                            mObjVerticesInitPos[3*i+2]) - centerVec;
        auto newVec = Vec3f(posVec[0],
                            posVec[1]*qCos(theta)-posVec[2]*qSin(theta),
                            posVec[1]*qSin(theta)+posVec[2]*qCos(theta))
                           + centerVec;
        (*mpObjVertices)[3*i]   = newVec[0];
        (*mpObjVertices)[3*i+1] = newVec[1];
        (*mpObjVertices)[3*i+2] = newVec[2];
    }
    emit objectEdited();
}


void EditObjDialog::rotateY(int state)
{
    auto yRotate = 3.f * (state - mPrevYRotate);
    auto theta = qDegreesToRadians(yRotate);
    auto centerVec = findCenter();

    for (unsigned i = 0; i < mpObjVertices->size()/3; ++i) {
        auto posVec = Vec3f(mObjVerticesInitPos[3*i], mObjVerticesInitPos[3*i+1],
                            mObjVerticesInitPos[3*i+2]) - centerVec;
        auto newVec = Vec3f(posVec[0]*qCos(theta) + posVec[2]*qSin(theta),
                            posVec[1],
                            -posVec[0]*qSin(theta) + posVec[2]*qCos(theta))
                           + centerVec;
        (*mpObjVertices)[3*i]   = newVec[0];
        (*mpObjVertices)[3*i+1] = newVec[1];
        (*mpObjVertices)[3*i+2] = newVec[2];
    }
    emit objectEdited();
}


void EditObjDialog::rotateZ(int state)
{
    auto zRotate = 3.f * (state - mPrevZRotate);
    auto theta = qDegreesToRadians(zRotate);
    auto centerVec = findCenter();

    for (unsigned i = 0; i < mpObjVertices->size()/3; ++i) {
        auto posVec = Vec3f(mObjVerticesInitPos[3*i], mObjVerticesInitPos[3*i+1],
                            mObjVerticesInitPos[3*i+2]) - centerVec;
        auto newVec = Vec3f(posVec[0]*qCos(theta) - posVec[1]*qSin(theta),
                            posVec[0]*qSin(theta) + posVec[1]*qCos(theta),
                            posVec[2]) + centerVec;
        (*mpObjVertices)[3*i]   = newVec[0];
        (*mpObjVertices)[3*i+1] = newVec[1];
        (*mpObjVertices)[3*i+2] = newVec[2];
    }
    emit objectEdited();
}


Vec3f EditObjDialog::findCenter()
{
    float minX = 1000000.f;
    float minY = 1000000.f;
    float minZ = 1000000.f;
    float maxX = -1000000.f;
    float maxY = -1000000.f;
    float maxZ = -1000000.f;
    for (unsigned i = 0; i < mObjVerticesInitPos.size()/3; ++i) {

        float tempX = mObjVerticesInitPos[3*i];
        float tempY = mObjVerticesInitPos[3*i+1];
        float tempZ = mObjVerticesInitPos[3*i+2];

        minX = tempX < minX ? tempX : minX;
        maxX = tempX > maxX ? tempX : maxX;

        minY = tempY < minY ? tempY : minY;
        maxY = tempY > maxY ? tempY : maxY;

        minZ = tempZ < minZ ? tempZ : minZ;
        maxZ = tempZ > maxZ ? tempZ : maxZ;
    }
    return Vec3f((maxX + minX) / 2.f,
                 (maxY + minY) / 2.f,
                 (maxZ + minZ) / 2.f);
}
