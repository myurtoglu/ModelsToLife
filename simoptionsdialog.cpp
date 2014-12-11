#include "simoptionsdialog.hpp"
#include "ui_simoptionsdialog.h"

SimOptionsDialog::SimOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimOptionsDialog)
{
    ui->setupUi(this);
    setUpSimOptionsDialog();
    createConnections();
    this->setFixedSize(this->width(),this->height());
}


SimOptionsDialog::~SimOptionsDialog()
{
    delete ui;
}


void SimOptionsDialog::setUpSimOptionsDialog()
{
    ui->horizontalSlider_deltaT->setRange(0, 100);
    ui->horizontalSlider_deltaT->setSingleStep(1);
    ui->horizontalSlider_deltaT->setPageStep(10);
    ui->horizontalSlider_deltaT->setValue(8); // 8 ms

    ui->horizontalSlider_gravity->setRange(-100, 150);
    ui->horizontalSlider_gravity->setSingleStep(1);
    ui->horizontalSlider_gravity->setPageStep(10);
    ui->horizontalSlider_gravity->setValue(20); // 2.0 m/s^2

    ui->horizontalSlider_restitution->setRange(0, 100);
    ui->horizontalSlider_restitution->setSingleStep(1);
    ui->horizontalSlider_restitution->setPageStep(10);
    ui->horizontalSlider_restitution->setValue(25); // 0.25

    ui->horizontalSlider_stiffness->setRange(0, 100);
    ui->horizontalSlider_stiffness->setSingleStep(1);
    ui->horizontalSlider_stiffness->setPageStep(10);
    ui->horizontalSlider_stiffness->setValue(20); // 100 N/m

    ui->horizontalSlider_damping->setRange(0, 200);
    ui->horizontalSlider_damping->setSingleStep(1);
    ui->horizontalSlider_damping->setPageStep(10);
    ui->horizontalSlider_damping->setValue(40); // 2

    ui->horizontalSlider_mass->setRange(0, 100);
    ui->horizontalSlider_mass->setSingleStep(1);
    ui->horizontalSlider_mass->setPageStep(10);
    ui->horizontalSlider_mass->setValue(4); // 0.2 kg
}


void SimOptionsDialog::createConnections()
{
    connect(ui->horizontalSlider_deltaT, &QSlider::valueChanged, this,
            &SimOptionsDialog::changeDeltaT);
    connect(ui->horizontalSlider_gravity, &QSlider::valueChanged, this,
            &SimOptionsDialog::changeGravity);
    connect(ui->horizontalSlider_restitution, &QSlider::valueChanged, this,
            &SimOptionsDialog::changeRestitution);
    connect(ui->horizontalSlider_stiffness, &QSlider::valueChanged, this,
            &SimOptionsDialog::changeStiffness);
    connect(ui->horizontalSlider_damping, &QSlider::valueChanged, this,
            &SimOptionsDialog::changeDamping);
    connect(ui->horizontalSlider_mass, &QSlider::valueChanged, this,
            &SimOptionsDialog::changeMass);
}


void SimOptionsDialog::changeDeltaT(int state)
{
    *mpDeltaT = 0.001f * state;
    emit propertiesChanged();
}


void SimOptionsDialog::changeGravity(int state)
{
    *mpGravity = 0.1f * state;
    emit propertiesChanged();
}


void SimOptionsDialog::changeRestitution(int state)
{
    *mpRestitution = 0.01f * state;
    emit propertiesChanged();
}


void SimOptionsDialog::changeStiffness(int state)
{
    *mpStiffness = 5.f * state;
    emit propertiesChanged();
}


void SimOptionsDialog::changeDamping(int state)
{
    *mpDamping = 0.05f * state;
    emit propertiesChanged();
}


void SimOptionsDialog::changeMass(int state)
{
    *mpMass = 0.05f * state;
    emit propertiesChanged();
}
