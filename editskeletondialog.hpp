#ifndef EDITSKELETONDIALOG_HPP
#define EDITSKELETONDIALOG_HPP

#include "vec.hpp"

#include <QDialog>


namespace Ui {
class EditSkeletonDialog;
}

class EditSkeletonDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditSkeletonDialog(QWidget *parent = 0);
    ~EditSkeletonDialog();
    void updateLists();

    std::vector<Vec3f>            *mpVertices;
    std::vector<std::vector<int>> *mpBones;

private:
    Ui::EditSkeletonDialog *ui;
    std::vector<Vec3f>     mVerticesInitPos;
    std::vector<int>       mJoints;
    std::vector<int>       mDirs;
    int                    mPrevXMove;
    int                    mPrevYMove;
    int                    mPrevZMove;

private slots:
    void addBone();
    void jointChanged(int state);
    void moveX(int state);
    void moveY(int state);
    void moveZ(int state);
    void sliderReleased();
    void sliderPressed();
    void removeBone();
    void stickBonesStateChanged(int state);

signals:
    void bonesEdited();
    void stickBonesStateChangedSignal(int state);
};

#endif // EDITSKELETONDIALOG_HPP
