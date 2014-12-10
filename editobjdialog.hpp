#ifndef EDITOBJDIALOG_HPP
#define EDITOBJDIALOG_HPP

#include "vec.hpp"

#include <QDialog>
#include <QGLWidget>

namespace Ui {
class EditObjDialog;
}


class EditObjDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditObjDialog(QWidget *parent = 0);
    ~EditObjDialog();

    std::vector<GLfloat> *mpObjVertices;

private:
    void  setUpEditObjDialog();
    void  createConnections();
    Vec3f findCenter();

    Ui::EditObjDialog    *ui;
    std::vector<GLfloat> mObjVerticesInitPos;
    int                  mPrevXShift;
    int                  mPrevYShift;
    int                  mPrevZShift;
    float                mPrevScale;
    int                  mPrevXRotate;
    int                  mPrevYRotate;
    int                  mPrevZRotate;

private slots:
    void translateXVal(int state);
    void translateYVal(int state);
    void translateZVal(int state);
    void scale(int state);
    void sliderReleased();
    void sliderPressed();
    void rotateX(int state);
    void rotateY(int state);
    void rotateZ(int state);

signals:
    void objectEdited();
};

#endif // EDITOBJDIALOG_HPP
