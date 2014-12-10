#include "glwidget.hpp"
#include "mainwindow.hpp"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <QApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <iostream>

GLWidget::GLWidget(QWidget *parent): QGLWidget(parent), mHorMove(0.f),
    mVertMove(0.f), mDepthMove(0.f), showVerts(true), showTets(true),
    showAxes(true), showObject(true), showSkeleton(false), showGround(true),
    xRot(0), yRot(0), zRot(0), zoomFactor(1.f), previewVertState(false),
    mStickTetState(false), mBoneStickTetState(true), simulate(false), mMidbutton(false)
{
    mVertices = {Vec3f(0.f, 2.f, 1.f), Vec3f(0.f, 1.f, 0.f),
                 Vec3f(1.f, 2.f, 1.f), Vec3f(0.f, 1.f, 1.f),
                 Vec3f(1.f, 1.f, 1.f), Vec3f(1.f, 2.f, 0.f),
                 Vec3f(1.f, 1.f, 0.f), Vec3f(0.f, 2.f, 0.f)};

    mTets = {{0, 1, 4, 3}, {0, 1, 5, 6}, {1, 7, 0, 5}, {1, 0, 4, 6},
             {4, 0, 2, 6}, {2, 0, 5, 6}};

    mBones = {};

    mPreviewVertPos = Vec3f(0.f, 0.f, 0.f);

    connect(&mTimerSim, &QTimer::timeout, this, &GLWidget::updateSimulation);
    connect(&mTimerVis, &QTimer::timeout, this, &GLWidget::updateSimulationGL);
    mTimerSim.start(16);
    mTimerVis.start(16);

    mParticleSystem.mpVertices    = &mVertices;
    mParticleSystem.mpTets        = &mTets;
    mParticleSystem.mpObjVertices = &mObjVertices;

    mParticleSystem.initParticleSystem();

    mParticleSystem.mGroundpos = 0.f;
}


void GLWidget::initializeGL()
{
    glClearColor(0.25, 0.25, 0.25, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_COLOR_MATERIAL);
}


void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
        glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
        glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);



        if (showObject) {
            glEnable(GL_LIGHTING);
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, &mObjVertices[0]);
            glNormalPointer(GL_FLOAT,0, &mObjNormals[0]);
            glColor3f(0.5, 0.3, 0.2);
            glDrawElements(GL_TRIANGLES, mObjIndices.size() ,GL_UNSIGNED_INT,
                           &mObjIndices[0]);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisable(GL_LIGHTING);
        }

        if (showGround) {
            glPushMatrix();
                glTranslatef(0.0, mParticleSystem.mGroundpos-0.05f, 0.0);
                ybox(0.0001);
            glPopMatrix();
        }

        if (showAxes) {
            glPushMatrix();
                glColor4f(1.f, 1.f, 0.f, 0.5f);
                float axisLength = 2.0;
                glBegin(GL_LINES);
                    glVertex3d(0.0, 0.0, 0.0);
                    glVertex3d(axisLength, 0.0, 0.0);
                    glVertex3d(0.0, 0.0, 0.0);
                    glVertex3d(0.0, axisLength, 0.0);
                    glVertex3d(0.0, 0.0, 0.0);
                    glVertex3d(0.0, 0.0, axisLength);
                glEnd();

                glRasterPos3d(axisLength, 0.0, 0.0);
                drawLabel("X");
                glRasterPos3d(0.0, axisLength, 0.0);
                drawLabel("Y");
                glRasterPos3d(0.0, 0.0, axisLength);
                drawLabel("Z");
            glPopMatrix();
        }

        if (previewVertState) {
            GLUquadricObj *quadric;
            quadric = gluNewQuadric();
            gluQuadricDrawStyle(quadric, GLU_FILL);
            glPushMatrix();
                glTranslatef(mPreviewVertPos[0], mPreviewVertPos[1],
                             mPreviewVertPos[2]);
                glColor4f(1.f, 0.f, 0.f, 1.f);
                gluSphere(quadric, 0.03 , 36 , 18);
            glPopMatrix();
        }

        if (showTets) {
            for (const auto &el : mTets) {
                drawTet(mVertices[el[0]], mVertices[el[1]], mVertices[el[2]],
                        mVertices[el[3]]);
            }
        }

        if (showSkeleton) {
            glDisable (GL_DEPTH_TEST);
            for (const auto &el : mBones) {
                glPushMatrix();
                    glColor4f(1.f, 1.f, 1.f, 1.0f);
                    glBegin(GL_LINES);
                        glVertex3d(mVertices[el[0]][0], mVertices[el[0]][1],
                                   mVertices[el[0]][2]);
                        glVertex3d(mVertices[el[1]][0], mVertices[el[1]][1],
                                   mVertices[el[1]][2]);
                    glEnd();
                glPopMatrix();
            }
            glEnable (GL_DEPTH_TEST);
        }

        if (showVerts) {
            glDisable (GL_DEPTH_TEST);
            GLUquadricObj *quadric;
            quadric = gluNewQuadric();
            gluQuadricDrawStyle(quadric, GLU_FILL);

            for (auto it = mVertices.begin(); it != mVertices.end(); ++it) {
                glColor3f(1.f, 1.f, 0.5f);
                glRasterPos3d((*it)[0]+0.05, (*it)[1]+0.05, (*it)[2]+0.05);
                auto pos = it - mVertices.begin();
                const char *num = QString::number(pos).toUtf8().constData();
                drawLabel(num);

                glPushMatrix();
                    glTranslatef((*it)[0], (*it)[1],(*it)[2]);
                    glColor4f(0.f, 0.f, 1.f, 1.f);
                    gluSphere(quadric, 0.02 , 36 , 18);
                glPopMatrix();
            }
            glEnable (GL_DEPTH_TEST);
        }

        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);

    glPopMatrix();
}


