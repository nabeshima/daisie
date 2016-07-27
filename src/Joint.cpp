/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Joint.h"

#include <ode/objects.h>

#include <string>

#include "Body.h"
#include "World.h"

using namespace std;
using namespace cotave;
using namespace daisie;

namespace {

int paramOffset(int base, bool isFirst) {
  return base + dParamGroup * ((isFirst) ? 0 : 1);
}
}

Joint::Joint(dJointID id, const string &name) : DaisieObject(name), _id(id) {
  dJointSetData(_id, this);
}

Joint::~Joint() { dJointDestroy(_id); }

dJointID Joint::id() const { return _id; }

void Joint::connect(Body *body1, Body *body2) {
  dJointAttach(_id, ((body1 != NULL) ? body1->id() : NULL),
               ((body2 != NULL) ? body2->id() : NULL));
}

Body *Joint::getConnectedBody(bool isFirst) const {
  return static_cast<Body *>(
      dBodyGetData(dJointGetBody(_id, ((isFirst) ? 0 : 1))));
}

void Joint::setFeedbackEnable(bool onoff) {
  if (onoff) {
    dJointSetFeedback(_id, &jFeedback);
  } else {
    dJointSetFeedback(_id, NULL);
  }
}

bool Joint::isFeedbackEnabled() const {
  return (dJointGetFeedback(_id) != NULL);
}

ColumnVector3 Joint::getFeedbackForce(bool isFirst) const {
  return (isFirst) ? ColumnVector3(jFeedback.f1) : ColumnVector3(jFeedback.f2);
}

ColumnVector3 Joint::getFeedbackTorque(bool isFirst) const {
  return (isFirst) ? ColumnVector3(jFeedback.t1) : ColumnVector3(jFeedback.t2);
}

// BallJoint

BallJoint::BallJoint(World *world, const string &name)
    : Joint(dJointCreateBall(world->id(), 0), name) {}

void BallJoint::setAnchor(const ColumnVector3 &anchor) {
  dJointSetBallAnchor(id(), anchor(0), anchor(1), anchor(2));
}

ColumnVector3 BallJoint::getAnchor() const {
  dVector3 tmp;
  dJointGetBallAnchor(id(), tmp);
  return ColumnVector3(tmp);
}

// HingeJoint

HingeJoint::HingeJoint(World *world, const string &name)
    : Joint(dJointCreateHinge(world->id(), 0), name) {}

void HingeJoint::setAnchor(const ColumnVector3 &anchor) {
  dJointSetHingeAnchor(id(), anchor(0), anchor(1), anchor(2));
}

void HingeJoint::setAxis(const ColumnVector3 &axis) {
  dJointSetHingeAxis(id(), axis(0), axis(1), axis(2));
}

void HingeJoint::setLoStop(double loStop) {
  dJointSetHingeParam(id(), dParamLoStop, loStop);
}

void HingeJoint::setHiStop(double hiStop) {
  dJointSetHingeParam(id(), dParamHiStop, hiStop);
}

void HingeJoint::setVelocity(double vel) {
  dJointSetHingeParam(id(), dParamVel, vel);
}

void HingeJoint::setFMax(double fMax) {
  dJointSetHingeParam(id(), dParamFMax, fMax);
}

void HingeJoint::setFudgeFactor(double fudgeFactor) {
  dJointSetHingeParam(id(), dParamFudgeFactor, fudgeFactor);
}

void HingeJoint::setBounce(double bounce) {
  dJointSetHingeParam(id(), dParamBounce, bounce);
}

void HingeJoint::setCFM(double cfm) {
  dJointSetHingeParam(id(), dParamCFM, cfm);
}

void HingeJoint::setStopERP(double erp) {
  dJointSetHingeParam(id(), dParamStopERP, erp);
}

void HingeJoint::setStopCFM(double stopCFM) {
  dJointSetHingeParam(id(), dParamStopCFM, stopCFM);
}

double HingeJoint::getAngle() const { return dJointGetHingeAngle(id()); }

double HingeJoint::getAngleRate() const {
  return dJointGetHingeAngleRate(id());
}

double HingeJoint::getLoStop() const {
  return dJointGetHingeParam(id(), dParamLoStop);
}

double HingeJoint::getHiStop() const {
  return dJointGetHingeParam(id(), dParamHiStop);
}

ColumnVector3 HingeJoint::getAnchor(bool isFirst) const {
  dVector3 tmp;
  if (isFirst) {
    dJointGetHingeAnchor(id(), tmp);
  } else {
    dJointGetHingeAnchor2(id(), tmp);
  }
  return ColumnVector3(tmp);
}

void HingeJoint::addTorque(double torque) {
  dJointAddHingeTorque(id(), torque);
}

