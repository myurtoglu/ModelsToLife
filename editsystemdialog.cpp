#include "editsystemdialog.hpp"
#include "ui_editsystemdialog.h"

#include <QtMath>

EditSystemDialog::EditSystemDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::EditSystemDialog), mPrevXShift(0), mPrevYShift(0),
    mPrevZShift(0), mPrevXRotate(0), mPrevYRotate(0), mPrevZRotate(0)
{
    ui->setupUi(this);
    setUpEditObjDialog();
    createConnections();
    this->setFixedSize(this->width(),this->height());
}


EditSystemDialog::~EditSystemDialog()
{
    delete ui;
}


void EditSystemDialog::setUpEditObjDialog()
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


void EditSystemDialog::createConnections()
{
    connect(ui->verticalSlider_translateX, &QSlider::sliderMoved, this,
            &EditSystemDialog::transX);
    connect(ui->verticalSlider_translateY, &QSlider::sliderMoved, this,
            &EditSystemDialog::transY);
    connect(ui->verticalSlider_translateZ, &QSlider::sliderMoved, this,
            &EditSystemDialog::transZ);
    connect(ui->verticalSlider_translateX, &QSlider::sliderReleased, this,
            &EditSystemDialog::sliderReleased);
    connect(ui->verticalSlider_translateX, &QSlider::sliderPressed, this,
            &EditSystemDialog::sliderPressed);
    connect(ui->verticalSlider_translateY, &QSlider::sliderReleased, this,
            &EditSystemDialog::sliderReleased);
    connect(ui->verticalSlider_translateY, &QSlider::sliderPressed, this,
            &EditSystemDialog::sliderPressed);
    connect(ui->verticalSlider_translateZ, &QSlider::sliderReleased, this,
            &EditSystemDialog::sliderReleased);
    connect(ui->verticalSlider_translateZ, &QSlider::sliderPressed, this,
            &EditSystemDialog::sliderPressed);
    connect(ui->verticalSlider_rotateX, &QSlider::sliderMoved, this,
            &EditSystemDialog::rotateX);
    connect(ui->verticalSlider_rotateY, &QSlider::sliderMoved, this,
            &EditSystemDialog::rotateY);
    connect(ui->verticalSlider_rotateZ, &QSlider::sliderMoved, this,
            &EditSystemDialog::rotateZ);
    connect(ui->verticalSlider_rotateX, &QSlider::sliderReleased, this,
            &EditSystemDialog::sliderReleased);
    connect(ui->verticalSlider_rotateX, &QSlider::sliderPressed, this,
            &EditSystemDialog::sliderPressed);
    connect(ui->verticalSlider_rotateY, &QSlider::sliderReleased, this,
            &EditSystemDialog::sliderReleased);
    connect(ui->verticalSlider_rotateY, &QSlider::sliderPressed, this,
            &EditSystemDialog::sliderPressed);
    connect(ui->verticalSlider_rotateZ, &QSlider::sliderReleased, this,
            &EditSystemDialog::sliderReleased);
    connect(ui->verticalSlider_rotateZ, &QSlider::sliderPressed, this,
            &EditSystemDialog::sliderPressed);
    connect(ui->checkBox_stickObjectToTets, &QCheckBox::stateChanged, this,
            &EditSystemDialog::stickObjectToTetsStateChanged);
}


void EditSystemDialog::sliderReleased()
{
    mPrevXShift = ui->verticalSlider_translateX->value();
    mPrevYShift = ui->verticalSlider_translateY->value();
    mPrevZShift = ui->verticalSlider_translateZ->value();
    mPrevXRotate = ui->verticalSlider_rotateX->value();
    mPrevYRotate = ui->verticalSlider_rotateY->value();
    mPrevZRotate = ui->verticalSlider_rotateZ->value();
}


void EditSystemDialog::sliderPressed()
{
    mObjVerticesInitPos = *mpObjVertices;
    mVerticesInitPos    = *mpVertices;
}


void EditSystemDialog::transX(int state)
{
    auto xShift = 0.05f * (state - mPrevXShift);
    for (unsigned i = 0; i < mpObjVertices->size(); ++i) {
        if (i % 3 == 0) {
            (*mpObjVertices)[i] = mObjVerticesInitPos[i] + xShift;
        }
    }
    for (unsigned i = 0; i < mpVertices->size(); ++i) {
        (*mpVertices)[i][0] = mVerticesInitPos[i][0] + xShift;
    }
    emit objectEdited();
}


void EditSystemDialog::transY(int state)
{
    auto yShift = 0.05f * (state - mPrevYShift);
    for (unsigned i = 0; i < mpObjVertices->size(); ++i) {
        if ((i+2) % 3 == 0) {
            (*mpObjVertices)[i] = mObjVerticesInitPos[i] + yShift;
        }
    }
    for (unsigned i = 0; i < mpVertices->size(); ++i) {
        (*mpVertices)[i][1] = mVerticesInitPos[i][1] + yShift;
    }
    emit objectEdited();
}


void EditSystemDialog::transZ(int state)
{
    auto zShift = 0.05f * (state - mPrevZShift);
    for (unsigned i = 0; i < mpObjVertices->size(); ++i) {
        if ((i+1) % 3 == 0) {
            (*mpObjVertices)[i] = mObjVerticesInitPos[i] + zShift;
        }
    }
    for (unsigned i = 0; i < mpVertices->size(); ++i) {
        (*mpVertices)[i][2] = mVerticesInitPos[i][2] + zShift;
    }
    emit objectEdited();
}


void EditSystemDialog::rotateX(int state)
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

    for (unsigned i = 0; i < mpVertices->size(); ++i) {
        auto posVec = mVerticesInitPos[i] - centerVec;
        auto newVec = Vec3f(posVec[0],
                            posVec[1]*qCos(theta)-posVec[2]*qSin(theta),
                            posVec[1]*qSin(theta)+posVec[2]*qCos(theta))
                           + centerVec;
        (*mpVertices)[i] = newVec;
    }
    emit objectEdited();
}


void EditSystemDialog::rotateY(int state)
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

    for (unsigned i = 0; i < mpVertices->size(); ++i) {
        auto posVec = mVerticesInitPos[i] - centerVec;
        auto newVec = Vec3f(posVec[0]*qCos(theta) + posVec[2]*qSin(theta),
                            posVec[1],
                            -posVec[0]*qSin(theta) + posVec[2]*qCos(theta))
                           + centerVec;
        (*mpVertices)[i] = newVec;
    }
    emit objectEdited();
}


void EditSystemDialog::rotateZ(int state)
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

    for (unsigned i = 0; i < mpVertices->size(); ++i) {
        auto posVec = mVerticesInitPos[i] - centerVec;
        auto newVec = Vec3f(posVec[0]*qCos(theta) - posVec[1]*qSin(theta),
                            posVec[0]*qSin(theta) + posVec[1]*qCos(theta),
                            posVec[2]) + centerVec;
        (*mpVertices)[i] = newVec;
    }
    emit objectEdited();
}


Vec3f EditSystemDialog::findCenter()
{
    float minX = 1000000.f;
    float minY = 1000000.f;
    float minZ = 1000000.f;
    float maxX = -1000000.f;
    float maxY = -1000000.f;
    float maxZ = -1000000.f;

    for (const auto &el : mVerticesInitPos) {

        float tempX = el[0];
        float tempY = el[1];
        float tempZ = el[2];

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


void EditSystemDialog::stickObjectToTetsStateChanged(int state)
{
    emit stickObjectToTetsStateChangedSignal(state);
}