void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-2*zoomFactor + mHorMove,
              3*zoomFactor + mDepthMove,
              7*zoomFactor + mVertMove, mHorMove, mDepthMove, mVertMove, 0, 1, 0);
}


void GLWidget::drawRectFace(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2,
                            const Vec3f& v3)
{
    glBegin(GL_LINE_STRIP);
        glVertex3f(v0[0], v0[1], v0[2]);
        glVertex3f(v1[0], v1[1], v1[2]);
        glVertex3f(v2[0], v2[1], v2[2]);
        glVertex3f(v3[0], v3[1], v3[2]);
        glVertex3f(v0[0], v0[1], v0[2]);
    glEnd();
}


void GLWidget::drawTriFace(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
{
    glBegin(GL_TRIANGLES);
        glVertex3f(v0[0], v0[1], v0[2]);
        glVertex3f(v1[0], v1[1], v1[2]);
        glVertex3f(v2[0], v2[1], v2[2]);
    glEnd();
}


void GLWidget::drawTet(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2,
                       const Vec3f& v3)
{
    glColor4f(1.f, 0.f, 0.f, 0.5f);
    drawTriFace(v0, v1, v2);
    glColor4f(1.f, 1.f, 0.f, 0.5f);
    drawTriFace(v0, v1, v3);
    glColor4f(0.f, 1.f, 0.f, 0.5f);
    drawTriFace(v0, v2, v3);
    glColor4f(0.f, 0.f, 1.f, 0.5f);
    drawTriFace(v1, v2, v3);
}


void GLWidget::showVertsStateChanged(int state)
{
    showVerts = (state == Qt::Checked);
    updateGL();
}


void GLWidget::showTetsStateChanged(int state)
{
    showTets = (state == Qt::Checked);
    updateGL();
}


void GLWidget::showAxesStateChanged(int state)
{
    showAxes = (state == Qt::Checked);
    updateGL();
}


void GLWidget::showObjectStateChanged(int state)
{
    showObject = (state == Qt::Checked);
    updateGL();
}


void GLWidget::showSkeletonStateChanged(int state)
{
    showSkeleton = (state == Qt::Checked);
    updateGL();
}


void GLWidget::showGroundStateChanged(int state)
{
    showGround = (state == Qt::Checked);
    updateGL();
}


void GLWidget::ybox(float h)
{
    glColor3f(0.65f, 0.45f, 0.2f);
    glBegin( GL_QUADS );

    glNormal3d( 1.0 ,0.0, 0.0);			// +x side
    glVertex3d( 5.0, 0.0, 5.0);
    glVertex3d( 5.0, 0.0,-5.0);
    glVertex3d( 5.0,  h,-5.0);
    glVertex3d( 5.0,  h, 5.0);

    glNormal3d( 0.0 ,0.0, -1.0);		// -z side
    glVertex3d( 5.0,0.0,-5.0);
    glVertex3d(-5.0,0.0,-5.0);
    glVertex3d(-5.0,  h,-0.25);
    glVertex3d( 5.0,  h,-0.25);

    glNormal3d(-1.0, 0.0, 0.0);			// -x side
    glVertex3d(-5.0,0.0,-5.0);
    glVertex3d(-5.0,0.0, 5.0);
    glVertex3d(-5.0,  h, 5.0);
    glVertex3d(-5.0,  h,-5.0);

    glNormal3d( 0.0, 0.0, 1.0);			// +z side
    glVertex3d(-5.0,0.0, 5.0);
    glVertex3d( 5.0,0.0, 5.0);
    glVertex3d( 5.0,  h, 5.0);
    glVertex3d(-5.0,  h, 5.0);

    glNormal3d( 0.0, 1.0, 0.0);			// top (+y)
    glVertex3d( 5.0,  h, 5.0);
    glVertex3d( 5.0,  h,-5.0);
    glVertex3d(-5.0,  h,-5.0);
    glVertex3d(-5.0,  h, 5.0);

    glNormal3d( 0.0,-1.0, 0.0);			// bottom (-y)
    glVertex3d( 0.25,0.0, 5.0);
    glVertex3d(-0.25,0.0, 5.0);
    glVertex3d(-0.25,0.0,-5.0);
    glVertex3d( 0.25,0.0,-5.0);

    glEnd();
}


 int GLWidget::selection(int x, int y)
{
     int width = this->width();
     int height = this->height();

     int winX = x;
     int winY = height - y;

     GLdouble posX, posY, posZ;
     bool bResult = gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &posX, &posY, &posZ);
     Vec3f nearPoint = Vec3f(posX, posY, posZ);

     bResult = gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &posX, &posY, &posZ);
     Vec3f farPoint = Vec3f(posX, posY, posZ);

     float p = farPoint[0]-nearPoint[0];
     float q = farPoint[1]-nearPoint[1];
     float r= farPoint[2]-nearPoint[2];
     Vec3f dirc = Vec3f(p,q,r);

     float d = 1000000.f;
     int which=-1;
     int num=0;
     for(auto it = mVertices.begin(); it != mVertices.end(); ++it) {
         Vec3f temp = *it;
         float distsquare = (temp-nearPoint)*(temp-nearPoint)-(dirc*(temp-nearPoint))*(dirc*(temp-nearPoint))/(p*p+q*q+r*r);
        if(distsquare<d) {
                d=distsquare;
                which = num;
        }
         num++;
     }

     return which;
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
     if(event->buttons() & Qt::MidButton){
         mMidbutton = true;
         whichtetvertice = selection(lastPos.x(),lastPos.y());
     }
}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    }
    else if(event->buttons() & Qt::MidButton){
        Vec3f originpos = mParticleSystem.mParticles[whichtetvertice].getPosition();
        Vec3f stepmove;

        float3x4 invViewMatrix1;
        invViewMatrix1.m[0][0] = modelview[0];
        invViewMatrix1.m[1][0]  = modelview[4];
        invViewMatrix1.m[2][0]  = modelview[8];
        invViewMatrix1.m[3][0] = modelview[12];
        invViewMatrix1.m[0][1]= modelview[1];
        invViewMatrix1.m[1][1] = modelview[5];
        invViewMatrix1.m[2][1] = modelview[9];
        invViewMatrix1.m[3][1]= modelview[13];
        invViewMatrix1.m[0][2] = modelview[2];
        invViewMatrix1.m[1][2] = modelview[6];
        invViewMatrix1.m[2][2] = modelview[10];
        invViewMatrix1.m[3][2] = modelview[14];

        stepmove = mul(invViewMatrix1,Vec3f(0.01*dx,-0.01*dy,0.0));

        Vec3f newpos = originpos + stepmove;
        mParticleSystem.mParticles[whichtetvertice].setPosition(newpos);
        mVertices[whichtetvertice] = newpos;

    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();

    if (mStickTetState) {
        mParticleSystem.mObjVertexSystem.updateSystem();
    }

    updateGL();
}


