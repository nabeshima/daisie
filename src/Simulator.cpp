/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Simulator.h"

#include <ode/objects.h>
#include <ode/odeinit.h>
#include <ode/odemath.h>

#include <map>
#include <utility>
#include <vector>

#include "Body.h"
#include "Geometry.h"

using namespace std;
using namespace daisie;

namespace daisie {

struct CollisionFeedback {
  dContactGeom contactGeom;
  dJointFeedback jointFB;
  Geometry *geom1;
  Geometry *geom2;
};

class NearCallback {
 private:
  Simulator *simulator;

  int maxContacts;
  dSurfaceParameters surfaceParams;
  dJointGroupID contactJointGroupID;

  vector<CollisionFeedback *> colVec;

 public:
  NearCallback(Simulator *simulator, int maxContacts,
               const dSurfaceParameters &surfaceParams);
  virtual ~NearCallback();

  static void _nearCallbackFunc(void *data, dGeomID geomID1, dGeomID geomID2);

  virtual void operator()(dGeomID geomID1, dGeomID geomID2);

  void setCollisionFeedback();
};

void NearCallback::_nearCallbackFunc(void *data, dGeomID geomID1,
                                     dGeomID geomID2) {
  (*static_cast<NearCallback *>(data))(geomID1, geomID2);
}

NearCallback::NearCallback(Simulator *simulator, int maxContacts,
                           const dSurfaceParameters &surfaceParams)
    : simulator(simulator),
      maxContacts(maxContacts),
      surfaceParams(surfaceParams),
      contactJointGroupID(dJointGroupCreate(0)) {}

NearCallback::~NearCallback() {
  unsigned int size = colVec.size();
  for (unsigned int i = 0; i < size; ++i) {
    delete colVec[i];
  }

  dJointGroupDestroy(contactJointGroupID);
}

void NearCallback::operator()(dGeomID geomID1, dGeomID geomID2) {
  Geometry *g1 = static_cast<Geometry *>(dGeomGetData(geomID1)),
           *g2 = static_cast<Geometry *>(dGeomGetData(geomID2));

  if (simulator->isIgnored(g1, g2)) {
    return;
  }

  dBodyID body1 = dGeomGetBody(geomID1), body2 = dGeomGetBody(geomID2);

  dWorldID worldID = simulator->getWorld().id();

  dContact *contact = new dContact[maxContacts];

  int colNum = dCollide(geomID1, geomID2, maxContacts, &contact[0].geom,
                        sizeof(dContact));

  bool fbEnabled = (g1->isFeedbackEnabled() || g2->isFeedbackEnabled());

  for (int i = 0; i < colNum; ++i) {
    dJointID jointID = NULL;

    // contact joint is created unless one geometry is RayGeometry.
    if (dGeomGetClass(geomID1) != dRayClass &&
        dGeomGetClass(geomID2) != dRayClass) {
      contact[i].surface = surfaceParams;

      jointID = dJointCreateContact(worldID, contactJointGroupID, contact + i);
      dJointAttach(jointID, body1, body2);
    }

    if (fbEnabled) {
      CollisionFeedback *cfb = new CollisionFeedback();
      cfb->contactGeom = contact[i].geom;

      if (jointID != NULL) {
        dJointSetFeedback(jointID, &(cfb->jointFB));
      }

      cfb->geom1 = g1;
      cfb->geom2 = g2;
      colVec.push_back(cfb);
    }
  }

  delete[] contact;
}

namespace {

ColumnVector3 getRelFromGlobal(dGeomID geomID, const ColumnVector3 &globalVec) {
  dVector3 p;
  const dReal *grot = dGeomGetRotation(geomID);
  p[0] = globalVec[0];
  p[1] = globalVec[1];
  p[2] = globalVec[2];
  p[3] = 0;

  dVector3 result;
  dMultiply1_331(result, grot, p);
  return ColumnVector3(result);
}
}  // namespace

void NearCallback::setCollisionFeedback() {
  unsigned int size = colVec.size();

  for (unsigned int i = 0; i < size; ++i) {
    CollisionFeedback *cfb = colVec[i];

    // for geom1
    {
      CollisionData cdata;
      cdata.opposite = cfb->geom2;
      cdata.depth = cfb->contactGeom.depth;

      if (dGeomGetClass(cfb->contactGeom.g1) == dPlaneClass ||
          dGeomGetClass(cfb->contactGeom.g1) == dRayClass) {
        cdata.absPos.fill(cfb->contactGeom.pos);
        cdata.absNormal.fill(cfb->contactGeom.normal);

        cdata.relPos.fill(cfb->contactGeom.pos);
        cdata.relNormal.fill(cfb->contactGeom.normal);

        cdata.absForce.fill(cfb->jointFB.f1);
        cdata.absTorque.fill(cfb->jointFB.t1);

        cdata.relForce.fill(cfb->jointFB.f1);
        cdata.relTorque.fill(cfb->jointFB.t1);
      } else {
        cdata.absPos.fill(cfb->contactGeom.pos);
        cdata.absNormal.fill(cfb->contactGeom.normal);

        cdata.relPos = getRelFromGlobal(cfb->contactGeom.g1,
                                        ColumnVector3(cfb->contactGeom.pos));
        cdata.relNormal = getRelFromGlobal(
            cfb->contactGeom.g1, ColumnVector3(cfb->contactGeom.normal));

        cdata.absForce.fill(cfb->jointFB.f1);
        cdata.absTorque.fill(cfb->jointFB.t1);

        cdata.relForce = getRelFromGlobal(cfb->contactGeom.g1,
                                          ColumnVector3(cfb->jointFB.f1));
        cdata.relTorque = getRelFromGlobal(cfb->contactGeom.g1,
                                           ColumnVector3(cfb->jointFB.t1));
      }

      cdata.absNormal = -cdata.absNormal;
      cdata.relNormal = -cdata.relNormal;

      cfb->geom1->colDataVec.push_back(cdata);
    }

    // for geom2
    {
      CollisionData cdata;
      cdata.opposite = cfb->geom1;
      cdata.depth = cfb->contactGeom.depth;

      if (dGeomGetClass(cfb->contactGeom.g2) == dPlaneClass ||
          dGeomGetClass(cfb->contactGeom.g2) == dRayClass) {
        cdata.absPos.fill(cfb->contactGeom.pos);
        cdata.absNormal.fill(cfb->contactGeom.normal);

        cdata.relPos.fill(cfb->contactGeom.pos);
        cdata.relNormal.fill(cfb->contactGeom.normal);

        cdata.absForce.fill(cfb->jointFB.f2);
        cdata.absTorque.fill(cfb->jointFB.t2);

        cdata.relForce.fill(cfb->jointFB.f2);
        cdata.relTorque.fill(cfb->jointFB.t2);
      } else {
        cdata.absPos.fill(cfb->contactGeom.pos);
        cdata.absNormal.fill(cfb->contactGeom.normal);

        cdata.relPos = getRelFromGlobal(cfb->contactGeom.g2,
                                        ColumnVector3(cfb->contactGeom.pos));
        cdata.relNormal = getRelFromGlobal(
            cfb->contactGeom.g2, ColumnVector3(cfb->contactGeom.normal));

        cdata.absForce.fill(cfb->jointFB.f2);
        cdata.absTorque.fill(cfb->jointFB.t2);

        cdata.relForce = getRelFromGlobal(cfb->contactGeom.g1,
                                          ColumnVector3(cfb->jointFB.f2));
        cdata.relTorque = getRelFromGlobal(cfb->contactGeom.g1,
                                           ColumnVector3(cfb->jointFB.t2));
      }

      cfb->geom2->colDataVec.push_back(cdata);
    }
  }
}
}  // namespace daisie

