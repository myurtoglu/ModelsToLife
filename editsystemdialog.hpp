#ifndef EDITSYSTEMDIALOG_HPP
#define EDITSYSTEMDIALOG_HPP

#include "vec.hpp"

#include <QDialog>
#include <QGLWidget>


#include <vector>


namespace Ui {
class EditSystemDialog;
}

class EditSystemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditSystemDialog(QWidget *parent = 0);
    ~EditSystemDialog();

    std::vector<Vec3f>            *mpVertices;
    std::vector<GLfloat>          *mpObjVertices;

private:
    void  setUpEditObjDialog();
    void  createConnections();
    Vec3f findCenter();

    Ui::EditSystemDialog           *ui;
    std::vector<GLfloat>           mObjVerticesInitPos;
    std::vector<Vec3f>             mVerticesInitPos;
    int                            mPrevXShift;
    int                            mPrevYShift;
    int                            mPrevZShift;
    int                            mPrevXRotate;
    int                            mPrevYRotate;
    int                            mPrevZRotate;

private slots:
    void transX(int state);
    void transY(int state);
    void transZ(int state);
    void rotateX(int state);
    void rotateY(int state);
    void rotateZ(int state);
    void sliderReleased();
    void sliderPressed();
    void stickObjectToTetsStateChanged(int state);

signals:
    void objectEdited();
    void stickObjectToTetsStateChangedSignal(int state);
};

#endif // EDITSYSTEMDIALOG_HPP