void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    mMidbutton = false;
}


void GLWidget::wheelEvent(QWheelEvent* event)
{
    if (mMidbutton) {
        return;
    }

    if (event->delta() > 0) {
        zoomFactor = std::max(0.f, zoomFactor*0.95f);
    }
    else if (event->delta() < 0) {
        zoomFactor *= 1.05f;
    }
    resizeGL(this->width(), this->height());
    updateGL();
}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}


void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        updateGL();
    }
}


void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        updateGL();
    }
}


void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        updateGL();
    }
}


void GLWidget::drawLabel(const char* format , ...)
{
    char    buf[8192];
    char*   ch=buf;
    va_list args;
    va_start(args,format);
    vsnprintf(buf,8192,format,args);
    va_end(args);
    while (*ch)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}


void GLWidget::loadModel(const QString &qFName)
{
    const char *fName = qFName.toLocal8Bit().constData();
    const aiScene* scene = importer.ReadFile(fName, aiProcess_CalcTangentSpace |
                                                    aiProcess_GenSmoothNormals |
                                                    aiProcess_Triangulate |
                                                    aiProcess_JoinIdenticalVertices |
                                                    aiProcess_SortByPType);
    if (!scene)
    {
        QMessageBox::information(this, tr("Cannot open file"),
                                 tr("Cannot open object"));
        return;
    }

    mObjVertices.clear();
    mObjIndices.clear();
    mObjNormals.clear();
    mObjVerticesInitPos.clear();
    mObjVerticesBackup.clear();
    mObjNormalsBackup.clear();
    mObjIndicesBackup.clear();

    float minX = 1e6, maxX = -1e6, minY = 1e6, maxY = -1e6, minZ = 1e6;
    float maxZ = -1e6;

    for (unsigned i = 0; i < scene->mMeshes[0]->mNumVertices; ++i) {
        minX = std::min(minX, scene->mMeshes[0]->mVertices[i].x);
        maxX = std::max(maxX, scene->mMeshes[0]->mVertices[i].x);
        minY = std::min(minY, scene->mMeshes[0]->mVertices[i].y);
        maxY = std::max(maxY, scene->mMeshes[0]->mVertices[i].y);
        minZ = std::min(minZ, scene->mMeshes[0]->mVertices[i].z);
        maxZ = std::max(maxZ, scene->mMeshes[0]->mVertices[i].z);
    }

    float xRange = maxX - minX, yRange = maxY - minY, zRange = maxZ - minZ;
    float scaling = 0.2*std::max(std::max(xRange, yRange), zRange);

    // scaling all the vertices and moving to (+) side of (0, 0, 0)
    for (unsigned i = 0; i < scene->mMeshes[0]->mNumVertices; ++i) {
        mObjVertices.push_back((scene->mMeshes[0]->mVertices[i].x-minX)/scaling);
        mObjVertices.push_back((scene->mMeshes[0]->mVertices[i].y-minY)/scaling);
        mObjVertices.push_back((scene->mMeshes[0]->mVertices[i].z-minZ)/scaling);
    }

    for (unsigned i = 0; i < scene->mMeshes[0]->mNumFaces; ++i) {
        mObjIndices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[0]);
        mObjIndices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[1]);
        mObjIndices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[2]);
    }

    for (unsigned i = 0; i < scene->mMeshes[0]->mNumVertices; ++i) {
        mObjNormals.push_back(scene->mMeshes[0]->mNormals[i].x);
        mObjNormals.push_back(scene->mMeshes[0]->mNormals[i].y);
        mObjNormals.push_back(scene->mMeshes[0]->mNormals[i].z);
    }
    mObjVerticesInitPos = mObjVertices;
    mParticleSystem.initParticleSystem();
    updateGL();
}