Simulator::Simulator(int maxContacts) : maxContacts(maxContacts), simTime(0.0) {
  // dInitODE(); // deprecated
  dInitODE2(0);
  dAllocateODEDataForThread(dAllocateMaskAll);

  //  surfaceParams.mode = dContactBounce | dContactSoftCFM;
  //  surfaceParams.mu = dInfinity;
  //  surfaceParams.mu2 = 0.0;
  //  surfaceParams.bounce = 0.1;
  //  surfaceParams.bounce_vel = 0.1;
  //  surfaceParams.soft_cfm = 0.001;
}

World &Simulator::getWorld() { return world; }

Space &Simulator::getSpace() { return space; }

void Simulator::addIgnoredGeometryPair(dGeomID geom1, dGeomID geom2) {
  ignoredGeomPair.insert(pair<dGeomID, dGeomID>(geom1, geom2));
  ignoredGeomPair.insert(pair<dGeomID, dGeomID>(geom2, geom1));
}

void Simulator::removeIgnoredGeometryPair(dGeomID geom1, dGeomID geom2) {
  {
    pair<multimap<dGeomID, dGeomID>::iterator,
         multimap<dGeomID, dGeomID>::iterator>
        p = ignoredGeomPair.equal_range(geom1);

    multimap<dGeomID, dGeomID>::iterator it = p.first, endIt = p.second;

    while (it != endIt) {
      if (it->second == geom2) {
        ignoredGeomPair.erase(it);
        break;
      }
      ++it;
    }
  }

  {
    pair<multimap<dGeomID, dGeomID>::iterator,
         multimap<dGeomID, dGeomID>::iterator>
        p = ignoredGeomPair.equal_range(geom2);

    multimap<dGeomID, dGeomID>::iterator it = p.first, endIt = p.second;

    while (it != endIt) {
      if (it->second == geom1) {
        ignoredGeomPair.erase(it);
        break;
      }
      ++it;
    }
  }
}

bool Simulator::isIgnored(dGeomID geom1, dGeomID geom2) const {
  pair<multimap<dGeomID, dGeomID>::const_iterator,
       multimap<dGeomID, dGeomID>::const_iterator>
      p = ignoredGeomPair.equal_range(geom1);

  multimap<dGeomID, dGeomID>::const_iterator it = p.first, endIt = p.second;

  while (it != endIt) {
    if (it->second == geom2) {
      return true;
    }
    ++it;
  }

  return false;
}

bool Simulator::addIgnoredGeometryPair(Geometry *geom1, Geometry *geom2) {
  if (geom1 != NULL && geom2 != NULL) {
    addIgnoredGeometryPair(geom1->id(), geom2->id());

    return true;
  }
  return false;
}

bool Simulator::removeIgnoredGeometryPair(Geometry *geom1, Geometry *geom2) {
  if (geom1 != NULL && geom2 != NULL) {
    removeIgnoredGeometryPair(geom1->id(), geom2->id());

    return true;
  }
  return false;
}

bool Simulator::isIgnored(Geometry *geom1, Geometry *geom2) const {
  if (geom1 != NULL && geom2 != NULL) {
    return isIgnored(geom1->id(), geom2->id());
  }
  return false;
}

void Simulator::update(double timeStep) {
  NearCallback *nearCallback =
      new NearCallback(this, maxContacts, surfaceParams);

  space.clearFeedback();

  dSpaceCollide(space.id(), nearCallback, NearCallback::_nearCallbackFunc);

  world.step(timeStep);

  nearCallback->setCollisionFeedback();

  delete nearCallback;

  simTime += timeStep;
}

double Simulator::getTime() const { return simTime; }
