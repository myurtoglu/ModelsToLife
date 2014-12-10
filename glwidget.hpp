#ifndef GLWIDGET_HPP
#define GLWIDGET_HPP

#include "vec.hpp"
#include "particlesystem.hpp"

#include <assimp/Importer.hpp>

#include <QGLWidget>
#include <QTimer>

#include <vector>


class GLWidget : public QGLWidget {
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    void resizeGL(int w, int h);
    void loadModel(const QString &qFName);
    void openTetFile(const QString &fileName);
    void saveToFile(const QString &fileName);
    void saveAllToFile(const QString &fileName);
    void openAllFromFile(const QString &fileName);
    void importTetgenFiles(const QString &fileNameNode,
                           const QString &fileNameEle);

    std::vector<Vec3f>            mVertices;
    std::vector<Vec3f>            mVerticesInitPos;
    std::vector<std::vector<int>> mTets;
    std::vector<std::vector<int>> mBones;
    float                         mHorMove;
    float                         mVertMove;
    float                         mDepthMove;
    std::vector<GLfloat>          mObjVertices;
    Vec3f                         mPreviewVertPos;
    ParticleSystem                mParticleSystem;

    typedef struct
    {
        Vec4f m[3];
    } float3x4;

    Vec3f mul(const float3x4 &M, const Vec3f &v)
    {
        Vec3f r;
        r[0] = (v * Vec3f(M.m[0][0], M.m[0][1], M.m[0][2]));
        r[1] = (v * Vec3f(M.m[1][0], M.m[1][1], M.m[1][2]));
        r[2] = (v * Vec3f(M.m[2][0], M.m[2][1], M.m[2][2]));
        return r;
    }

private:
    void initializeGL();
    void paintGL();
    void drawRectFace(const Vec3f&, const Vec3f&, const Vec3f&, const Vec3f&);
    void drawTriFace(const Vec3f&, const Vec3f&, const Vec3f&);
    void drawTet(const Vec3f&, const Vec3f&, const Vec3f&, const Vec3f&);
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
    void ybox(float h);
    void drawLabel(const char* format , ...);
    void mouseReleaseEvent(QMouseEvent *event) ;
    int selection(int x, int y);

    std::vector<GLfloat>          mObjVerticesInitPos;
    std::vector<GLfloat>          mObjNormals;
    std::vector<GLuint>           mObjIndices;
    bool                          showVerts;
    bool                          showTets;
    bool                          showAxes;
    bool                          showObject;
    bool                          showSkeleton;
    bool                          showGround;
    int                           xRot;
    int                           yRot;
    int                           zRot;
    QPoint                        lastPos;
    float                         zoomFactor;
    bool                          previewVertState;
    bool                          mStickTetState;
    bool                          mBoneStickTetState;
    Assimp::Importer              importer;
    bool                          simulate;
    QTimer                        mTimerSim;
    QTimer                        mTimerVis;
    std::vector<Vec3f>            mVerticesBackup;
    std::vector<std::vector<int>> mTetsBackup;
    std::vector<std::vector<int>> mBonesBackup;
    std::vector<GLfloat>          mObjVerticesBackup;
    std::vector<GLfloat>          mObjNormalsBackup;
    std::vector<GLuint>           mObjIndicesBackup;
    bool                          mMidbutton;
    GLint                         viewport[4];
    GLdouble                      modelview[16];
    GLdouble                      projection[16];
    int                           whichtetvertice;

public slots:
    void showVertsStateChanged(int state);
    void showTetsStateChanged(int state);
    void showAxesStateChanged(int state);
    void showObjectStateChanged(int state);
    void showSkeletonStateChanged(int state);
    void showGroundStateChanged(int state);
    void tetsEdited();
    void objectEdited();
    void bonesEdited();
    void previewVertStateChanged(int state);
    void simulationStateChanged(int state);
    void resetSim();
    void stickObjectToTetsStateChanged(int state);
    void boneStickTetStateChanged(int state);


private slots:
    void updateSimulation();
    void updateSimulationGL();
};

#endif // GLWIDGET_HPP
