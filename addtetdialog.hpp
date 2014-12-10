#ifndef ADDTETDIALOG_HPP
#define ADDTETDIALOG_HPP

#include "vec.hpp"

#include <QDialog>

#include <stack>
#include <vector>


namespace Ui {
class AddTetDialog;
}

class AddTetDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddTetDialog(QWidget *parent = 0);
    ~AddTetDialog();
    void updateLists();

    std::vector<Vec3f>            *mpVertices;
    std::vector<std::vector<int>> *mpTets;
    Vec3f                         *mpPreviewVertPos;

private slots:
    void addVertex();
    void addVertexStateChanged(int state);
    void undo();
    void removeVertex();
    void removeTet();
    void previewVert(int state);
    void prevVertsetXVal(int state);
    void prevVertsetYVal(int state);
    void prevVertsetZVal(int state);
    void sliderShowStateChanged(int state);
    void sliderMovedX(int state);
    void sliderMovedY(int state);
    void sliderMovedZ(int state);
    void translateXVal(int state);
    void translateYVal(int state);
    void translateZVal(int state);
    void editSliderReleased();
    void editSliderPressed();
    void rotateX(int state);
    void rotateY(int state);
    void rotateZ(int state);

signals:
    void tetsEdited();
    void previewVertStateChanged(int state);

private:
    void  setUpAddTetDialog();
    void  createConnections();
    void  createShortcutKeys();
    Vec3f findCenter();

    Ui::AddTetDialog                          *ui;
    std::stack<std::vector<Vec3f>>            mPrevVertices;
    std::stack<std::vector<std::vector<int>>> mPrevTets;
    float                                     mPrevXShift;
    float                                     mPrevYShift;
    float                                     mPrevZShift;
    std::vector<Vec3f>                        mVerticesInitPos;
    int                                       mPrevXRotate;
    int                                       mPrevYRotate;
    int                                       mPrevZRotate;
};

#endif // ADDTETDIALOG_HPP