// Hinge2Joint
Hinge2Joint::Hinge2Joint(World *world, const string &name)
    : Joint(dJointCreateHinge2(world->id(), 0), name) {}

void Hinge2Joint::setAnchor(const ColumnVector3 &anchor) {
  dJointSetHinge2Anchor(id(), anchor(0), anchor(1), anchor(2));
}

void Hinge2Joint::setAxis(const ColumnVector3 &axis, bool isFirst) {
  if (isFirst) {
    dJointSetHinge2Axis1(id(), axis(0), axis(1), axis(2));
  } else {
    dJointSetHinge2Axis2(id(), axis(0), axis(1), axis(2));
  }
}

void Hinge2Joint::setLoStop(double loStop, bool isFirst) {
  dJointSetHinge2Param(id(), paramOffset(dParamLoStop, isFirst), loStop);
}

void Hinge2Joint::setHiStop(double hiStop, bool isFirst) {
  dJointSetHinge2Param(id(), paramOffset(dParamHiStop, isFirst), hiStop);
}

void Hinge2Joint::setVelocity(double vel, bool isFirst) {
  dJointSetHinge2Param(id(), paramOffset(dParamVel, isFirst), vel);
}

void Hinge2Joint::setFMax(double fMax, bool isFirst) {
  dJointSetHinge2Param(id(), paramOffset(dParamFMax, isFirst), fMax);
}

void Hinge2Joint::setFudgeFactor(double fudgeFactor, bool isFirst) {
  dJointSetHinge2Param(id(), paramOffset(dParamFudgeFactor, isFirst),
                       fudgeFactor);
}

void Hinge2Joint::setBounce(double bounce, bool isFirst) {
  dJointSetHinge2Param(id(), paramOffset(dParamBounce, isFirst), bounce);
}

void Hinge2Joint::setCFM(double cfm, bool isFirst) {
  dJointSetHinge2Param(id(), paramOffset(dParamCFM, isFirst), cfm);
}

void Hinge2Joint::setStopERP(double stopERP, bool isFirst) {
  dJointSetHinge2Param(id(), paramOffset(dParamStopERP, isFirst), stopERP);
}

void Hinge2Joint::setStopCFM(double stopCFM, bool isFirst) {
  dJointSetHinge2Param(id(), paramOffset(dParamStopCFM, isFirst), stopCFM);
}

double Hinge2Joint::getAngle(bool) const {
  return dJointGetHinge2Angle1(id());
  // dJointGetHinge2Angle2( id());
  // ODEでaxis2周りのリミット、角度出力などインプリメントされていない
}

double Hinge2Joint::getAngleRate(bool isFirst) const {
  return (isFirst) ? dJointGetHinge2Angle1Rate(id())
                   : dJointGetHinge2Angle2Rate(id());
}

double Hinge2Joint::getLoStop(bool isFirst) const {
  return dJointGetHinge2Param(id(), paramOffset(dParamLoStop, isFirst));
}

double Hinge2Joint::getHiStop(bool isFirst) const {
  return dJointGetHinge2Param(id(), paramOffset(dParamHiStop, isFirst));
}

ColumnVector3 Hinge2Joint::getAnchor(bool isFirst) const {
  dVector3 tmp;
  if (isFirst) {
    dJointGetHinge2Anchor(id(), tmp);
  } else {
    dJointGetHinge2Anchor2(id(), tmp);
  }
  return ColumnVector3(tmp);
}

void Hinge2Joint::addTorque(double torque, bool isFirst) {
  dJointAddHinge2Torques(id(), ((isFirst) ? torque : 0.0),
                         ((isFirst) ? 0.0 : torque));
}

// SliderJoint

SliderJoint::SliderJoint(World *world, const string &name)
    : Joint(dJointCreateSlider(world->id(), 0), name) {}

void SliderJoint::setAxis(const ColumnVector3 &axis) {
  dJointSetSliderAxis(id(), axis(0), axis(1), axis(2));
}

void SliderJoint::setLoStop(double loStop) {
  dJointSetSliderParam(id(), dParamLoStop, loStop);
}

void SliderJoint::setHiStop(double hiStop) {
  dJointSetSliderParam(id(), dParamHiStop, hiStop);
}

void SliderJoint::setVelocity(double vel) {
  dJointSetSliderParam(id(), dParamVel, vel);
}

void SliderJoint::setFMax(double fMax) {
  dJointSetSliderParam(id(), dParamFMax, fMax);
}

void SliderJoint::setFudgeFactor(double fudgeFactor) {
  dJointSetSliderParam(id(), dParamFudgeFactor, fudgeFactor);
}

