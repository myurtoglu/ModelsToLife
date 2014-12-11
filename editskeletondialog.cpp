#include "editskeletondialog.hpp"
#include "ui_editskeletondialog.h"

#include <QDebug>
#include <QString>
#include <QtMath>

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>


EditSkeletonDialog::EditSkeletonDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::EditSkeletonDialog), mPrevXMove(0), mPrevYMove(0),
    mPrevZMove(0)
{
    ui->setupUi(this);

    connect(ui->pushButton_addBone, &QPushButton::clicked, this,
            &EditSkeletonDialog::addBone);
    connect(ui->comboBox_joint, SIGNAL(currentIndexChanged(int)), this,
            SLOT(jointChanged(int)));
    connect(ui->verticalSlider_x, &QSlider::sliderMoved, this,
            &EditSkeletonDialog::moveX);
    connect(ui->verticalSlider_y, &QSlider::sliderMoved, this,
            &EditSkeletonDialog::moveY);
    connect(ui->verticalSlider_z, &QSlider::sliderMoved, this,
            &EditSkeletonDialog::moveZ);
    connect(ui->pushButton_removeBone, &QPushButton::clicked, this,
            &EditSkeletonDialog::removeBone);

    ui->verticalSlider_x->setRange(-60, 60);
    ui->verticalSlider_y->setRange(-60, 60);
    ui->verticalSlider_z->setRange(-60, 60);
    ui->verticalSlider_x->setSingleStep(1);
    ui->verticalSlider_y->setSingleStep(1);
    ui->verticalSlider_z->setSingleStep(1);
    ui->verticalSlider_x->setPageStep(10);
    ui->verticalSlider_y->setPageStep(10);
    ui->verticalSlider_z->setPageStep(10);

    connect(ui->verticalSlider_x, &QSlider::sliderReleased, this,
            &EditSkeletonDialog::sliderReleased);
    connect(ui->verticalSlider_x, &QSlider::sliderPressed, this,
            &EditSkeletonDialog::sliderPressed);
    connect(ui->verticalSlider_y, &QSlider::sliderReleased, this,
            &EditSkeletonDialog::sliderReleased);
    connect(ui->verticalSlider_y, &QSlider::sliderPressed, this,
            &EditSkeletonDialog::sliderPressed);
    connect(ui->verticalSlider_z, &QSlider::sliderReleased, this,
            &EditSkeletonDialog::sliderReleased);
    connect(ui->verticalSlider_z, &QSlider::sliderPressed, this,
            &EditSkeletonDialog::sliderPressed);

    ui->checkBox_stickBones->setChecked(true);

    connect(ui->checkBox_stickBones, &QCheckBox::stateChanged, this,
            &EditSkeletonDialog::stickBonesStateChanged);
}


EditSkeletonDialog::~EditSkeletonDialog()
{
    delete ui;
}


void EditSkeletonDialog::sliderReleased()
{
    mPrevXMove = ui->verticalSlider_x->value();
    mPrevYMove = ui->verticalSlider_y->value();
    mPrevZMove = ui->verticalSlider_z->value();
}


void EditSkeletonDialog::sliderPressed()
{
    mVerticesInitPos = *mpVertices;
}


void EditSkeletonDialog::updateLists()
{
    ui->comboBox_v1->clear();
    ui->comboBox_v2->clear();
    ui->listWidget_vertices->clear();
    ui->listWidget_bones->clear();

    std::vector<QString> vertexStrings;
    for (auto it = (*mpVertices).begin(); it != (*mpVertices).end(); ++it) {
        int pos = it - (*mpVertices).begin();
        auto txt = QString("%1: (x: %2, y: %3, z: %4)").arg(pos)
                                                       .arg((*it)[0])
                                                       .arg((*it)[1])
                                                       .arg((*it)[2]);
        vertexStrings.push_back(txt);

        ui->comboBox_v1->addItem(QString::number(pos));
        ui->comboBox_v2->addItem(QString::number(pos));

    }

    for (const auto &el : vertexStrings) {
        ui->listWidget_vertices->addItem(el);
    }

    std::vector<QString> boneStrings;
    for (auto it = (*mpBones).begin(); it != (*mpBones).end(); ++it) {
        int pos = it - (*mpBones).begin();
        auto txt = QString("%1: (%2, %3)").arg(pos).arg((*it)[0])
                                                   .arg((*it)[1]);
        boneStrings.push_back(txt);
    }

    for (const auto &el : boneStrings) {
        ui->listWidget_bones->addItem(el);
    }
}


