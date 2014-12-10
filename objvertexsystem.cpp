#include "objvertexsystem.hpp"

ObjVertexSystem::ObjVertexSystem() { }


float ObjVertexSystem::determinant4x4(const Vec4f& v0, const Vec4f& v1,
                                      const Vec4f& v2, const Vec4f& v3)
{
    float det = v0[3]*v1[2]*v2[1]*v3[0] - v0[2]*v1[3]*v2[1]*v3[0] -
                v0[3]*v1[1]*v2[2]*v3[0] + v0[1]*v1[3]*v2[2]*v3[0] +

                v0[2]*v1[1]*v2[3]*v3[0] - v0[1]*v1[2]*v2[3]*v3[0] -
                v0[3]*v1[2]*v2[0]*v3[1] + v0[2]*v1[3]*v2[0]*v3[1] +

                v0[3]*v1[0]*v2[2]*v3[1] - v0[0]*v1[3]*v2[2]*v3[1] -
                v0[2]*v1[0]*v2[3]*v3[1] + v0[0]*v1[2]*v2[3]*v3[1] +

                v0[3]*v1[1]*v2[0]*v3[2] - v0[1]*v1[3]*v2[0]*v3[2] -
                v0[3]*v1[0]*v2[1]*v3[2] + v0[0]*v1[3]*v2[1]*v3[2] +

                v0[1]*v1[0]*v2[3]*v3[2] - v0[0]*v1[1]*v2[3]*v3[2] -
                v0[2]*v1[1]*v2[0]*v3[3] + v0[1]*v1[2]*v2[0]*v3[3] +

                v0[2]*v1[0]*v2[1]*v3[3] - v0[0]*v1[2]*v2[1]*v3[3] -
                v0[1]*v1[0]*v2[2]*v3[3] + v0[0]*v1[1]*v2[2]*v3[3];

    return det;
}


Vec4f ObjVertexSystem::calcBarycentricCoords(const Vec3f& p,
                                             const std::vector<Vec3f>& tetVertices)
{
    Vec4f p0(p[0], p[1], p[2], 1.f);
    Vec4f v0(tetVertices[0][0], tetVertices[0][1], tetVertices[0][2], 1.f);
    Vec4f v1(tetVertices[1][0], tetVertices[1][1], tetVertices[1][2], 1.f);
    Vec4f v2(tetVertices[2][0], tetVertices[2][1], tetVertices[2][2], 1.f);
    Vec4f v3(tetVertices[3][0], tetVertices[3][1], tetVertices[3][2], 1.f);

    auto det0 = determinant4x4(v0, v1, v2, v3);
    auto det1 = determinant4x4(p0, v1, v2, v3);
    auto det2 = determinant4x4(v0, p0, v2, v3);
    auto det3 = determinant4x4(v0, v1, p0, v3);
    auto det4 = determinant4x4(v0, v1, v2, p0);

    return Vec4f((det1/det0), (det2/det0), (det3/det0), (det4/det0));
}


bool ObjVertexSystem::checkPointInTet(const Vec3f& p,
                                      const std::vector<Vec3f>& tetVertices)
{
    Vec4f p0(p[0], p[1], p[2], 1.f);
    Vec4f v0(tetVertices[0][0], tetVertices[0][1], tetVertices[0][2], 1.f);
    Vec4f v1(tetVertices[1][0], tetVertices[1][1], tetVertices[1][2], 1.f);
    Vec4f v2(tetVertices[2][0], tetVertices[2][1], tetVertices[2][2], 1.f);
    Vec4f v3(tetVertices[3][0], tetVertices[3][1], tetVertices[3][2], 1.f);

    auto det0 = determinant4x4(v0, v1, v2, v3);
    auto det1 = determinant4x4(p0, v1, v2, v3);
    auto det2 = determinant4x4(v0, p0, v2, v3);
    auto det3 = determinant4x4(v0, v1, p0, v3);
    auto det4 = determinant4x4(v0, v1, v2, p0);

    if (std::abs(det0) > 1e-6)
    {
        if (det0 < 0)
        {
            if ((det1 < 0) && (det2 < 0) && (det3 < 0) && (det4 < 0))
            {
                return true;
            }
        }
        if (det0 > 0)
        {
            if ((det1 > 0) && (det2 > 0) && (det3 > 0) && (det4 > 0))
            {
                return true;
            }
        }
    }
    return false;
}


int ObjVertexSystem::findTetNumber(const Vec3f& vert)
{
    for (auto it = mpTets->begin(); it != mpTets->end(); ++it)
    {
        std::vector<Vec3f> tetVertices = {(*mpVertices)[(*it)[0]],
                                          (*mpVertices)[(*it)[1]],
                                          (*mpVertices)[(*it)[2]],
                                          (*mpVertices)[(*it)[3]]};
        if (checkPointInTet(vert, tetVertices)) {
            return (it - mpTets->begin());
        }
    }

    // error: no matching tet found
    return -1;
}


void ObjVertexSystem::initObjVertexSystem()
{
    mTetNumbers.clear();
    mBarycentricCoords.clear();

    auto objVertSz = mpObjVertices->size()/3;

    if (objVertSz == 0) {
        return;
    }

    for (unsigned i = 0; i < objVertSz; ++i) {
        auto vert = Vec3f((*mpObjVertices)[3*i], (*mpObjVertices)[3*i+1],
                          (*mpObjVertices)[3*i+2]);
        auto tetNumber = findTetNumber(vert);

        // keep all vertices in tets
        tetNumber = tetNumber < 0 ? 0 : tetNumber;

        mTetNumbers.push_back(tetNumber);
        mBarycentricCoords.push_back(calcBarycentricCoords(vert,
                                                    getTetVertices(tetNumber)));
    }
}


std::vector<Vec3f> ObjVertexSystem::getTetVertices(int tetNumber)
{
    if (tetNumber < 0) {
        return std::vector<Vec3f>();
    }

    std::vector<Vec3f> tetVertices = {(*mpVertices)[(*mpTets)[tetNumber][0]],
                                      (*mpVertices)[(*mpTets)[tetNumber][1]],
                                      (*mpVertices)[(*mpTets)[tetNumber][2]],
                                      (*mpVertices)[(*mpTets)[tetNumber][3]]};
    return tetVertices;
}


void ObjVertexSystem::updateSystem()
{
    auto objVertSz = mpObjVertices->size()/3;

    for (unsigned i = 0; i < objVertSz; ++i) {
        auto barycentricCoords = mBarycentricCoords[i];
        auto tetNumber = mTetNumbers[i];
        auto vert = calcGlobalCoords(barycentricCoords, tetNumber);

        // update obj vertices for displaying
        (*mpObjVertices)[3*i]   = vert[0];
        (*mpObjVertices)[3*i+1] = vert[1];
        (*mpObjVertices)[3*i+2] = vert[2];
    }
}


Vec3f ObjVertexSystem::calcGlobalCoords(const Vec4f& barycentricCoords,
                                        int tetNumber)
{
    std::vector<Vec3f> tetVertices = getTetVertices(tetNumber);

    Vec3f globalCoords = tetVertices[0]*barycentricCoords[0]
                         + tetVertices[1]*barycentricCoords[1]
                         + tetVertices[2]*barycentricCoords[2]
                         + tetVertices[3]*barycentricCoords[3];
    return globalCoords;
}