void GLWidget::openTetFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_3);
    mVertices.clear();
    mTets.clear();
    mBones.clear();
    mVerticesBackup.clear();
    mTetsBackup.clear();
    mBonesBackup.clear();
    float verticesSizeFloat, tetsSizeFloat, bonesSizeFloat;
    int verticesSize, tetsSize, bonesSize;
    in >> verticesSizeFloat;
    in >> tetsSizeFloat;
    in >> bonesSizeFloat;
    verticesSize = static_cast<int>(verticesSizeFloat);
    tetsSize = static_cast<int>(tetsSizeFloat);
    bonesSize = static_cast<int>(bonesSizeFloat);

    for (int i = 0; i < verticesSize; ++i) {
        float x, y, z;
        in >> x; in >> y; in >> z;
        mVertices.push_back(Vec3f(x, y, z));
    }

    for (int i = 0; i < tetsSize; ++i) {
        float x, y, z, w;
        in >> x; in >> y; in >> z; in >> w;
        std::vector<int> v = {static_cast<int>(x),
                              static_cast<int>(y),
                              static_cast<int>(z),
                              static_cast<int>(w)};
        mTets.push_back(v);
    }

    for (int i = 0; i < bonesSize; ++i) {
        float x, y;
        in >> x; in >> y;
        std::vector<int> v = {static_cast<int>(x),
                              static_cast<int>(y)};
        mBones.push_back(v);
    }

    mParticleSystem.initParticleSystem();
    updateGL();
}