void SliderJoint::setBounce(double bounce) {
  dJointSetSliderParam(id(), dParamBounce, bounce);
}

void SliderJoint::setCFM(double cfm) {
  dJointSetSliderParam(id(), dParamCFM, cfm);
}

void SliderJoint::setStopERP(double erp) {
  dJointSetSliderParam(id(), dParamStopERP, erp);
}

void SliderJoint::setStopCFM(double stopCFM) {
  dJointSetSliderParam(id(), dParamStopCFM, stopCFM);
}

double SliderJoint::getPosition() const {
  return dJointGetSliderPosition(id());
}

double SliderJoint::getPositionRate() const {
  return dJointGetSliderPositionRate(id());
}

void SliderJoint::addForce(double force) { dJointAddSliderForce(id(), force); }

// UniversalJoint

UniversalJoint::UniversalJoint(World *world, const string &name)
    : Joint(dJointCreateUniversal(world->id(), 0), name) {}

void UniversalJoint::setAnchor(const ColumnVector3 &anchor) {
  dJointSetUniversalAnchor(id(), anchor(0), anchor(1), anchor(2));
}

void UniversalJoint::setAxis(const ColumnVector3 &axis, bool isFirst) {
  if (isFirst) {
    dJointSetUniversalAxis1(id(), axis(0), axis(1), axis(2));
  } else {
    dJointSetUniversalAxis2(id(), axis(0), axis(1), axis(2));
  }
}

void UniversalJoint::setLoStop(double loStop, bool isFirst) {
  dJointSetUniversalParam(id(), paramOffset(dParamLoStop, isFirst), loStop);
}

void UniversalJoint::setHiStop(double hiStop, bool isFirst) {
  dJointSetUniversalParam(id(), paramOffset(dParamHiStop, isFirst), hiStop);
}

void UniversalJoint::setVelocity(double vel, bool isFirst) {
  dJointSetUniversalParam(id(), paramOffset(dParamVel, isFirst), vel);
}

void UniversalJoint::setFMax(double fMax, bool isFirst) {
  dJointSetUniversalParam(id(), paramOffset(dParamFMax, isFirst), fMax);
}

void UniversalJoint::setFudgeFactor(double fudgeFactor, bool isFirst) {
  dJointSetUniversalParam(id(), paramOffset(dParamFudgeFactor, isFirst),
                          fudgeFactor);
}

void UniversalJoint::setBounce(double bounce, bool isFirst) {
  dJointSetUniversalParam(id(), paramOffset(dParamBounce, isFirst), bounce);
}

void UniversalJoint::setCFM(double cfm, bool isFirst) {
  dJointSetUniversalParam(id(), paramOffset(dParamCFM, isFirst), cfm);
}

void UniversalJoint::setStopERP(double stopERP, bool isFirst) {
  dJointSetUniversalParam(id(), paramOffset(dParamStopERP, isFirst), stopERP);
}

void UniversalJoint::setStopCFM(double stopCFM, bool isFirst) {
  dJointSetUniversalParam(id(), paramOffset(dParamStopCFM, isFirst), stopCFM);
}

double UniversalJoint::getAngle(bool isFirst) const {
  return (isFirst) ? dJointGetUniversalAngle1(id())
                   : dJointGetUniversalAngle2(id());
}

double UniversalJoint::getAngleRate(bool isFirst) const {
  return (isFirst) ? dJointGetUniversalAngle1Rate(id())
                   : dJointGetUniversalAngle2Rate(id());
}

double UniversalJoint::getLoStop(bool isFirst) const {
  return dJointGetUniversalParam(id(), paramOffset(dParamLoStop, isFirst));
}

double UniversalJoint::getHiStop(bool isFirst) const {
  return dJointGetUniversalParam(id(), paramOffset(dParamHiStop, isFirst));
}

ColumnVector3 UniversalJoint::getAnchor(bool isFirst) const {
  dVector3 tmp;
  if (isFirst) {
    dJointGetUniversalAnchor(id(), tmp);
  } else {
    dJointGetUniversalAnchor2(id(), tmp);
  }
  return ColumnVector3(tmp);
}

void UniversalJoint::addTorque(double torque, bool isFirst) {
  dJointAddUniversalTorques(id(), ((isFirst) ? torque : 0.0),
                            ((isFirst) ? 0.0 : torque));
}

// PistonJoint

// FixedJoint

FixedJoint::FixedJoint(World *world, const string &name)
    : Joint(dJointCreateFixed(world->id(), 0), name) {
  dJointSetFixed(id());
}

// AMotorJoint

// LMotorJoint

// Plane2DJoint

Plane2DJoint::Plane2DJoint(World *world, const string &name)
    : Joint(dJointCreatePlane2D(world->id(), 0), name) {}
