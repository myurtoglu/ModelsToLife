#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include "objvertexsystem.hpp"
#include "particle.hpp"
#include "vec.hpp"

#include <QGLWidget>

#include <vector>


class ParticleSystem {
public:
    ParticleSystem();
    void initParticleSystem();
    void computeForcesAndUpdateParticles();

    std::vector<Vec3f>            *mpVertices;
    std::vector<std::vector<int>> *mpTets;
    std::vector<Particle>         mParticles;
    std::vector<Vec3f>            mInitialVertexPositions;
    std::vector<GLfloat>          *mpObjVertices;
    ObjVertexSystem               mObjVertexSystem;
    void                          startSimulation() { mSimulate = true; }
    void                          stopSimulation() { mSimulate = false; }

    float mDeltaT;
    float mGravity;
    float mRestitution;
    float mStiffness;
    float mDamping;
    float mMass;
    float mGroundpos;


private:
    void computeForcesOnSystem();
    void computeForcesOnParticle(Particle &particle, unsigned particleIdx);
    float currentLength(int nodeA, int nodeB);
    float restLength(int nodeA, int nodeB);
    Vec3f directionFromAtoB(int nodeA, int nodeB);

    bool  mSimulate;
};


#endif // PARTICLESYSTEM_HPP