void GLWidget::saveToFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_3);
    out << static_cast<float>(mVertices.size());
    out << static_cast<float>(mTets.size());
    out << static_cast<float>(mBones.size());

    for (const auto &el : mVertices) {
        out << el[0] << el[1] << el[2];
    }

    for (const auto &el : mTets) {
        out << static_cast<float>(el[0]) << static_cast<float>(el[1])
            << static_cast<float>(el[2]) << static_cast<float>(el[3]);
    }

    for (const auto &el : mBones) {
        out << static_cast<float>(el[0]) << static_cast<float>(el[1]);
    }
}


void GLWidget::saveAllToFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_3);
    out << static_cast<float>(mVertices.size());
    out << static_cast<float>(mTets.size());
    out << static_cast<float>(mBones.size());
    out << static_cast<float>(mObjVertices.size());
    out << static_cast<float>(mObjIndices.size());
    out << static_cast<float>(mObjNormals.size());

    for (const auto &el : mVertices) {
        out << el[0] << el[1] << el[2];
    }

    for (const auto &el : mTets) {
        out << static_cast<float>(el[0]) << static_cast<float>(el[1])
            << static_cast<float>(el[2]) << static_cast<float>(el[3]);
    }

    for (const auto &el : mBones) {
        out << static_cast<float>(el[0]) << static_cast<float>(el[1]);
    }

    for (const auto &el : mObjVertices) {
        out << el;
    }

    for (const auto &el : mObjIndices) {
        out << el;
    }

    for (const auto &el : mObjNormals) {
        out << el;
    }
}


void GLWidget::openAllFromFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_3);
    mVertices.clear();
    mTets.clear();
    mBones.clear();
    mObjVertices.clear();
    mObjIndices.clear();
    mObjNormals.clear();
    mObjVerticesInitPos.clear();
    mVerticesBackup.clear();
    mTetsBackup.clear();
    mBonesBackup.clear();
    mObjVerticesBackup.clear();
    mObjNormalsBackup.clear();
    mObjIndicesBackup.clear();
    float verticesSizeFloat, tetsSizeFloat, bonesSizeFloat;
    float objVerticesSizeFloat, objIndicesSizeFloat, objNormalsSizeFloat;
    int verticesSize, tetsSize, objVerticesSize, objIndicesSize;
    int bonesSize, objNormalsSize;
    in >> verticesSizeFloat;
    in >> tetsSizeFloat;
    in >> bonesSizeFloat;
    in >> objVerticesSizeFloat;
    in >> objIndicesSizeFloat;
    in >> objNormalsSizeFloat;
    verticesSize    = static_cast<int>(verticesSizeFloat);
    tetsSize        = static_cast<int>(tetsSizeFloat);
    bonesSize       = static_cast<int>(bonesSizeFloat);
    objVerticesSize = static_cast<int>(objVerticesSizeFloat);
    objIndicesSize  = static_cast<int>(objIndicesSizeFloat);
    objNormalsSize  = static_cast<int>(objNormalsSizeFloat);

    for (int i = 0; i < verticesSize; ++i) {
        float x, y, z;
        in >> x; in >> y; in >> z;
        mVertices.push_back(Vec3f(x, y, z));
    }

    for (int i = 0; i < tetsSize; ++i) {
        float x, y, z, w;
        in >> x; in >> y; in >> z; in >> w;
        std::vector<int> v = {static_cast<int>(x),
                              static_cast<int>(y),
                              static_cast<int>(z),
                              static_cast<int>(w)};
        mTets.push_back(v);
    }

    for (int i = 0; i < bonesSize; ++i) {
        float x, y;
        in >> x; in >> y;
        std::vector<int> v = {static_cast<int>(x),
                              static_cast<int>(y)};
        mBones.push_back(v);
    }

    for (int i = 0; i < objVerticesSize; ++i) {
        float inFloat;
        in >> inFloat;
        mObjVertices.push_back(inFloat);
    }

    for (int i = 0; i < objIndicesSize; ++i) {
        int inInt;
        in >> inInt;
        mObjIndices.push_back(inInt);
    }

    for (int i = 0; i < objNormalsSize; ++i) {
        float inFloat;
        in >> inFloat;
        mObjNormals.push_back(inFloat);
    }

    mObjVerticesInitPos = mObjVertices;
    mParticleSystem.initParticleSystem();
    updateGL();
}


void GLWidget::tetsEdited()
{
    updateGL();
}


