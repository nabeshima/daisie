/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _DAISIE_BODY_H_
#define _DAISIE_BODY_H_

#include <cutil/Kinematics.h>

#include <ode/common.h>

#include <string>

#include "DaisieObject.h"
#include "Mass.h"

namespace daisie {

class World;

using namespace std;
using namespace cotave;

class Body : public DaisieObject {
  friend class DaisieObjectFactory<World>;

 private:
  dBodyID _id;

  // non-copyable
  Body(const Body &);
  void operator=(const Body &);

 protected:
  // constructer used in World
  Body(World *world, const string &name);
  virtual ~Body();

 public:
  dBodyID id() const;

  // Kinematics

  void setPosition(const ColumnVector3 &p);
  void setRotation(const RotationMatrix &R);
  void setQuaternion(const Quaternion &q);
  void setLinearVelocity(const ColumnVector3 &lv);
  void setAngularVelocity(const ColumnVector3 &av);

  ColumnVector3 getPosition() const;
  RotationMatrix getRotation() const;
  Quaternion getQuaternion() const;
  ColumnVector3 getLinearVelocity() const;
  ColumnVector3 getAngularVelocity() const;

  // Dynamics parameters

  void setMass(const Mass &m);
  Mass getMass() const;

  // force and torque

  void addForce(const ColumnVector3 &f);
  void addTorque(const ColumnVector3 &t);
  void addRelativeForce(const ColumnVector3 &rf);
  void addRelativeTorque(const ColumnVector3 &rt);
  void addForceAtPoint(const ColumnVector3 &f, const ColumnVector3 &p);
  void addForceAtRelativePoint(const ColumnVector3 &f, const ColumnVector3 &rp);
  void addRelativeForceAtPoint(const ColumnVector3 &rf, const ColumnVector3 &p);
  void addRelativeForceAtRelativePoint(const ColumnVector3 &rf,
                                       const ColumnVector3 &rp);

  ColumnVector3 getForce() const;
  ColumnVector3 getTorque() const;
};
}  // namespace daisie

#endif
