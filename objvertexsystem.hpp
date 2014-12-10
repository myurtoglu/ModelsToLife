#ifndef OBJVERTEXSYSTEM_HPP
#define OBJVERTEXSYSTEM_HPP

#include "vec.hpp"

#include <QGLWidget>

#include <vector>


class ObjVertexSystem {
public:
    ObjVertexSystem();
    void updateSystem();
    void initObjVertexSystem();

    std::vector<GLfloat>          *mpObjVertices;
    std::vector<Vec3f>            *mpVertices;
    std::vector<std::vector<int>> *mpTets;

private:
    Vec3f              getGlobalCoords(int vertIdx);
    void               updateObjVertices();
    float              determinant4x4(const Vec4f& v0, const Vec4f& v1,
                                      const Vec4f& v2, const Vec4f& v3);
    int                findTetNumber(const Vec3f& vert);
    Vec4f              calcBarycentricCoords(const Vec3f& p,
                                             const std::vector<Vec3f>& tetVertices);
    bool               checkPointInTet(const Vec3f& p,
                                       const std::vector<Vec3f>& tetVertices);
    std::vector<Vec3f> getTetVertices(int tetNumber);
    Vec3f              calcGlobalCoords(const Vec4f& barycentricCoords,
                                        int tetNumber);

    std::vector<int>              mTetNumbers;
    std::vector<Vec4f>            mBarycentricCoords;
};

#endif // OBJVERTEXSYSTEM_HPP
