#include "particlesystem.hpp"

#include <QDebug>

#include <algorithm>
#include <cmath>
#include <iostream>


ParticleSystem::ParticleSystem(): mDeltaT(0.016f), mGravity(9.8f),
    mRestitution(0.25f), mStiffness(200.f), mDamping(10.f), mMass(0.5f),
    mSimulate(true)
{ }


void ParticleSystem::initParticleSystem()
{
    mInitialVertexPositions        = *mpVertices;
    mObjVertexSystem.mpObjVertices = mpObjVertices;
    mObjVertexSystem.mpVertices    = mpVertices;
    mObjVertexSystem.mpTets        = mpTets;

    mParticles.clear();
    for (auto it = mpVertices->begin(); it != mpVertices->end(); ++it) {
        Particle particle;
        auto idx = it - mpVertices->begin();
        particle.setPosition(*it);
        particle.setMass(mMass);
        for (const auto &tet : *mpTets) {
            auto res = find(tet.begin(), tet.end(), idx);
            if (res != tet.end()) {
                for (const auto &el : tet) {
                    if (el != idx) {
                        particle.mNeighbors.insert(el);
                    }
                }
            }
        }
        mParticles.push_back(particle);
    }
    mObjVertexSystem.initObjVertexSystem();
}


void ParticleSystem::computeForcesAndUpdateParticles()
{
    if (mSimulate) {
        float friction = 0.3f;
        computeForcesOnSystem();

        for (auto it = mParticles.begin(); it != mParticles.end(); ++it) {

            auto particleIdx = (it - mParticles.begin());
            Particle temppar = *it;

            Vec3f k1 = mDeltaT * it->getVelocity();
            temppar.setPosition(it->getPosition()+k1/2.0f);
            computeForcesOnParticle(temppar, particleIdx);

            Vec3f k2 = mDeltaT*(it->getVelocity() + mDeltaT/2.0*temppar.getForce()/temppar.getMass());
            temppar.setPosition(it->getPosition()+k2/2.0f);
            computeForcesOnParticle(temppar, particleIdx);

            Vec3f k3 = mDeltaT*(it->getVelocity() + mDeltaT/2.0*temppar.getForce()/temppar.getMass());
            temppar.setPosition(it->getPosition()+k3);
            computeForcesOnParticle(temppar, particleIdx);

            Vec3f k4 = mDeltaT*(it->getVelocity() + mDeltaT*temppar.getForce()/temppar.getMass());


            Vec3f position = it->getPosition() + k1/6.0+k2/3.0+k3/3.0+k4/6.0;

            it->setPosition(position);

            // collision
            if (((it->getPosition() - Vec3f(0.0,mGroundpos,0.0))
                 * Vec3f(0.0, 1.0, 0.0) < 0.05)
                && it->getVelocity()*Vec3f(0.0, 1.0, 0.0) <= 0.0) {
                auto oldVel = it->getVelocity();
                auto newVel = Vec3f(friction*mRestitution*oldVel[0], -mRestitution*oldVel[1],
                                    friction*mRestitution*oldVel[2]);
                it->setVelocity(newVel);

                if (abs(it->getVelocity().length()) < 0.001) {
                    it->setPosition(Vec3f(it->getPosition()[0], mGroundpos,
                                          it->getPosition()[2]));
                    it->setVelocity(Vec3f(newVel[0], 0.0, newVel[2]));
                }
            }

            else {
                it->setVelocity(it->getVelocity() + mDeltaT*it->getForce()/it->getMass());
            }
        }

        for (unsigned i = 0; i < mParticles.size(); ++i) {
            (*mpVertices)[i] = mParticles[i].getPosition();
        }
    }

    mObjVertexSystem.updateSystem();
}


void ParticleSystem::computeForcesOnParticle(Particle &particle,
                                             unsigned particleIdx)
{
    auto gravityForce = Vec3f(0.f, -mGravity*particle.getMass(), 0.f);
    auto neighborForce = Vec3f(0.f, 0.f, 0.f);
    for (const auto &neighbor : particle.mNeighbors) {
        auto springCompression = currentLength(particleIdx, neighbor)
                                 - restLength(particleIdx, neighbor);
        auto velocityDiff = mParticles[neighbor].getVelocity()
                            - particle.getVelocity();
        auto forceDirection = directionFromAtoB(particleIdx, neighbor);
        neighborForce += (mStiffness * springCompression
                          + mDamping * velocityDiff * forceDirection)
                         * forceDirection;
    }
    particle.setForce(gravityForce + neighborForce);
}


void ParticleSystem::computeForcesOnSystem()
{
    for (auto it = mParticles.begin(); it != mParticles.end(); ++it) {
        auto particleIdx = it - mParticles.begin();
        computeForcesOnParticle(*it, particleIdx);
    }
}


Vec3f ParticleSystem::directionFromAtoB(int nodeA, int nodeB)
{
    auto posA = (*mpVertices)[nodeA];
    auto posB = (*mpVertices)[nodeB];
    auto diff = posB - posA;
    if (diff.length() > 1e-8)
        diff.normalize();
    return diff;
}


float ParticleSystem::currentLength(int nodeA, int nodeB)
{
    auto posA = (*mpVertices)[nodeA];
    auto posB = (*mpVertices)[nodeB];
    auto diff = posB - posA;
    return diff.length();
}


float ParticleSystem::restLength(int nodeA, int nodeB)
{
    auto posA = mInitialVertexPositions[nodeA];
    auto posB = mInitialVertexPositions[nodeB];
    auto diff = posB - posA;
    return diff.length();
}
