#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
                                         ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUpMainWindow();
    createDialogs();
    connectVariables();
    createConnections();
    createShortcutKeys();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setUpMainWindow()
{
    ui->checkBox_showTets->setChecked(true);
    ui->checkBox_showVerts->setChecked(true);
    ui->checkBox_showAxes->setChecked(true);
    ui->checkBox_showObject->setChecked(true);
    ui->checkBox_showGround->setChecked(true);
    propertiesChanged();
}


void MainWindow::createDialogs()
{
    pAddTetDialog = new AddTetDialog(this);
    pAddTetDialog->setWindowTitle("Edit tetrahedra");

    pEditObjDialog = new EditObjDialog(this);
    pEditObjDialog->setWindowTitle("Edit object");

    pEditSkeletonDialog = new EditSkeletonDialog(this);
    pEditSkeletonDialog->setWindowTitle("Edit skeleton");

    pSimOptionsDialog = new SimOptionsDialog(this);
    pSimOptionsDialog->setWindowTitle("Simulation Parameters");

    pEditSystemDialog = new EditSystemDialog(this);
    pEditSystemDialog->setWindowTitle("Edit system");
}


void MainWindow::connectVariables()
{
    pAddTetDialog->mpVertices        = &(ui->widget->mVertices);
    pAddTetDialog->mpTets            = &(ui->widget->mTets);
    pAddTetDialog->mpPreviewVertPos  = &(ui->widget->mPreviewVertPos);

    pEditSkeletonDialog->mpVertices  = &(ui->widget->mVertices);
    pEditSkeletonDialog->mpBones     = &(ui->widget->mBones);

    pSimOptionsDialog->mpDeltaT      = &(ui->widget->mParticleSystem.mDeltaT);
    pSimOptionsDialog->mpGravity     = &(ui->widget->mParticleSystem.mGravity);
    pSimOptionsDialog->mpRestitution = &(ui->widget->mParticleSystem.mRestitution);
    pSimOptionsDialog->mpStiffness   = &(ui->widget->mParticleSystem.mStiffness);
    pSimOptionsDialog->mpDamping     = &(ui->widget->mParticleSystem.mDamping);
    pSimOptionsDialog->mpMass        = &(ui->widget->mParticleSystem.mMass);

    pEditObjDialog->mpObjVertices    = &(ui->widget->mObjVertices);

    pEditSystemDialog->mpObjVertices = &(ui->widget->mObjVertices);
    pEditSystemDialog->mpVertices    = &(ui->widget->mVertices);
}