void EditSkeletonDialog::addBone()
{
    auto idx0 = ui->comboBox_v1->currentIndex();
    auto idx1 = ui->comboBox_v2->currentIndex();

    if (idx0 == idx1) {
        return;
    }

    bool idx0alreadyAdded = false;
    bool idx1alreadyAdded = false;

    for (const auto &el : *mpBones) {
        if ((el[0] == idx0) || (el[1] == idx0)) {
            idx0alreadyAdded = true;
        }

        if ((el[0] == idx1) || (el[1] == idx1)) {
            idx1alreadyAdded = true;
        }
    }

    mpBones->push_back(std::vector<int>{idx0, idx1});



    if (!idx0alreadyAdded) {
        mJoints.push_back(idx0);
    }

    if (!idx1alreadyAdded) {
        mJoints.push_back(idx1);
    }

    ui->comboBox_joint->clear();
    for (const auto &el : mJoints) {
        ui->comboBox_joint->addItem(QString::number(el));
    }

    updateLists();
    emit bonesEdited();
}


void EditSkeletonDialog::jointChanged(int state)
{
    auto idx   = ui->comboBox_joint->currentIndex();
    auto joint = mJoints[idx];
    mDirs.clear();

    for (const auto &el : *mpBones) {
        if(el[0] == joint) {
            mDirs.push_back(el[1]);
        }
        if (el[1] == joint) {
            mDirs.push_back(el[0]);
        }
    }

    ui->comboBox_dir->clear();
    for (const auto &el : mDirs) {
        ui->comboBox_dir->addItem(QString::number(el));
    }
}


void EditSkeletonDialog::moveX(int state)
{
    if (mpBones->size() == 0) {
        return;
    }

    auto xRotate   = 3.f * (state - mPrevXMove);
    auto theta     = qDegreesToRadians(xRotate);
    auto joint     = mJoints[ui->comboBox_joint->currentIndex()];
    auto dirn      = mDirs[ui->comboBox_dir->currentIndex()];
    std::set<int> editedVerts;
    editedVerts.insert(dirn);

    while (true) {
        bool added = false;
        for (const auto &el : editedVerts) {
            for (const auto &bone : *mpBones) {
                if(bone[0] == el && bone[1] != joint
                                 && (editedVerts.count(bone[1]) == 0)) {
                    editedVerts.insert(bone[1]);
                    added = true;
                }
                if (bone[1] == el && bone[0] != joint
                                  && (editedVerts.count(bone[0]) == 0)) {
                    editedVerts.insert(bone[0]);
                    added = true;
                }
            }
        }
        if (!added) {
            break;
        }
    }

    for (auto &el: editedVerts) {
        auto posVec = mVerticesInitPos[el] - mVerticesInitPos[joint];
        auto newVec = Vec3f(posVec[0],
                            posVec[1]*qCos(theta)-posVec[2]*qSin(theta),
                            posVec[1]*qSin(theta)+posVec[2]*qCos(theta))
                           + mVerticesInitPos[joint];
        (*mpVertices)[el] = newVec;
    }

    updateLists();
    emit bonesEdited();
}


void EditSkeletonDialog::moveY(int state)
{
    if (mpBones->size() == 0) {
        return;
    }

    auto yRotate   = 3.f * (state - mPrevYMove);
    auto theta     = qDegreesToRadians(yRotate);
    auto joint     = mJoints[ui->comboBox_joint->currentIndex()];
    auto dirn      = mDirs[ui->comboBox_dir->currentIndex()];
    std::set<int> editedVerts;
    editedVerts.insert(dirn);

    while (true) {
        bool added = false;
        for (const auto &el : editedVerts) {
            for (const auto &bone : *mpBones) {
                if(bone[0] == el && bone[1] != joint
                                 && (editedVerts.count(bone[1]) == 0)) {
                    editedVerts.insert(bone[1]);
                    added = true;
                }
                if (bone[1] == el && bone[0] != joint
                                  && (editedVerts.count(bone[0]) == 0)) {
                    editedVerts.insert(bone[0]);
                    added = true;
                }
            }
        }
        if (!added) {
            break;
        }
    }

    for (auto &el: editedVerts) {
        auto posVec = mVerticesInitPos[el] - mVerticesInitPos[joint];
        auto newVec = Vec3f(posVec[0]*qCos(theta) + posVec[2]*qSin(theta),
                            posVec[1],
                            -posVec[0]*qSin(theta) + posVec[2]*qCos(theta))
                            + mVerticesInitPos[joint];

        (*mpVertices)[el] = newVec;
    }

    updateLists();
    emit bonesEdited();
}


