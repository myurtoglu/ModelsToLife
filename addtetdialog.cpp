#include "addtetdialog.hpp"
#include "ui_addtetdialog.h"

#include <QtMath>

#include <algorithm>


AddTetDialog::AddTetDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::AddTetDialog), mPrevXShift(0.f), mPrevYShift(0.f),
    mPrevZShift(0.f), mPrevXRotate(0), mPrevYRotate(0), mPrevZRotate(0)
{
    ui->setupUi(this);
    setUpAddTetDialog();
    createConnections();
    createShortcutKeys();
}


AddTetDialog::~AddTetDialog()
{
    delete ui;
}


void AddTetDialog::setUpAddTetDialog()
{
    ui->lineEdit->setValidator(new QDoubleValidator(-100000, 100000, 5,
                                                    ui->lineEdit));
    ui->lineEdit_2->setValidator(new QDoubleValidator(-100000, 100000, 5,
                                                      ui->lineEdit_2));
    ui->lineEdit_3->setValidator(new QDoubleValidator(-100000, 100000, 5,
                                                      ui->lineEdit_3));

    ui->comboBox_9->addItem("From 3 verts + new");
    ui->comboBox_9->addItem("From 4 vertices");
    ui->comboBox_9->addItem("Just a new vertex");

    ui->comboBox_4->hide();
    ui->label_9->hide();

    ui->verticalSlider_x->setRange(-100, 100);
    ui->verticalSlider_y->setRange(-100, 100);
    ui->verticalSlider_z->setRange(-100, 100);
    ui->verticalSlider_x->setSingleStep(1);
    ui->verticalSlider_y->setSingleStep(1);
    ui->verticalSlider_z->setSingleStep(1);
    ui->verticalSlider_x->setPageStep(10);
    ui->verticalSlider_y->setPageStep(10);
    ui->verticalSlider_z->setPageStep(10);
    ui->verticalSlider_x->setTickInterval(10);
    ui->verticalSlider_y->setTickInterval(10);
    ui->verticalSlider_z->setTickInterval(10);
    ui->verticalSlider_x->setTickPosition(QSlider::TicksRight);
    ui->verticalSlider_y->setTickPosition(QSlider::TicksRight);
    ui->verticalSlider_z->setTickPosition(QSlider::TicksRight);

    ui->verticalSlider_x->hide();
    ui->verticalSlider_y->hide();
    ui->verticalSlider_z->hide();
    ui->checkBox_vertPrev->hide();
    ui->label_15->hide();
    ui->label_16->hide();
    ui->label_17->hide();
    ui->label_18->hide();

    ui->verticalSlider_vertEditX->setRange(-50, 50);
    ui->verticalSlider_vertEditY->setRange(-50, 50);
    ui->verticalSlider_vertEditZ->setRange(-50, 50);
    ui->verticalSlider_vertEditX->setSingleStep(1);
    ui->verticalSlider_vertEditY->setSingleStep(1);
    ui->verticalSlider_vertEditZ->setSingleStep(1);
    ui->verticalSlider_vertEditX->setPageStep(10);
    ui->verticalSlider_vertEditY->setPageStep(10);
    ui->verticalSlider_vertEditZ->setPageStep(10);

    ui->verticalSlider_transX->setRange(-100, 100);
    ui->verticalSlider_transY->setRange(-100, 100);
    ui->verticalSlider_transZ->setRange(-100, 100);
    ui->verticalSlider_transX->setSingleStep(1);
    ui->verticalSlider_transY->setSingleStep(1);
    ui->verticalSlider_transZ->setSingleStep(1);
    ui->verticalSlider_transX->setPageStep(10);
    ui->verticalSlider_transY->setPageStep(10);
    ui->verticalSlider_transZ->setPageStep(10);

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


void AddTetDialog::createConnections()
{
    connect(ui->pushButton_add, &QPushButton::clicked, this,
            &AddTetDialog::addVertex);
    connect(ui->pushButton_undo, &QPushButton::clicked, this,
            &AddTetDialog::undo);
    connect(ui->pushButton_removeVert, &QPushButton::clicked, this,
            &AddTetDialog::removeVertex);
    connect(ui->pushButton_removeTet, &QPushButton::clicked, this,
            &AddTetDialog::removeTet);
    connect(ui->comboBox_9, SIGNAL(currentIndexChanged(int)), this,
            SLOT(addVertexStateChanged(int)));
    connect(ui->checkBox_vertPrev, &QCheckBox::stateChanged, this,
            &AddTetDialog::previewVert);
    connect(ui->verticalSlider_x, &QSlider::valueChanged, this,
            &AddTetDialog::prevVertsetXVal);
    connect(ui->verticalSlider_y, &QSlider::valueChanged, this,
            &AddTetDialog::prevVertsetYVal);
    connect(ui->verticalSlider_z, &QSlider::valueChanged, this,
            &AddTetDialog::prevVertsetZVal);
    connect(ui->checkBox_sliderShow, &QCheckBox::stateChanged, this,
            &AddTetDialog::sliderShowStateChanged);
    connect(ui->verticalSlider_vertEditX, &QSlider::sliderMoved, this,
            &AddTetDialog::sliderMovedX);
    connect(ui->verticalSlider_vertEditY, &QSlider::sliderMoved, this,
            &AddTetDialog::sliderMovedY);
    connect(ui->verticalSlider_vertEditZ, &QSlider::sliderMoved, this,
            &AddTetDialog::sliderMovedZ);
    connect(ui->verticalSlider_transX, &QSlider::sliderMoved, this,
            &AddTetDialog::translateXVal);
    connect(ui->verticalSlider_transY, &QSlider::sliderMoved, this,
            &AddTetDialog::translateYVal);
    connect(ui->verticalSlider_transZ, &QSlider::sliderMoved, this,
            &AddTetDialog::translateZVal);
    connect(ui->verticalSlider_vertEditX, &QSlider::sliderReleased, this,
            &AddTetDialog::editSliderReleased);
    connect(ui->verticalSlider_vertEditY, &QSlider::sliderReleased, this,
            &AddTetDialog::editSliderReleased);
    connect(ui->verticalSlider_vertEditZ, &QSlider::sliderReleased, this,
            &AddTetDialog::editSliderReleased);
    connect(ui->verticalSlider_transX, &QSlider::sliderReleased, this,
            &AddTetDialog::editSliderReleased);
    connect(ui->verticalSlider_transY, &QSlider::sliderReleased, this,
            &AddTetDialog::editSliderReleased);
    connect(ui->verticalSlider_transZ, &QSlider::sliderReleased, this,
            &AddTetDialog::editSliderReleased);
    connect(ui->verticalSlider_vertEditX, &QSlider::sliderPressed, this,
            &AddTetDialog::editSliderPressed);
    connect(ui->verticalSlider_vertEditY, &QSlider::sliderPressed, this,
            &AddTetDialog::editSliderPressed);
    connect(ui->verticalSlider_vertEditZ, &QSlider::sliderPressed, this,
            &AddTetDialog::editSliderPressed);
    connect(ui->verticalSlider_transX, &QSlider::sliderPressed, this,
            &AddTetDialog::editSliderPressed);
    connect(ui->verticalSlider_transY, &QSlider::sliderPressed, this,
            &AddTetDialog::editSliderPressed);
    connect(ui->verticalSlider_transZ, &QSlider::sliderPressed, this,
            &AddTetDialog::editSliderPressed);

    connect(ui->verticalSlider_rotateX, &QSlider::sliderMoved, this,
            &AddTetDialog::rotateX);
    connect(ui->verticalSlider_rotateY, &QSlider::sliderMoved, this,
            &AddTetDialog::rotateY);
    connect(ui->verticalSlider_rotateZ, &QSlider::sliderMoved, this,
            &AddTetDialog::rotateZ);
    connect(ui->verticalSlider_rotateX, &QSlider::sliderReleased, this,
            &AddTetDialog::editSliderReleased);
    connect(ui->verticalSlider_rotateX, &QSlider::sliderPressed, this,
            &AddTetDialog::editSliderPressed);
    connect(ui->verticalSlider_rotateY, &QSlider::sliderReleased, this,
            &AddTetDialog::editSliderReleased);
    connect(ui->verticalSlider_rotateY, &QSlider::sliderPressed, this,
            &AddTetDialog::editSliderPressed);
    connect(ui->verticalSlider_rotateZ, &QSlider::sliderReleased, this,
            &AddTetDialog::editSliderReleased);
    connect(ui->verticalSlider_rotateZ, &QSlider::sliderPressed, this,
            &AddTetDialog::editSliderPressed);
}


void AddTetDialog::createShortcutKeys()
{
    ui->pushButton_undo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
}


void AddTetDialog::updateLists()
{
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    ui->listWidget->clear();
    ui->listWidget_tets->clear();

    std::vector<QString> tetVerticesStrings;
    for (auto it = (*mpVertices).begin(); it != (*mpVertices).end(); ++it) {
        int pos = it - (*mpVertices).begin();
        auto txt = QString("%1: (x: %2, y: %3, z: %4)").arg(pos)
                                                       .arg((*it)[0])
                                                       .arg((*it)[1])
                                                       .arg((*it)[2]);
        tetVerticesStrings.push_back(txt);

        ui->comboBox->addItem(QString::number(pos));
        ui->comboBox_2->addItem(QString::number(pos));
        ui->comboBox_3->addItem(QString::number(pos));
        ui->comboBox_4->addItem(QString::number(pos));
    }

    for (const auto &el : tetVerticesStrings) {
        ui->listWidget->addItem(el);
    }

    std::vector<QString> tetTetsStrings;
    for (auto it = (*mpTets).begin(); it != (*mpTets).end(); ++it) {
        int pos = it - (*mpTets).begin();
        auto txt = QString("%1: (%2, %3, %4, %5)").arg(pos)
                                                  .arg((*it)[0])
                                                  .arg((*it)[1])
                                                  .arg((*it)[2])
                                                  .arg((*it)[3]);
        tetTetsStrings.push_back(txt);
    }

    for (const auto &el : tetTetsStrings) {
        ui->listWidget_tets->addItem(el);
    }
}


void AddTetDialog::addVertex()
{
    auto idx0 = ui->comboBox->currentIndex();
    auto idx1 = ui->comboBox_2->currentIndex();
    auto idx2 = ui->comboBox_3->currentIndex();
    if (((idx0 == idx1) || (idx1 == idx2) || (idx0 == idx2)) &&
        ui->comboBox_9->currentIndex() != 2) {
        return;
    }

    mPrevVertices.push(*mpVertices);
    mPrevTets.push(*mpTets);

    auto np = Vec3f(ui->lineEdit->text().toFloat(),
                    ui->lineEdit_2->text().toFloat(),
                    ui->lineEdit_3->text().toFloat());

    if (ui->comboBox_9->currentIndex() == 0) {
        mpVertices->push_back(np);
        mpTets->push_back(std::vector<int>{idx0, idx1, idx2,
                                           static_cast<int>(mpVertices->size())-1});
    }
    else if (ui->comboBox_9->currentIndex() == 1) {
        auto idx3 = ui->comboBox_4->currentIndex();

        if ((idx3 == idx0) || (idx3 == idx1) || (idx3 == idx2)) {
            return;
        }
        mpTets->push_back(std::vector<int>{idx0, idx1, idx2, idx3});
    }
    else if (ui->comboBox_9->currentIndex() == 2) {
        if (std::find(mpVertices->cbegin(),
                      mpVertices->cend(), np) != mpVertices->cend()) {
            return;
        }
        else {
            mpVertices->push_back(np);
        }
    }

    updateLists();
    emit tetsEdited();
}


void AddTetDialog::addVertexStateChanged(int state)
{
    if (state == 0){
        //comboboxes
        ui->comboBox->show();
        ui->comboBox_2->show();
        ui->comboBox_3->show();
        ui->comboBox_4->hide();

        //combobox labels
        ui->label_6->show();
        ui->label_7->show();
        ui->label_8->show();
        ui->label_9->hide();

        // lineedits
        ui->lineEdit->show();
        ui->lineEdit_2->show();
        ui->lineEdit_3->show();

        // lineedits labels
        ui->label_3->show();
        ui->label_4->show();
        ui->label_5->show();
    }
    else if (state == 1) {
        //comboboxes
        ui->comboBox->show();
        ui->comboBox_2->show();
        ui->comboBox_3->show();
        ui->comboBox_4->show();

        //combobox labels
        ui->label_6->show();
        ui->label_7->show();
        ui->label_8->show();
        ui->label_9->show();

         // lineedits
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ui->lineEdit_3->hide();

        // lineedits labels
        ui->label_3->hide();
        ui->label_4->hide();
        ui->label_5->hide();
    }
    else {
        //comboboxes
        ui->comboBox->hide();
        ui->comboBox_2->hide();
        ui->comboBox_3->hide();
        ui->comboBox_4->hide();

        //combobox labels
        ui->label_6->hide();
        ui->label_7->hide();
        ui->label_8->hide();
        ui->label_9->hide();

        // lineedits
        ui->lineEdit->show();
        ui->lineEdit_2->show();
        ui->lineEdit_3->show();

        // lineedits labels
        ui->label_3->show();
        ui->label_4->show();
        ui->label_5->show();
    }
}


void AddTetDialog::undo()
{
    if (mPrevVertices.size() == 0) {
        return;
    }
    *mpVertices = mPrevVertices.top();
    mPrevVertices.pop();
    *mpTets = mPrevTets.top();
    mPrevTets.pop();
    updateLists();
    emit tetsEdited();
}


void AddTetDialog::removeVertex()
{
    auto idx = ui->listWidget->selectionModel()->currentIndex().row();
    if (idx < 0) {
        return;
    }

    mPrevVertices.push(*mpVertices);
    mPrevTets.push(*mpTets);

    mpVertices->erase(mpVertices->begin()+idx);
    mpTets->erase(std::remove_if(mpTets->begin(), mpTets->end(),
                  [idx](std::vector<int> v)
                  {
                      return std::find(v.begin(), v.end(), idx)!= v.end();
                  }), mpTets->end());

    for (auto it = mpTets->begin(); it != mpTets->end(); ++it) {
        for (auto &el : *it) {
            if (el >= idx) { --el; }
        }
    }

    updateLists();
    emit tetsEdited();
}


void AddTetDialog::removeTet()
{
    auto idx = ui->listWidget_tets->selectionModel()->currentIndex().row();
    if (idx < 0) {
        return;
    }

    mPrevVertices.push(*mpVertices);
    mPrevTets.push(*mpTets);

    mpTets->erase(mpTets->begin()+idx);

    updateLists();
    emit tetsEdited();
}


void AddTetDialog::previewVert(int state)
{
    emit previewVertStateChanged(state);
}


void AddTetDialog::prevVertsetXVal(int state)
{
    float xPos = 0.05f * state;
    (*mpPreviewVertPos)[0] = xPos;
    ui->lineEdit->setText(QString::number(xPos));
    emit tetsEdited();
}


void AddTetDialog::prevVertsetYVal(int state)
{
    float yPos = 0.05f * state;
    (*mpPreviewVertPos)[1] = yPos;
    ui->lineEdit_2->setText(QString::number(yPos));
    emit tetsEdited();
}


void AddTetDialog::prevVertsetZVal(int state)
{
    float zPos = 0.05f * state;
    (*mpPreviewVertPos)[2] = zPos;
    ui->lineEdit_3->setText(QString::number(zPos));
    emit tetsEdited();
}


void AddTetDialog::sliderShowStateChanged(int state)
{
    if (state == Qt::Checked) {
        ui->verticalSlider_x->show();
        ui->verticalSlider_y->show();
        ui->verticalSlider_z->show();
        ui->checkBox_vertPrev->show();
        ui->label_15->show();
        ui->label_16->show();
        ui->label_17->show();
        ui->label_18->show();
    }
    else {
        ui->verticalSlider_x->hide();
        ui->verticalSlider_y->hide();
        ui->verticalSlider_z->hide();
        ui->checkBox_vertPrev->hide();
        ui->label_15->hide();
        ui->label_16->hide();
        ui->label_17->hide();
        ui->label_18->hide();
    }
}


void AddTetDialog::editSliderReleased()
{
    mPrevXShift = ui->verticalSlider_transX->value();
    mPrevYShift = ui->verticalSlider_transY->value();
    mPrevZShift = ui->verticalSlider_transZ->value();
    ui->verticalSlider_vertEditX->setValue(0);
    ui->verticalSlider_vertEditY->setValue(0);
    ui->verticalSlider_vertEditZ->setValue(0);
    mPrevXRotate = ui->verticalSlider_rotateX->value();
    mPrevYRotate = ui->verticalSlider_rotateY->value();
    mPrevZRotate = ui->verticalSlider_rotateZ->value();
}


void AddTetDialog::editSliderPressed()
{
    mVerticesInitPos = *mpVertices;
    mPrevVertices.push(*mpVertices);
    mPrevTets.push(*mpTets);
}


void AddTetDialog::sliderMovedX(int state)
{
    float xMoved = 0.01f * state;
    auto idx = ui->listWidget->selectionModel()->currentIndex().row();

    if (idx < 0) {
        return;
    }

    (*mpVertices)[idx][0] = mVerticesInitPos[idx][0] + xMoved;

    auto txt = QString("%1: (x: %2, y: %3, z: %4)").arg(idx)
                                                   .arg((*mpVertices)[idx][0])
                                                   .arg((*mpVertices)[idx][1])
                                                   .arg((*mpVertices)[idx][2]);
    ui->listWidget->item(idx)->setText(txt);
    emit tetsEdited();
}


void AddTetDialog::sliderMovedY(int state)
{
    float yMoved = 0.01f * state;
    auto idx = ui->listWidget->selectionModel()->currentIndex().row();

    if (idx < 0) {
        return;
    }

    (*mpVertices)[idx][1] = mVerticesInitPos[idx][1] + yMoved;

    auto txt = QString("%1: (x: %2, y: %3, z: %4)").arg(idx)
                                                   .arg((*mpVertices)[idx][0])
                                                   .arg((*mpVertices)[idx][1])
                                                   .arg((*mpVertices)[idx][2]);
    ui->listWidget->item(idx)->setText(txt);
    emit tetsEdited();
}


void AddTetDialog::sliderMovedZ(int state)
{
    float zMoved = 0.01f * state;
    auto idx = ui->listWidget->selectionModel()->currentIndex().row();

    if (idx < 0) {
        return;
    }

    (*mpVertices)[idx][2] = mVerticesInitPos[idx][2] + zMoved;

    auto txt = QString("%1: (x: %2, y: %3, z: %4)").arg(idx)
                                                   .arg((*mpVertices)[idx][0])
                                                   .arg((*mpVertices)[idx][1])
                                                   .arg((*mpVertices)[idx][2]);
    ui->listWidget->item(idx)->setText(txt);
    emit tetsEdited();
}


void AddTetDialog::translateXVal(int state)
{
    auto xShift = 0.05f * (state - mPrevXShift);
    for (unsigned i = 0; i < mpVertices->size(); ++i) {
        (*mpVertices)[i][0] = mVerticesInitPos[i][0] + xShift;
    }
    updateLists();
    emit tetsEdited();
}


void AddTetDialog::translateYVal(int state)
{
    auto yShift = 0.05f * (state - mPrevYShift);
    for (unsigned i = 0; i < mpVertices->size(); ++i) {
        (*mpVertices)[i][1] = mVerticesInitPos[i][1] + yShift;
    }
    updateLists();
    emit tetsEdited();
}


void AddTetDialog::translateZVal(int state)
{
    auto zShift = 0.05f * (state - mPrevZShift);
    for (unsigned i = 0; i < mpVertices->size(); ++i) {
        (*mpVertices)[i][2] = mVerticesInitPos[i][2] + zShift;
    }
    updateLists();
    emit tetsEdited();
}


void AddTetDialog::rotateX(int state)
{
    auto xRotate = 3.f * (state - mPrevXRotate);
    auto theta = qDegreesToRadians(xRotate);
    auto centerVec = findCenter();

    for (unsigned i = 0; i < mpVertices->size(); ++i) {
        auto posVec = mVerticesInitPos[i] - centerVec;
        auto newVec = Vec3f(posVec[0],
                            posVec[1]*qCos(theta)-posVec[2]*qSin(theta),
                            posVec[1]*qSin(theta)+posVec[2]*qCos(theta))
                           + centerVec;
        (*mpVertices)[i] = newVec;
    }
    updateLists();
    emit tetsEdited();
}


void AddTetDialog::rotateY(int state)
{
    auto yRotate = 3.f * (state - mPrevYRotate);
    auto theta = qDegreesToRadians(yRotate);
    auto centerVec = findCenter();

    for (unsigned i = 0; i < mpVertices->size(); ++i) {
        auto posVec = mVerticesInitPos[i] - centerVec;
        auto newVec = Vec3f(posVec[0]*qCos(theta) + posVec[2]*qSin(theta),
                            posVec[1],
                            -posVec[0]*qSin(theta) + posVec[2]*qCos(theta))
                           + centerVec;
        (*mpVertices)[i] = newVec;
    }
    updateLists();
    emit tetsEdited();
}


void AddTetDialog::rotateZ(int state)
{
    auto zRotate = 3.f * (state - mPrevZRotate);
    auto theta = qDegreesToRadians(zRotate);
    auto centerVec = findCenter();

    for (unsigned i = 0; i < mpVertices->size(); ++i) {
        auto posVec = mVerticesInitPos[i] - centerVec;
        auto newVec = Vec3f(posVec[0]*qCos(theta) - posVec[1]*qSin(theta),
                            posVec[0]*qSin(theta) + posVec[1]*qCos(theta),
                            posVec[2]) + centerVec;
        (*mpVertices)[i] = newVec;
    }
    updateLists();
    emit tetsEdited();
}


Vec3f AddTetDialog::findCenter()
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
