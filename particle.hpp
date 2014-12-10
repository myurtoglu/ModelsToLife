#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "vec.hpp"

#include <set>


class Particle {
public:
    Particle(): mPosition(0.f, 0.f, 0.f), mVelocity(0.f, 0.f, 0.f),
                mForce(0.f, 0.f, 0.f) { }
    float getMass() const { return mMass; }
    Vec3f getPosition() const { return mPosition; }
    Vec3f getVelocity() const { return mVelocity; }
    Vec3f getForce() const { return mForce; }
    void  setMass(float mass) { mMass = mass; }
    void  setPosition(Vec3f pos) { mPosition = pos; }
    void  setVelocity(Vec3f vel) { mVelocity = vel; }
    void  setForce(Vec3f force) { mForce = force; }
//    bool misGround;

    std::set<int>    mNeighbors;

private:
    float            mMass;
    Vec3f            mPosition;
    Vec3f            mVelocity;
    Vec3f            mForce;

};

#endif // PARTICLE_HPP