void EditSkeletonDialog::moveZ(int state)
{
    if (mpBones->size() == 0) {
        return;
    }

    auto zRotate   = 3.f * (state - mPrevZMove);
    auto theta     = qDegreesToRadians(zRotate);
    auto joint     = mJoints[ui->comboBox_joint->currentIndex()];
    auto dirn      = mDirs[ui->comboBox_dir->currentIndex()];
    std::set<int> editedVerts;
    editedVerts.insert(dirn);

    while (true) {
        bool added = false;
        for (const auto &el : editedVerts) {
            for (const auto &bone : *mpBones) {
                if(bone[0] == el && bone[1] != joint
                                 && (editedVerts.count(bone[1]) == 0)) {
                    editedVerts.insert(bone[1]);
                    added = true;
                }
                if (bone[1] == el && bone[0] != joint
                                  && (editedVerts.count(bone[0]) == 0)) {
                    editedVerts.insert(bone[0]);
                    added = true;
                }
            }
        }
        if (!added) {
            break;
        }
    }

    for (auto &el: editedVerts) {
        auto posVec = mVerticesInitPos[el] - mVerticesInitPos[joint];
        auto newVec = Vec3f(posVec[0]*qCos(theta) - posVec[1]*qSin(theta),
                            posVec[0]*qSin(theta) + posVec[1]*qCos(theta),
                            posVec[2]) + mVerticesInitPos[joint];
        (*mpVertices)[el] = newVec;
    }

    updateLists();
    emit bonesEdited();
}


void EditSkeletonDialog::removeBone()
{
    auto idx = ui->listWidget_bones->selectionModel()->currentIndex().row();
    if (idx < 0) {
        return;
    }

    auto remBone = *(mpBones->begin()+idx);

    mpBones->erase(mpBones->begin()+idx);

    bool removeBone0 = true;
    bool removeBone1 = true;

    for (const auto &el : *mpBones) {
        if ((el[0] == remBone[0]) || (el[1] == remBone[0])) {
            removeBone0 = false;
        }
        if ((el[0] == remBone[1]) || (el[1] == remBone[1])) {
            removeBone1 = false;
        }
    }

    if (removeBone0) {
        auto it = std::find(mJoints.begin(), mJoints.end(), remBone[0]);
        mJoints.erase(it);
    }

    if (removeBone1) {
        auto it = std::find(mJoints.begin(), mJoints.end(), remBone[1]);
        mJoints.erase(it);
    }

    ui->comboBox_joint->clear();
    for (const auto &el : mJoints) {
        ui->comboBox_joint->addItem(QString::number(el));
    }

    updateLists();
    emit bonesEdited();
}


void EditSkeletonDialog::stickBonesStateChanged(int state)
{
    emit stickBonesStateChangedSignal(state);
}


void EditSkeletonDialog::updateJoints()
{
    mJoints.clear();

    for (const auto &el : *mpBones) {
        int idx0 = el[0];
        int idx1 = el[1];

        bool idx0alreadyAdded = false;
        bool idx1alreadyAdded = false;

        for (const auto &el : mJoints) {
            if (el == idx0) {
                idx0alreadyAdded = true;
            }

            if (el == idx1) {
                idx1alreadyAdded = true;
            }
        }

        if (!idx0alreadyAdded) {
            mJoints.push_back(idx0);
        }

        if (!idx1alreadyAdded) {
            mJoints.push_back(idx1);
        }
    }

    ui->comboBox_joint->clear();
    for (const auto &el : mJoints) {
        ui->comboBox_joint->addItem(QString::number(el));
    }
}