void GLWidget::bonesEdited()
{
    updateGL();
    if (mBoneStickTetState) {
        mParticleSystem.mObjVertexSystem.updateSystem();
    }
}


void GLWidget::objectEdited()
{
    updateGL();
}


void GLWidget::previewVertStateChanged(int state)
{
    previewVertState = (state == Qt::Checked);
    updateGL();
}


void GLWidget::simulationStateChanged(int state)
{
    simulate = (state == Qt::Checked);

    if (mVerticesBackup.size() == 0 && simulate) {
        mVerticesBackup    = mVertices;
        mTetsBackup        = mTets;
        mBonesBackup       = mBones;
        mObjVerticesBackup = mObjVertices;
        mObjNormalsBackup  = mObjNormals;
        mObjIndicesBackup  = mObjIndices;
    }

    if (simulate) {
        mParticleSystem.initParticleSystem();
    }
}


void GLWidget::updateSimulation()
{
    if (!simulate) {
        return;
    }

    if(mMidbutton){
        mParticleSystem.stopSimulation();
    }

    else {
        mParticleSystem.startSimulation();
    }

    mParticleSystem.computeForcesAndUpdateParticles();
}


void GLWidget::updateSimulationGL()
{
    if (!simulate) {
        return;
    }
    updateGL();
}


void GLWidget::resetSim()
{
    if (mVerticesBackup.size() != 0) {
        mVertices    = mVerticesBackup;
        mTets        = mTetsBackup;
        mBones       = mBonesBackup;
        mObjVertices = mObjVerticesBackup;
        mObjNormals  = mObjNormalsBackup;
        mObjIndices  = mObjIndicesBackup;
    }

    if (simulate) {
        mParticleSystem.initParticleSystem();
    }
    updateGL();
}


void GLWidget::stickObjectToTetsStateChanged(int state)
{
    mStickTetState = (state == Qt::Checked);
}


void GLWidget::boneStickTetStateChanged(int state)
{
    mStickTetState = (state == Qt::Checked);
}


void GLWidget::importTetgenFiles(const QString &fileNameNode,
                                 const QString &fileNameEle)
{
    mVertices.clear();
    mTets.clear();
    mBones.clear();
    mObjVertices.clear();
    mObjIndices.clear();
    mObjNormals.clear();
    mObjVerticesInitPos.clear();
    mVerticesBackup.clear();
    mTetsBackup.clear();
    mBonesBackup.clear();
    mObjVerticesBackup.clear();
    mObjNormalsBackup.clear();
    mObjIndicesBackup.clear();

    QFile fileNode(fileNameNode);
    fileNode.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in(&fileNode);
    auto nodesSize = in.readLine().split(" ")[0].toInt();
    for (int i = 0; i < nodesSize; ++i) {
        QStringList line = in.readLine().split(" ");
        QString str;
        int idx = -1;
        auto el = Vec3f(0.0, 0.0, 0.0);
        foreach (str, line) {
            if (str != "") {
                if (idx >= 0) {
                    el[idx] = str.toFloat();
                }
                ++idx;
            }
        }
        mVertices.push_back(el);
    }
    fileNode.close();

    QFile fileEle(fileNameEle);
    fileEle.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in2(&fileEle);
    auto tetsSize = in2.readLine().split(" ")[0].toInt();
    for (int i = 0; i < tetsSize; ++i) {
        QStringList line = in2.readLine().split(" ");
        QString str;
        int idx = -1;
        std::vector<int> el = {0, 0, 0, 0};
        foreach (str, line) {
            if (str != "") {
                if (idx >= 0) {
                    el[idx] = str.toInt();
                }
                ++idx;
            }
        }
        mTets.push_back(el);
    }

    float minX = 1e6, maxX = -1e6, minY = 1e6, maxY = -1e6, minZ = 1e6;
    float maxZ = -1e6;

    for (const auto &el : mVertices) {
        minX = std::min(minX, el[0]);
        maxX = std::max(maxX, el[0]);
        minY = std::min(minY, el[1]);
        maxY = std::max(maxY, el[1]);
        minZ = std::min(minZ, el[2]);
        maxZ = std::max(maxZ, el[2]);
    }

    float xRange = maxX - minX, yRange = maxY - minY, zRange = maxZ - minZ;
    float scaling = 0.2*std::max(std::max(xRange, yRange), zRange);

    for (auto &el : mVertices) {
        el[0] = (el[0] - minX)/scaling;
        el[1] = (el[1] - minY)/scaling;
        el[2] = (el[2] - minZ)/scaling;
    }
    updateGL();
}