void MainWindow::createConnections()
{
    connect(ui->actionSave, &QAction::triggered, this,
            &MainWindow::saveToFile);
    connect(ui->actionOpen, &QAction::triggered, this,
            &MainWindow::openFile);
    connect(ui->actionOpen_Object, &QAction::triggered, this,
            &MainWindow::openObjFile);
    connect(ui->actionSave_All, &QAction::triggered, this,
            &MainWindow::saveAllToFile);
    connect(ui->actionOpen_All, &QAction::triggered, this,
            &MainWindow::openAllFromFile);
    connect(ui->actionCreate_tets_from_ply, &QAction::triggered, this,
            &MainWindow::createTetFromPly);

    connect(ui->pushButton_editObj, &QPushButton::clicked, this,
            &MainWindow::openEditObjectDialog);
    connect(ui->pushButton_addTet, &QPushButton::clicked, this,
            &MainWindow::openAddTetDialog);
    connect(ui->pushButton_editSkeleton, &QPushButton::clicked, this,
            &MainWindow::openEditSkeletonDialog);
    connect(ui->pushButton_resetSim, &QPushButton::clicked, ui->widget,
            &GLWidget::resetSim);
    connect(ui->pushButton_simOptions, &QPushButton::clicked, this,
            &MainWindow::openSimOptionsDialog);
    connect(ui->pushButton_editSystem, &QPushButton::clicked, this,
            &MainWindow::openEditSystemDialog);

    connect(ui->checkBox_showVerts, &QCheckBox::stateChanged, ui->widget,
            &GLWidget::showVertsStateChanged);
    connect(ui->checkBox_showTets, &QCheckBox::stateChanged, ui->widget,
            &GLWidget::showTetsStateChanged);
    connect(ui->checkBox_showAxes, &QCheckBox::stateChanged, ui->widget,
            &GLWidget::showAxesStateChanged);
    connect(ui->checkBox_showObject, &QCheckBox::stateChanged, ui->widget,
            &GLWidget::showObjectStateChanged);
    connect(ui->checkBox_simulate, &QCheckBox::stateChanged, ui->widget,
            &GLWidget::simulationStateChanged);
    connect(ui->checkBox_showSkeleton, &QCheckBox::stateChanged, ui->widget,
            &GLWidget::showSkeletonStateChanged);
    connect(ui->checkBox_showGround, &QCheckBox::stateChanged, ui->widget,
            &GLWidget::showGroundStateChanged);

    connect(pAddTetDialog, &AddTetDialog::previewVertStateChanged, ui->widget,
            &GLWidget::previewVertStateChanged);
    connect(pAddTetDialog, &AddTetDialog::tetsEdited, ui->widget,
            &GLWidget::tetsEdited);
    connect(pEditObjDialog, &EditObjDialog::objectEdited, ui->widget,
            &GLWidget::objectEdited);
    connect(pEditSkeletonDialog, &EditSkeletonDialog::bonesEdited, ui->widget,
            &GLWidget::bonesEdited);
    connect(pSimOptionsDialog, &SimOptionsDialog::propertiesChanged, this,
            &MainWindow::propertiesChanged);
    connect(pEditSystemDialog, &EditSystemDialog::objectEdited, ui->widget,
            &GLWidget::objectEdited);
    connect(pEditSystemDialog, &EditSystemDialog::stickObjectToTetsStateChangedSignal,
            ui->widget, &GLWidget::stickObjectToTetsStateChanged);
    connect(pEditSkeletonDialog, &EditSkeletonDialog::stickBonesStateChangedSignal,
            ui->widget, &GLWidget::boneStickTetStateChanged);

    connect(pEditSystemDialog, &EditSystemDialog::accepted, this,
            &MainWindow::editAccepted);
    connect(pEditObjDialog, &EditObjDialog::accepted, this,
            &MainWindow::editAccepted);
    connect(pEditSkeletonDialog, &EditSkeletonDialog::accepted, this,
            &MainWindow::editAccepted);
    connect(pAddTetDialog, &AddTetDialog::accepted, this, &MainWindow::editAccepted);
}


void MainWindow::createShortcutKeys()
{
    ui->actionSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    ui->actionOpen->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    ui->actionOpen_Object->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
    ui->actionSave_All->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    ui->actionOpen_All->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_L));

    QAction *exit = new QAction(this);
    exit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

    connect(exit, SIGNAL(triggered()), this, SLOT(close()));
    this->addAction(exit);

    QAction *showTets = new QAction(this);
    showTets->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));

    connect(showTets, SIGNAL(triggered()), ui->checkBox_showTets,
            SLOT(toggle()));
    this->addAction(showTets);

    QAction *showVerts = new QAction(this);
    showVerts->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));

    connect(showVerts, SIGNAL(triggered()), ui->checkBox_showVerts,
            SLOT(toggle()));
    this->addAction(showVerts);

    QAction *showObject = new QAction(this);
    showObject->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));

    connect(showObject, SIGNAL(triggered()), ui->checkBox_showObject,
            SLOT(toggle()));
    this->addAction(showObject);
}


void MainWindow::openAddTetDialog()
{
    pAddTetDialog->updateLists();
    pAddTetDialog->show();
}


void MainWindow::openEditObjectDialog()
{
    pEditObjDialog->show();
}


