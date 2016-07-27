/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Body.h"

#include <ode/objects.h>

#include <string>

#include "World.h"

using namespace std;
using namespace cotave;
using namespace daisie;

// constructer used in World
Body::Body(World *world, const string &name)
    : DaisieObject(name), _id(dBodyCreate(world->id())) {
  dBodySetData(_id, this);
}

Body::~Body() { dBodyDestroy(_id); }

dBodyID Body::id() const { return _id; }

void Body::setPosition(const ColumnVector3 &p) {
  dBodySetPosition(_id, p(0), p(1), p(2));
}

void Body::setRotation(const RotationMatrix &R) {
  dMatrix3 tmp;
  tmp[0] = R(0, 0);
  tmp[1] = R(0, 1);
  tmp[2] = R(0, 2);
  tmp[3] = 0.0;
  tmp[4] = R(1, 0);
  tmp[5] = R(1, 1);
  tmp[6] = R(1, 2);
  tmp[7] = 0.0;
  tmp[8] = R(2, 0);
  tmp[9] = R(2, 1);
  tmp[10] = R(2, 2);
  tmp[11] = 0.0;
  dBodySetRotation(_id, tmp);
}

void Body::setQuaternion(const Quaternion &q) {
  dBodySetQuaternion(
      _id, q.data());  // dQuaternion is array of double whose size is 4.
}

void Body::setLinearVelocity(const ColumnVector3 &lv) {
  dBodySetLinearVel(_id, lv(0), lv(1), lv(2));
}

void Body::setAngularVelocity(const ColumnVector3 &av) {
  dBodySetAngularVel(_id, av(0), av(1), av(2));
}

ColumnVector3 Body::getPosition() const {
  return ColumnVector3(dBodyGetPosition(_id));
}

RotationMatrix Body::getRotation() const {
  const dReal *tmp = dBodyGetRotation(_id);
  return RotationMatrix(tmp[0], tmp[1], tmp[2], tmp[4], tmp[5], tmp[6], tmp[8],
                        tmp[9], tmp[10]);
}

Quaternion Body::getQuaternion() const {
  return Quaternion(dBodyGetQuaternion(_id));
}

ColumnVector3 Body::getLinearVelocity() const {
  return ColumnVector3(dBodyGetLinearVel(_id));
}

ColumnVector3 Body::getAngularVelocity() const {
  return ColumnVector3(dBodyGetAngularVel(_id));
}

void Body::setMass(const Mass &m) { dBodySetMass(_id, &m); }

Mass Body::getMass() const {
  Mass tmp;
  dBodyGetMass(_id, &tmp);
  return tmp;
}

void Body::addForce(const ColumnVector3 &f) {
  dBodyAddForce(_id, f(0), f(1), f(2));
}

void Body::addTorque(const ColumnVector3 &t) {
  dBodyAddTorque(_id, t(0), t(1), t(2));
}

void Body::addRelativeForce(const ColumnVector3 &rf) {
  dBodyAddRelForce(_id, rf(0), rf(1), rf(2));
}

void Body::addRelativeTorque(const ColumnVector3 &rt) {
  dBodyAddRelTorque(_id, rt(0), rt(1), rt(2));
}

void Body::addForceAtPoint(const ColumnVector3 &f, const ColumnVector3 &p) {
  dBodyAddForceAtPos(_id, f(0), f(1), f(2), p(0), p(1), p(2));
}

void Body::addForceAtRelativePoint(const ColumnVector3 &f,
                                   const ColumnVector3 &rp) {
  dBodyAddForceAtRelPos(_id, f(0), f(1), f(2), rp(0), rp(1), rp(2));
}

void Body::addRelativeForceAtPoint(const ColumnVector3 &rf,
                                   const ColumnVector3 &p) {
  dBodyAddRelForceAtPos(_id, rf(0), rf(1), rf(2), p(0), p(1), p(2));
}

void Body::addRelativeForceAtRelativePoint(const ColumnVector3 &rf,
                                           const ColumnVector3 &rp) {
  dBodyAddRelForceAtRelPos(_id, rf(0), rf(1), rf(2), rp(0), rp(1), rp(2));
}

ColumnVector3 Body::getForce() const {
  return ColumnVector3(dBodyGetForce(_id));
}

ColumnVector3 Body::getTorque() const {
  return ColumnVector3(dBodyGetTorque(_id));
}