void MainWindow::openEditSkeletonDialog()
{
    pEditSkeletonDialog->updateLists();
    pEditSkeletonDialog->show();
}


void MainWindow::openSimOptionsDialog()
{
    pSimOptionsDialog->show();
}


void MainWindow::openEditSystemDialog()
{
    pEditSystemDialog->show();
}


void MainWindow::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Tet File"),
                                                    "",
                                                    tr("Tet file (*.tet);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }
    else {
        ui->widget->saveToFile(fileName);
    }
}


void MainWindow::openFile()
{
    ui->checkBox_simulate->setChecked(false);
    ui->widget->simulationStateChanged(Qt::Unchecked);
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Tet File"), "",
                                                    tr("Tet file (*.tet);;All Files (*)"));

    if (fileName.isEmpty()) {
        return;
    }
    else {
        ui->widget->openTetFile(fileName);
    }
}


void MainWindow::openObjFile()
{
    ui->checkBox_simulate->setChecked(false);
    ui->widget->simulationStateChanged(Qt::Unchecked);
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Object"), "",
                                                    tr("Object (*.ply);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }
    else {
        ui->widget->loadModel(fileName);
    }
}


void MainWindow::saveAllToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Physics Simulation to File"),
                                                    "",
                                                    tr("Simulation file (*.physim);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }
    else {
        ui->widget->saveAllToFile(fileName);
    }
}


void MainWindow::openAllFromFile()
{
    ui->checkBox_simulate->setChecked(false);
    ui->widget->simulationStateChanged(Qt::Unchecked);
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Physics Simulation from File"),
                                                    "",
                                                    tr("Simulation file (*.physim);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }
    else {
        ui->widget->openAllFromFile(fileName);
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    float vel = 0.2;
    if (event->key() == Qt::Key_D) {
        ui->widget->mHorMove += vel;
    }
    else if (event->key() == Qt::Key_A) {
        ui->widget->mHorMove -= vel;
    }
    else if (event->key() == Qt::Key_W) {
        ui->widget->mVertMove -= vel;
    }
    else if (event->key() == Qt::Key_S) {
        ui->widget->mVertMove += vel;
    }
    else if (event->key() == Qt::Key_E) {
        ui->widget->mDepthMove += vel;
    }
    else if (event->key() == Qt::Key_Q) {
        ui->widget->mDepthMove -= vel;
    }

    ui->widget->resizeGL(ui->widget->width(), ui->widget->height());
    ui->widget->updateGL();
}


void MainWindow::propertiesChanged()
{
    auto txt = QString("Gravity =  %1, dT = %2, Restitution = %3, Stiffness = %4, Damping = %5, Mass = %6")
                       .arg(ui->widget->mParticleSystem.mGravity)
                       .arg(ui->widget->mParticleSystem.mDeltaT)
                       .arg(ui->widget->mParticleSystem.mRestitution)
                       .arg(ui->widget->mParticleSystem.mStiffness)
                       .arg(ui->widget->mParticleSystem.mDamping)
                       .arg(ui->widget->mParticleSystem.mMass);
    ui->statusBar->showMessage(txt);
}


void MainWindow::createTetFromPly()
{
    ui->checkBox_simulate->setChecked(false);
    ui->widget->simulationStateChanged(Qt::Unchecked);
    QString fileNameNode = QFileDialog::getOpenFileName(this,
                                                        tr("Open node file"),
                                                        "",
                                                        tr("Node file (*.node);;All Files (*)"));
    QString fileNameEle = QFileDialog::getOpenFileName(this,
                                                       tr("Open ele file"),
                                                       "",
                                                       tr("Ele file (*.ele);;All Files (*)"));
    if (fileNameNode.isEmpty()) {
        return;
    }

    if (fileNameEle.isEmpty()) {
        return;
    }

    ui->widget->importTetgenFiles(fileNameNode, fileNameEle);
}


void MainWindow::editAccepted()
{
    ui->widget->mParticleSystem.initParticleSystem();
}
